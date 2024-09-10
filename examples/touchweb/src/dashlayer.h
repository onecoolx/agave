/* Dashlayer.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _DASHLAYER_H_
#define _DASHLAYER_H_

#include <string>
#include "layer.h"
#include "picasso.h"

class DashLayer : public Layer
{
public:
	DashLayer(Widget* parent);
	virtual ~DashLayer();

	void setTitle(const ustring& title);
	void setCommitText(const ustring& text);
	void setCancelText(const ustring& text);
	void setCommit(bool b) { m_scombtn = b; }
	void setCancel(bool b) { m_scanbtn = b; }

	virtual void OnPaint(ps_context* gc, const Rect* r);
	virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);
	virtual void OnMouseEvent(const MouseEvent* e);

	virtual void OnCommit(void);
	virtual void OnCancel(void);

	virtual void Active(void);
	virtual void Commit(void);
	virtual void Cancel(void);

	static ps_font* getTitleFont(void);
protected:
	void Show(void);
	void Hide(void);
private:
	void draw_title(ps_context* gc);
	void draw_btns(ps_context* gc, const Rect& rc);
	void draw_btnText(ps_context* gc, const ustring& text, const Rect& r);
	void draw_lights(ps_context* gc, const Rect& r);
	void button_event(void * p);
	void init_layer(void);
	ustring m_title;
	ustring m_comText;
	ustring m_canText;
	Rect m_commit;
	Rect m_cancel;
	Rect m_event;
	bool m_scombtn;
	bool m_scanbtn;
	int  m_btn;
	static ps_font* tfont;
	static ps_gradient* grads[4];
	static ps_matrix* mtx;
	static bool init;
};

#endif/*_DASHLAYER_H_*/
