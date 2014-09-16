/**
 * Copyright (c) 2013-2014 Quentin Smetz <qsmetz@gmail.com>, Sebastien
 * Jodogne <s.jodogne@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/


//!
//! \file ViewConfigurationDialog.cpp
//! \brief The ViewConfigurationDialog.cpp file contains the definition of
//!        non-inline methods of the ViewConfigurationDialog class.
//!
//! \author Quentin Smetz
//!

#include "ViewConfigurationDialog.h"
using namespace std;
using namespace customwidget;

// Constructor
ViewConfigurationDialog::ViewConfigurationDialog(QWidget* parent)
    : OkCancelDialog(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout();

    m_configArea = new QWidget();
    mainLayout->addWidget(m_configArea);

    QHBoxLayout* hLayout = new QHBoxLayout();

    m_realTimeCheckBox = new CheckBox("Mettre à jour en temps réel");
    m_realTimeCheckBox->setChecked(true);
    hLayout->addWidget(m_realTimeCheckBox);

    m_resetButton = new PushButton("Réinitialiser");
    hLayout->addWidget(m_resetButton);

    mainLayout->addLayout(hLayout);

    OkCancelDialog::setLayout(mainLayout);

    // Event connections
    connect(m_realTimeCheckBox, SIGNAL(clicked()), this, SLOT(sendCurrentConfiguration()));
    connect(m_resetButton, SIGNAL(clicked()), this, SLOT(reset()));
}

// Destructor
ViewConfigurationDialog::~ViewConfigurationDialog()
{}

// The 'layout' method
QLayout* ViewConfigurationDialog::layout() const
{
    return m_configArea->layout();
}

// The 'setLayout' method
void ViewConfigurationDialog::setLayout(QLayout* layout)
{
    layout->setMargin(0);
    m_configArea->setLayout(layout);
}

// The 'accept' slot
void ViewConfigurationDialog::accept()
{
    m_previousConfig = m_currentConfig;
    sendCurrentConfiguration(false);
    OkCancelDialog::accept();
}

// The 'reject' slot
void ViewConfigurationDialog::reject()
{
    reset();
    OkCancelDialog::reject();
}

// The 'reset' slot
void ViewConfigurationDialog::reset()
{
    m_currentConfig = m_previousConfig;
    sendCurrentConfiguration(true, false);
    updateComponentsFromCurrentConfiguration();
}

// The 'sendNewConfiguration' slot
void ViewConfigurationDialog::sendCurrentConfiguration(bool ifRealTime, bool updateBefore)
{
    if(updateBefore)
        updateCurrentConfigurationFromComponents();

    if(!ifRealTime || m_realTimeCheckBox->isChecked())
    {
        set<ViewConfiguration::ViewParam> const& params = usefulParams();
        for(set<ViewConfiguration::ViewParam>::iterator param = params.begin()
            ; param != params.end() ; param++)
            emit newConfiguration(m_currentConfig, *param);
    }
}
