/* application.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */
#include "config.h"
#include <string.h>
#include <picasso/picasso.h>

#include "application.h"
#include "mainwindow.h"
#include "macross.h"
#include "toolbar.h"
#include "files.h"
#include "engine.h"
#include "extracfg.h"
#include "history.h"
#include "favorites.h"
#include "network.h"
#include "autofill.h"
#include "unicode.h"
#include "scripts.h"
#include "webview.h"

#if defined(WIN32)
#include "application_win32.h"
#endif

#ifdef GTK2
#include "application_gtk2.h"
#endif

Application* Application::g_instance = 0;

Application::Application()
	:m_impl(0)
	,m_window(0)
	,m_engine(0)
	,m_extra(0)
	,m_history(0)
	,m_favorites(0)
	,m_autofill(0)
	,m_network(0)
{
	System_init();
	Scripts_init();

	g_instance = this;
	m_impl = new ApplicationImpl(this);
	m_window = new MainWindow;
	m_engine = new Engine;
	m_extra = new ExtraConfig;
	m_history = new History;
	m_favorites = new Favorites;
	m_autofill = new AutoFill;
	m_network = new NetService;
}

Application::~Application()
{
    WebView::shutdown();

	m_extra->save();

	delete m_network;
	delete m_autofill;
	delete m_favorites;
	delete m_history;
	delete m_engine;
	delete m_extra;
	delete m_window;
	delete m_impl;
}

bool Application::init(void* hInst, char* cmdline)
{
	init_config();
	init_extra();
    m_impl->init();
	m_window->setHomeUrl(cmdline);
	return m_window->Create(hInst);
}

bool Application::init(void* hInst, uchar_t* cmdline)
{
	init_config();
	init_extra();
    m_impl->init();
	std::string cstr = Unicode::ConvertUTF16ToUTF8(ustring(cmdline));
	m_window->setHomeUrl(cstr.c_str());
	return m_window->Create(hInst);
}

int Application::run()
{
	return m_impl->run_loop();
}

int Application::bits_pixel(void) const
{
	return m_impl->bits_pixel();
}

int Application::bytes_pixel(void) const
{
	return m_impl->bytes_pixel();
}

int Application::today(void) const
{
	return m_impl->today();
}

ps_color_format Application::color_format(void) const
{
	return m_impl->color_format();
}

void Application::idle_loop(void)
{
	m_window->FireIdle();
}

bool Application::event_loop(void)
{
	//Note: it will be global event loop feature.
	return Widget::dispatchEvents(); 
}

void Application::loop_callback(void)
{
#if !defined(THREAD_RENDER)
	macross_event_dispatch();
#endif
}

void Application::init_config(void)
{
	MC_CONFIG config;
	if (macross_get_config(&config)) {
		config.scrollbar_width = 0;//scrollbar width 0
		config.fontsize = 12;
		config.fixed_fontsize = 12;
		config.default_encoding = CHARSET_UTF8;
		strcpy(config.language, "zh-cn");

		File* f = File::OpenFile(DATA_PATH("config.ini"), File::READ_ONLY, File::BINARY);
		if (f) {
			f->ReadData((unsigned char*)&config, sizeof(MC_CONFIG));
			File::CloseFile(f);
		} else {
			f = File::OpenFile(DATA_PATH("config.ini"), File::READ_WRITE, File::BINARY);
			f->WriteData((unsigned char*)&config, sizeof(MC_CONFIG));
			File::CloseFile(f);
		}
		macross_set_config(&config);
		MainWindow::setUIFontAntialias(config.text_antialias ? true : false);
	}
}

void Application::init_extra(void)
{
	m_extra->load();
}

MainWindow* Application::getMainWindow(void)
{
	return m_window;
}

History* Application::getHistory(void)
{
	return m_history;
}

Favorites* Application::getFavorites(void)
{
	return m_favorites;
}

AutoFill* Application::getAutofill(void)
{
	return m_autofill;
}

NetService* Application::getNetService(void)
{
	return m_network;
}

Engine* Application::getSearchEngine(void)
{
	return m_engine;
}

ExtraConfig* Application::getExtraConfig(void)
{
	return m_extra;
}

Application* Application::getInstance(void)
{
	return g_instance;
}

unsigned long Application::tickCount(void)
{
	return ApplicationImpl::tickCount();
}

bool Application::report(void)
{
#if 0
	std::string mac;
	if (!m_extra->getEquipId(mac)) {
		if (!m_impl->macAddress(mac))
			return false;
		m_extra->setEquipId(mac);
	}
	std::string reportform = std::string("mac=");
	reportform += mac;
	reportform += std::string("&ver="APP_VERSION);
#if defined(M8)
	reportform += std::string("&os=m8");
#elif defined(WINCE)
	reportform += std::string("&os=wince");
#elif defined(WIN32)
	reportform += std::string("&os=win32");
#elif defined(linux)
	reportform += std::string("&os=linux");
#else
	reportform += std::string("&os=unknow");
#endif
	reportform += std::string("&lang=zh_CN");
	reportform += std::string("&pdr=0");
	reportform += std::string("&oem=0");

	std::string url("http://www.zncsoft.net/signal?");
	url += reportform;
	macross_network_request(REQUEST_POST, url.c_str(), 0, 30);	
#endif
	return true;
}

void SetImeStatus(bool b)
{
	if (MainWindow* m = Application::getInstance()->getMainWindow()) {
		if (b)
			m->getImToolBar()->Show();
		else
			m->getImToolBar()->Hide();
	}

	SetImeStatus_platform(b);
}

bool ImeIsShow(void)
{
	return ImeIsShow_platform();
}

