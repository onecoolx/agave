/* fillmgr.cpp - Agave application
 *
 * Copyright (C) 2010 Zhang Ji Peng
 * Contact : onecoolx@gmail.com
 */

#include "config.h"
#include <picasso/picasso.h>

#include "fillmgr.h"
#include "shadebtn.h"
#include "autofill.h"
#include "application.h"
#include "mainwindow.h"

AutoFillManager::AutoFillManager(Widget* parent)
    : Panel(parent)
    , m_main(0)
    , m_save(0)
    , m_exit(0)
    , m_notsave(0)
{
    setTitle(U("Password Management"));
    m_save = new ShadeButton(this, ShadeButton::GreenBtn);
    m_save->setClickHandle(EVENT_OBJECT(this), EVENT_OBJECT_FUNC(AutoFillManager, form_save));
    m_save->setText(U("Save Password"));

    m_exit = new ShadeButton(this, ShadeButton::RedBtn);
    m_exit->setClickHandle(EVENT_OBJECT(this), EVENT_OBJECT_FUNC(AutoFillManager, form_exit));
    m_exit->setText(U("Cancel"));

    m_notsave = new ShadeButton(this, ShadeButton::BlueBtn);
    m_notsave->setClickHandle(EVENT_OBJECT(this), EVENT_OBJECT_FUNC(AutoFillManager, form_notsave));
    m_notsave->setText(U("Not Save"));

    clear();
}

AutoFillManager::~AutoFillManager()
{
    delete m_notsave;
    delete m_exit;
    delete m_save;
}

void AutoFillManager::clear(void)
{
    m_oldForm = AutoFill::FormData();
    m_newForm = AutoFill::FormData();
    m_isFinded = false;
}

AutoFill::FormData& AutoFillManager::getOldForm(void)
{
    return m_oldForm;
}

AutoFill::FormData& AutoFillManager::getNewForm(void)
{
    return m_newForm;
}

bool AutoFillManager::isFinded(void)
{
    return m_isFinded;
}

void AutoFillManager::setFinded(bool b)
{
    m_isFinded = b;
}

void AutoFillManager::form_save(void)
{
    if (!m_oldForm.url.empty() && !m_oldForm.formid.empty()) {
        AutoFill* pa = Application::getInstance()->getAutofill();
        if (pa) {
            m_oldForm.values = m_newForm.values;
            pa->updateFormData(m_oldForm);
        }
    } else {
        AutoFill* pa = Application::getInstance()->getAutofill();
        if (pa) {
            pa->addFormData(m_newForm);
        }
    }
    form_exit();
}

void AutoFillManager::form_notsave(void)
{
    if (!m_oldForm.url.empty() && !m_oldForm.formid.empty()) {
        m_oldForm.notsave = 1;
        AutoFill* pa = Application::getInstance()->getAutofill();
        if (pa) {
            pa->updateFormData(m_oldForm);
        }
    } else {
        m_newForm.notsave = 1;
        AutoFill* pa = Application::getInstance()->getAutofill();
        if (pa) {
            pa->addFormData(m_newForm);
        }
    }
    form_exit();
}

void AutoFillManager::form_exit(void)
{
    clear();
    Hide();
}

void AutoFillManager::OnCreate(uint32_t f, int x, int y, int w, int h)
{
    Panel::OnCreate(f, x, y, w, h);

    int b = TOOLBAR_HEIGHT / 30;
    int button_width = width() / 3 - 20 * b;
    int button_height = TOOLBAR_HEIGHT - 4 * b;
    int ystart = titleHeight() + (h - titleHeight() - button_height) / 2;

    m_save->OnCreate(WF_ENABLED | WF_VISIBLE, width() - button_width - b * 10, ystart, button_width, button_height);
    m_exit->OnCreate(WF_ENABLED | WF_VISIBLE, button_width + b * 30, ystart, button_width, button_height);
    m_notsave->OnCreate(WF_ENABLED | WF_VISIBLE, b * 10, ystart, button_width, button_height);

    addChild(m_save);
    addChild(m_exit);
    addChild(m_notsave);
}

void AutoFillManager::setMainWindow(MainWindow* main)
{
    m_main = main;
}
