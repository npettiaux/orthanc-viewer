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
//! \file TranslationRotationDialog.cpp
//! \brief The TranslationRotationDialog.cpp file contains the definition of
//!        non-inline methods of the TranslationRotationDialog class.
//!
//! \author Quentin Smetz
//!

#include "TranslationRotationDialog.h"
using namespace std;
using namespace customwidget;

// Constructor
TranslationRotationDialog::TranslationRotationDialog(QWidget* parent)
    : ViewConfigurationDialog(parent)
{
    setModal(false);
    setWindowTitle("Translation et rotation");

    // Construct the interface
    QHBoxLayout* hLayout = new QHBoxLayout();

    GroupBox* translationBox = new GroupBox("Translation");
    QGridLayout* translationLayout = new QGridLayout();

    m_xSlider = new DoubleSlider(Qt::Vertical);
    m_xSlider->setDoubleRange(Range(-200, 200), 0.1);
    m_xSlider->setDoubleValue(0);
    m_xSlider->setSingleStep(1);
    m_xSlider->setPageStep(10);
    translationLayout->addWidget(m_xSlider, 0, 0);
    translationLayout->addWidget(new Label("X"), 1, 0, Qt::AlignCenter);
    m_ySlider = new DoubleSlider(Qt::Vertical);
    m_ySlider->setDoubleRange(Range(-200, 200), 0.1);
    m_ySlider->setDoubleValue(0);
    m_ySlider->setSingleStep(1);
    m_ySlider->setPageStep(10);
    translationLayout->addWidget(m_ySlider, 0, 1);
    translationLayout->addWidget(new Label("Y"), 1, 1, Qt::AlignCenter);
    m_zSlider = new DoubleSlider(Qt::Vertical);
    m_zSlider->setDoubleRange(Range(-200, 200), 0.1);
    m_zSlider->setDoubleValue(0);
    m_zSlider->setSingleStep(1);
    m_zSlider->setPageStep(10);
    translationLayout->addWidget(m_zSlider, 0, 2);
    translationLayout->addWidget(new Label("Z"), 1, 2, Qt::AlignCenter);

    translationBox->setLayout(translationLayout);
    hLayout->addWidget(translationBox);

    GroupBox* rotationBox = new GroupBox("Rotation");
    QFormLayout* rotationLayout = new QFormLayout();

    m_aDial = new Dial();
    m_aDial->setWrapping(true);
    m_aDial->setRange(0, 360);
    m_aDial->setValue(0);
    m_aDial->setSingleStep(1);
    m_aDial->setPageStep(90);
    rotationLayout->addRow("X", m_aDial);
    m_bDial = new Dial();
    m_bDial->setWrapping(true);
    m_bDial->setRange(0, 360);
    m_bDial->setValue(0);
    m_bDial->setSingleStep(1);
    m_bDial->setPageStep(90);
    rotationLayout->addRow("Y", m_bDial);
    m_cDial = new Dial();
    m_cDial->setWrapping(true);
    m_cDial->setRange(0, 360);
    m_cDial->setValue(0);
    m_cDial->setSingleStep(1);
    m_cDial->setPageStep(90);
    rotationLayout->addRow("Z", m_cDial);

    rotationBox->setLayout(rotationLayout);
    hLayout->addWidget(rotationBox);

    setLayout(hLayout);

    // Initialize the config
    sendCurrentConfiguration();
    m_previousConfig = m_currentConfig;

    // Event connections
    connect(m_xSlider, SIGNAL(doubleValueChanged(double)), this, SLOT(sendCurrentConfiguration()));
    connect(m_ySlider, SIGNAL(doubleValueChanged(double)), this, SLOT(sendCurrentConfiguration()));
    connect(m_zSlider, SIGNAL(doubleValueChanged(double)), this, SLOT(sendCurrentConfiguration()));
    connect(m_aDial, SIGNAL(valueChanged(int)), this, SLOT(sendCurrentConfiguration()));
    connect(m_bDial, SIGNAL(valueChanged(int)), this, SLOT(sendCurrentConfiguration()));
    connect(m_cDial, SIGNAL(valueChanged(int)), this, SLOT(sendCurrentConfiguration()));
}

// Destructor
TranslationRotationDialog::~TranslationRotationDialog()
{}

// The 'updateComponentsFromCurrentConfiguration' method
void TranslationRotationDialog::updateComponentsFromCurrentConfiguration()
{
    m_xSlider->blockSignals(true);
    m_ySlider->blockSignals(true);
    m_zSlider->blockSignals(true);
    m_aDial->blockSignals(true);
    m_bDial->blockSignals(true);
    m_cDial->blockSignals(true);

    m_xSlider->setDoubleValue(m_currentConfig.translation().x());
    m_ySlider->setDoubleValue(m_currentConfig.translation().y());
    m_zSlider->setDoubleValue(m_currentConfig.translation().z());
    m_aDial->setValue(m_currentConfig.rotation().x());
    m_bDial->setValue(m_currentConfig.rotation().y());
    m_cDial->setValue(m_currentConfig.rotation().z());

    m_xSlider->blockSignals(false);
    m_ySlider->blockSignals(false);
    m_zSlider->blockSignals(false);
    m_aDial->blockSignals(false);
    m_bDial->blockSignals(false);
    m_cDial->blockSignals(false);
}

// The 'updateCurrentConfiguration' method
void TranslationRotationDialog::updateCurrentConfigurationFromComponents()
{
    m_currentConfig.setTranslation(Vector3D(static_cast<double>(m_xSlider->doubleValue()),
                                            static_cast<double>(m_ySlider->doubleValue()),
                                            static_cast<double>(m_zSlider->doubleValue())));
    m_currentConfig.setRotation(Vector3D(m_aDial->value(),
                                         m_bDial->value(),
                                         m_cDial->value()));
}

// The 'usefulParams' method
set<ViewConfiguration::ViewParam> TranslationRotationDialog::usefulParams() const
{
    set<ViewConfiguration::ViewParam> viewParams;
    viewParams.insert(ViewConfiguration::TRANSLATION);
    viewParams.insert(ViewConfiguration::ROTATION);
    return viewParams;
}
