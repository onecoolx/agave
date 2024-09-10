/* application.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "picasso.h"

class MainWindow;
class Engine;
class ExtraConfig;
class AutoFill;
class History;
class Favorites;
class NetService;
class ApplicationImpl;

class Application
{
public:
	Application();
	~Application();

	bool init(void*, char*);
	bool init(void*, uchar_t*);

	int  run();
	void idle_loop(void);
	bool event_loop(void);
	void loop_callback(void);

	ps_color_format color_format(void) const;
	int bits_pixel(void) const;
	int bytes_pixel(void) const;
	int today(void) const;

	bool report(void);
public:
	static unsigned long tickCount(void);
	static Application* getInstance(void);
	MainWindow* getMainWindow(void);
	History* getHistory(void);
	Favorites* getFavorites(void);
	AutoFill* getAutofill(void);
	Engine* getSearchEngine(void);
	ExtraConfig* getExtraConfig(void);
	NetService* getNetService(void);
private:
	static Application* g_instance;
	void init_config(void);
	void init_extra(void);
	ApplicationImpl* m_impl;
	MainWindow* m_window;
	Engine* m_engine;
	ExtraConfig* m_extra;
	History* m_history;
	Favorites* m_favorites;
	AutoFill* m_autofill;
	NetService* m_network;
};

void SetImeStatus(bool b);
bool ImeIsShow(void);

#endif /*_APPLICATION_H_*/
