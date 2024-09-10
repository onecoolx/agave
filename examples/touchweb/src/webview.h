/* webview.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _WEBVIEW_H_
#define _WEBVIEW_H_

#include <string>
#include "widget.h"

#include "macross.h"

class WebViewImpl;
class MainWindow;
class RenderThread;

class WebView : public Widget
{
public:
	enum {
		Render_Normal,
		Render_SmartFit,
	};
public:
	WebView(Widget* parent);
	virtual ~WebView();
	static void init(void);
    static void shutdown(void);

	virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
	virtual void OnDestroy(void);
	virtual void OnPaint(ps_context* gc, const Rect* r);
	virtual void OnSetFocus(void);
	virtual void OnKillFocus(void);
	virtual void OnMouseEvent(const MouseEvent* e);
	virtual void OnKeyEvent(const KeyEvent* e);
	virtual void OnIdle(void);
	virtual void OnChar(uchar_t code);

	void loadUrl(const std::string& url);
	void loadHtml(const std::string& string, const std::string& refurl);
	void stopLoad(void);
	void goBack(void);
	void goForward(void);
	void reLoad(void);
	void reLayout(void);
	void reFresh(void);
	void resetPage(void);

	//offset position
	int virtualOffsetX(void) const;
	int virtualOffsetY(void) const;

	Rect getContentsRect(void) const;
	Rect getViewportRect(void) const;

	unsigned int loading(void) const;
	void setLoading(unsigned int p, bool finish);

	void Update(const Rect* r);
	const ustring& title(void) const;
	void setTitle(const ustring& title);
	const ustring& location(void) const;
	void setLocation(const ustring& location);
	int renderMode(void) const;
	void setRenderMode(int m);

	bool isNewUrl(void);
	void setNewUrl(bool b);

	float zoomFactor(void) const;
	void setZoomFactor(float scale);

	void setFullMode(bool b);
	void setSize(int w, int h);

	int getLastPosX(void) const;
	int getLastPosY(void) const;
	void savePosition(int x, int y);
	void setViewScroll(int x, int y);

	void setMainWindow(MainWindow* main);
	static bool haveMemory(void);
public:
	WebViewImpl* impl(void) const;
	void autofill_if_needed(void);
public:
    void setPlatformView(MaCrossView* view);
    MaCrossView* getPlatformView(void) const;
public:
    // event callbacks tasks.
	void autofill_task(void* p);
	void update_scroll_view_task(void* p);
private:
	void clear_paint_page(void);
	void paint_scroll_bars(void);
	void clear_scroll_bars(void);
	bool adjust_scroll_increase(int& x, int xoff, int& y, int yoff);
	void update_scroll_view(void);
	WebViewImpl* m_impl;
    static RenderThread* render;
};

#endif/*_WEBVIEW_H_*/

