/* fillmgr.h - MaCross application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#ifndef _AUTOFILL_MANAGER_H_
#define _AUTOFILL_MANAGER_H_

#include "panel.h"
#include "autofill.h"

class MainWindow;
class ShadeButton;

class AutoFillManager : public Panel
{
public:
    AutoFillManager(Widget* parent);
    virtual ~AutoFillManager();

    virtual void OnCreate(uint32_t flags, int x, int y, int w, int h);

    void clear(void);
    bool isFinded(void);
    void setFinded(bool b);
    AutoFill::FormData& getOldForm(void);
    AutoFill::FormData& getNewForm(void);

    void setMainWindow(MainWindow* main);
private:
    void form_save(void);
    void form_notsave(void);
    void form_exit(void);
    MainWindow* m_main;
    ShadeButton* m_save;
    ShadeButton* m_exit;
    ShadeButton* m_notsave;

    AutoFill::FormData m_newForm;
    AutoFill::FormData m_oldForm;
    bool m_isFinded;
};

#endif /*_AUTOFILL_MANAGER_H_*/
