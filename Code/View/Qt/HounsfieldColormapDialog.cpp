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
//! \file HounsfieldColormapDialog.cpp
//! \brief The HounsfieldColormapDialog.cpp file contains the definition of
//!        non-inline methods of the HounsfieldColormapDialog class.
//!
//! \author Quentin Smetz
//!

#include "HounsfieldColormapDialog.h"
using namespace std;
using namespace customwidget;

// Constructor
HounsfieldColormapDialog::HounsfieldColormapDialog
(vector<Range> const& ranges, Range const& range, QWidget* parent)
    : ViewConfigurationDialog(parent)
{
    setModal(false);
    setWindowTitle("Fenêtrage et palette de couleurs");

    // Constructs the interface
    QVBoxLayout* vLayout = new QVBoxLayout();

    GroupBox* boxHounsfield = new GroupBox("Fenêtrage");
    QVBoxLayout* vLayoutHounsfield = new QVBoxLayout();
    m_hounsfieldWidget = new HounsfieldWidget(ranges, range);
    vLayoutHounsfield->addWidget(m_hounsfieldWidget);
    boxHounsfield->setLayout(vLayoutHounsfield);
    vLayout->addWidget(boxHounsfield);

    GroupBox* boxColormap = new GroupBox("Palette de couleurs");
    QVBoxLayout* vLayoutColormap = new QVBoxLayout();
    m_colormapWidget = new ColormapWidget(m_hounsfieldWidget->getMaximumRange());
    vLayoutColormap->addWidget(m_colormapWidget);
    boxColormap->setLayout(vLayoutColormap);
    vLayout->addWidget(boxColormap);

    setLayout(vLayout);

    sendCurrentConfiguration();
    m_previousConfig = m_currentConfig;

    // Event connections
    connect(m_hounsfieldWidget, SIGNAL(newValues()), this, SLOT(sendCurrentConfiguration()));
    connect(m_colormapWidget, SIGNAL(newValues()), this, SLOT(sendCurrentConfiguration()));
}

// Destructor
HounsfieldColormapDialog::~HounsfieldColormapDialog()
{}

// The 'getMaximumRange' method
Range HounsfieldColormapDialog::getMaximumRange()
{
    return m_hounsfieldWidget->getMaximumRange();
}

// The 'updateComponentsFromCurrentConfiguration' method
void HounsfieldColormapDialog::updateComponentsFromCurrentConfiguration()
{
    m_hounsfieldWidget->blockSignals(true);
    m_colormapWidget->blockSignals(true);

    m_hounsfieldWidget->setHounsfield(m_currentConfig.hounsfield());
    m_colormapWidget->setColormap(m_currentConfig.colormap());

    m_colormapWidget->setFocusRange(m_currentConfig.hounsfield());

    m_hounsfieldWidget->blockSignals(false);
    m_colormapWidget->blockSignals(false);
}

// The 'updateCurrentConfigurationFromComponents' method
void HounsfieldColormapDialog::updateCurrentConfigurationFromComponents()
{
    m_currentConfig.setHounsfield(m_hounsfieldWidget->getHounsfield(), getMaximumRange());
    m_currentConfig.setColormap(m_colormapWidget->getColormap());

    m_colormapWidget->setFocusRange(m_currentConfig.hounsfield());
}

// The 'usefulParams' method
set<ViewConfiguration::ViewParam> HounsfieldColormapDialog::usefulParams() const
{
    set<ViewConfiguration::ViewParam> viewParams;
    viewParams.insert(ViewConfiguration::HOUNSFIELD);
    viewParams.insert(ViewConfiguration::COLORMAP);
    return viewParams;
}
