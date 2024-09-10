/* application_win32.cpp - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */
#include "config.h"
#include <windows.h>
#if defined(WIN32) && (_MSC_VER >= 1400) 
#include <iphlpapi.h>
#endif
#include "macross.h"
#include "application.h"
#include "application_win32.h"

#ifdef WINCE
#include <objbase.h>
#include <aygshell.h>
#include <GetDeviceUniqueId.h>
#endif

//{45F419A3-1089-4a14-80B7-4CDD72D7AB2E}
static const GUID g_guid = {0x45F419A3, 0x1089, 0x4A14, {0x80, 0xB7, 0x4C, 0xDD, 0x72, 0xD7, 0xAB, 0x2E}};

#ifdef WINCE
int global_scale = 1;
#endif


ApplicationImpl::ApplicationImpl(Application* app)
	: m_data(app)
	, m_cbit(0)
	, m_cbyte(0)
	, m_format(COLOR_FORMAT_UNKNOWN)

{
	HDC hdc = GetDC(0); 
	m_cbit = GetDeviceCaps(hdc, BITSPIXEL);	
	m_cbyte = m_cbit/8;
	ReleaseDC(0, hdc);

	if (m_cbyte == 4) {
		m_format = COLOR_FORMAT_BGRA;
		macross_initialize(PIXEL_FORMAT_BGRA32, screen_width(), screen_height());
	} else if (m_cbyte == 2) {
		m_format = COLOR_FORMAT_RGB565;
		macross_initialize(PIXEL_FORMAT_RGB16, screen_width(), screen_height());
	}
}

ApplicationImpl::~ApplicationImpl()
{
	macross_shutdown();
}

int ApplicationImpl::screen_width(void) const
{
	HDC hdc = GetDC(NULL);
	int width = GetDeviceCaps(hdc, HORZRES);
	ReleaseDC(NULL, hdc);
	return width;
}

int ApplicationImpl::screen_height(void) const
{
	HDC hdc = GetDC(NULL);
	int height = GetDeviceCaps(hdc, VERTRES);
	ReleaseDC(NULL, hdc);
	return height;
}

int ApplicationImpl::run_loop(void)
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			m_data->event_loop();
		} else {
			if (!m_data->event_loop()) {
				m_data->idle_loop();
			}
		    Sleep(10);
			m_data->loop_callback();
		}
	}
	return (int)msg.wParam;
}

int ApplicationImpl::today(void) const
{
	int today = 0;
	SYSTEMTIME sm;
	GetLocalTime(&sm);

	today = sm.wYear;
	today *= 100;
	today += sm.wMonth;
	today *= 100;
	today += sm.wDay;
	return today;
}

unsigned long ApplicationImpl::tickCount(void)
{
	return GetTickCount();
}

bool ApplicationImpl::macAddress(std::string& retstr)
{
#if defined(WINCE)
	GUID guid = {0};
	if (S_OK != CoCreateGuid(&guid))
		return false;

	BYTE equid[GETDEVICEUNIQUEID_V1_OUTPUT+1] = {0};
	DWORD esize = GETDEVICEUNIQUEID_V1_OUTPUT;

	HRESULT hr = GetDeviceUniqueID(reinterpret_cast<LPBYTE>(const_cast<LPGUID>(&guid)),
									sizeof(guid),
									GETDEVICEUNIQUEID_V1,
									equid,
									&esize);
	if (SUCCEEDED (hr)) {
		char inmac[GETDEVICEUNIQUEID_V1_OUTPUT*2+1] = {0};
		sprintf(inmac,"%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",
				equid[0], equid[1], equid[2], equid[3],
				equid[4], equid[5], equid[6], equid[7],
				equid[8], equid[9], equid[10], equid[11],
				equid[12], equid[13], equid[14], equid[15],
				equid[16], equid[17], equid[18], equid[19]);
		retstr = std::string(inmac);
		return true;
	} else
		return false;
#elif defined(WIN32) && (_MSC_VER >= 1400) 
	unsigned long len = 0; 
	if (ERROR_BUFFER_OVERFLOW != GetAdaptersInfo(0, &len))
		return false;

	unsigned char* buf = new unsigned char[len];
	PIP_ADAPTER_INFO padap = reinterpret_cast<PIP_ADAPTER_INFO>(buf);
	GetAdaptersInfo(padap, &len);
	char inmac[16] = {0};
	sprintf(inmac,"%02X%02X%02X%02X%02X%02X",
		  			padap->Address[0],
		  			padap->Address[1],
		  			padap->Address[2],
		  			padap->Address[3],
		  			padap->Address[4],
		  			padap->Address[5]);	
	delete [] buf;
	retstr = std::string(inmac);
	return true;
#else
	return false;
#endif
}

void SetImeStatus_platform(bool b)
{
#ifdef WINCE
	if (b)
		SipShowIM(SIPF_ON);
	else
		SipShowIM(SIPF_OFF);
#endif
}

bool ImeIsShow_platform(void)
{
#ifdef WINCE
	SIPINFO info = {sizeof(SIPINFO), SPI_GETSIPINFO, {0,0,0,0}, {0,0,0,0}, 0, 0};
	if (SipGetInfo(&info)) {
		if (info.fdwFlags & SIPF_ON) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
#else
	return false;
#endif
}

void System_init(void)
{
#ifdef WINCE
	HDC hdc = GetDC(0);
	int dpi = GetDeviceCaps(hdc, LOGPIXELSX);
	global_scale = (dpi/96) ? (dpi/96) : 1;
	ReleaseDC(0, hdc);
#if defined(M8)
	CoInitializeEx(0, COINIT_MULTITHREADED);
#endif
#endif
}

