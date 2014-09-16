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
//! \file HounsfieldWidget.cpp
//! \brief The HounsfieldWidget.cpp file contains the definition of
//!        non-inline methods of the HounsfieldWidget class.
//!
//! \author Quentin Smetz
//!

#include "HounsfieldWidget.h"
#include "Model/ProgramConfiguration.h"
using namespace std;
using namespace customwidget;

// Constructor
HounsfieldWidget::HounsfieldWidget(vector<Range> const& ranges, Range const& range, QWidget* parent)
    : Widget(parent), m_hounsfield(range), m_keepWindowIfAsked(true), m_presets(ranges)
{
    // Construct the interface
    QVBoxLayout* vLayoutSpinBox = new QVBoxLayout();
    QVBoxLayout* vLayoutSlider = new QVBoxLayout();

    m_minSpinBox = new DoubleSpinBox();
    m_minSlider = new DoubleSlider(Qt::Horizontal);
    m_maxSpinBox = new DoubleSpinBox();
    m_maxSlider = new DoubleSlider(Qt::Horizontal);
    setMinimum(ranges.at(0).min());
    setMaximum(ranges.at(0).max());

    m_minSpinBox->setValue(range.min());
    m_minSpinBox->setSingleStep(5);
    vLayoutSpinBox->addWidget(m_minSpinBox);

    m_minSlider->setDoubleValue(range.min());
    vLayoutSlider->addWidget(m_minSlider);

    m_maxSpinBox->setValue(range.max());
    m_maxSpinBox->setSingleStep(5);
    vLayoutSpinBox->addWidget(m_maxSpinBox);

    m_maxSlider->setDoubleValue(range.max());
    vLayoutSlider->addWidget(m_maxSlider);

    QVBoxLayout* vLayout = new QVBoxLayout();

    QHBoxLayout* hLayout = new QHBoxLayout();

    hLayout->addLayout(vLayoutSpinBox);
    hLayout->addLayout(vLayoutSlider);

    vLayout->addLayout(hLayout);

    QHBoxLayout* hLayout2 = new QHBoxLayout();

    m_keepWindowWidthCheckBox = new CheckBox("Maintenir la taille du fenêtrage");
    m_keepWindowWidthCheckBox->setChecked(true);
    hLayout2->addWidget(m_keepWindowWidthCheckBox);

    // Load given presets and custom presets
    m_presetsComboBox = new ComboBox();
    m_presetsComboBox->addItem("Personnalisé");
    m_presetNo = 0;
    m_presetsComboBox->setCurrentIndex(0);
    for(unsigned int i = 0 ; i < ranges.size() ; i++)
    {
        QString item = "DICOM : ";
        item += ranges.at(i).toString().c_str();
        m_presetsComboBox->addItem(item);
        if(ranges.at(i) == range)
        {
            m_presetNo = i+1;
            m_presetsComboBox->setCurrentIndex(i+1);
        }

        if(ranges.at(i).min() < m_minSpinBox->minimum())
            setMinimum(ranges.at(i).min());
        if(ranges.at(i).max() > m_minSpinBox->maximum())
            setMaximum(ranges.at(i).max());
    }

    map<string, Range> config_huPresets = ProgramConfiguration::instance()->huPresets();
    for(map<string, Range>::iterator iter = config_huPresets.begin()
        ; iter != config_huPresets.end() ; iter++)
    {
        m_presetsComboBox->addItem(QString(iter->first.c_str()) + QString(" : ")
                                   + QString(iter->second.toString().c_str()));
        m_presets.push_back(iter->second);

        if(iter->second.min() < m_minSpinBox->minimum())
            setMinimum(iter->second.min());
        if(iter->second.max() > m_minSpinBox->maximum())
            setMaximum(iter->second.max());
    }

    hLayout2->addWidget(m_presetsComboBox);

    vLayout->addLayout(hLayout2);

    setLayout(vLayout);

    // Event connections
    connect(m_minSpinBox, SIGNAL(valueChanged(double)), m_minSlider, SLOT(setDoubleValue(double)));
    connect(m_minSlider, SIGNAL(doubleValueChanged(double)), m_minSpinBox, SLOT(setValue(double)));
    connect(m_maxSpinBox, SIGNAL(valueChanged(double)), m_maxSlider, SLOT(setDoubleValue(double)));
    connect(m_maxSlider, SIGNAL(doubleValueChanged(double)), m_maxSpinBox, SLOT(setValue(double)));

    connect(m_minSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateHounsfieldMin()));
    connect(m_maxSpinBox, SIGNAL(valueChanged(double)), this, SLOT(updateHounsfieldMax()));

    connect(m_minSpinBox, SIGNAL(valueChanged(double)), m_presetsComboBox, SLOT(setFirstIndex()));
    connect(m_maxSpinBox, SIGNAL(valueChanged(double)), m_presetsComboBox, SLOT(setFirstIndex()));

    connect(m_presetsComboBox, SIGNAL(activated(int)), this, SLOT(loadPreset(int)));
}

// Destructor
HounsfieldWidget::~HounsfieldWidget()
{}

// The 'setHounsfield' method
void HounsfieldWidget::setHounsfield(Range const& hounsfield)
{
    // Block signals during change...
    m_minSpinBox->blockSignals(true);
    m_minSlider->blockSignals(true);
    m_maxSpinBox->blockSignals(true);
    m_maxSlider->blockSignals(true);
    m_presetsComboBox->blockSignals(true);

    // ... load previous state ...
    m_hounsfield = hounsfield;
    unsigned int index = 1;
    m_presetNo = 0;
    for(vector<Range>::iterator iter = m_presets.begin() ; iter != m_presets.end() ; iter++)
    {
        if(*iter == m_hounsfield)
            m_presetNo = index;
        index++;
    }

    // ... updates the component according to the new state ...
    m_minSpinBox->setValue(m_hounsfield.min());
    m_minSlider->setDoubleValue(m_hounsfield.min());
    m_maxSpinBox->setValue(m_hounsfield.max());
    m_maxSlider->setDoubleValue(m_hounsfield.max());

    m_presetsComboBox->setCurrentIndex(m_presetNo);

    // ... and then reactive signals after change
    m_minSpinBox->blockSignals(false);
    m_minSlider->blockSignals(false);
    m_maxSpinBox->blockSignals(false);
    m_maxSlider->blockSignals(false);
    m_presetsComboBox->blockSignals(false);

    // Signal the state change
    emit newValues();
}

// The 'getMaximumRange' method
Range HounsfieldWidget::getMaximumRange()
{
    return Range(m_minSpinBox->minimum(), m_minSpinBox->maximum());
}

// The 'updateHounsfieldMin' slot
void HounsfieldWidget::updateHounsfieldMin()
{
    double size = m_hounsfield.size();

    // Save new hounsfield value and check if min < max
    m_hounsfield.min() = m_minSpinBox->value();
    if(m_hounsfield.min()+1 > m_hounsfield.max())
    {
        m_maxSpinBox->setValue(m_hounsfield.min()+1);
        m_maxSlider->setDoubleValue(m_hounsfield.min()+1);
    }

    // Change max when the window must keep its size
    if(m_keepWindowIfAsked && m_keepWindowWidthCheckBox->isChecked())
    {
        m_keepWindowIfAsked = false;
        if(m_hounsfield.min()+size != m_hounsfield.max())
        {
            m_maxSpinBox->setValue(m_hounsfield.min()+size);
            m_maxSlider->setDoubleValue(m_hounsfield.min()+size);
        }
        m_keepWindowIfAsked = true;
    }

    // Signal the change
    emit newValues();
}

// The 'updateHounsfieldMax' slot
void HounsfieldWidget::updateHounsfieldMax()
{
    double size = m_hounsfield.size();

    // Save new hounsfield value and check if min < max
    m_hounsfield.max() = m_maxSpinBox->value();
    if(m_hounsfield.max()-1 < m_hounsfield.min())
    {
        m_minSpinBox->setValue(m_hounsfield.max()-1);
        m_minSlider->setDoubleValue(m_hounsfield.max()-1);
    }

    // Change min when the window must keep its size
    if(m_keepWindowIfAsked && m_keepWindowWidthCheckBox->isChecked())
    {
        m_keepWindowIfAsked = false;
        if(m_hounsfield.max()-size != m_hounsfield.min())
        {
            m_minSpinBox->setValue(m_hounsfield.max()-size);
            m_minSlider->setDoubleValue(m_hounsfield.max()-size);
        }
        m_keepWindowIfAsked = true;
    }

    // Signal the change
    emit newValues();
}

// The 'loadPreset' slot
void HounsfieldWidget::loadPreset(int index)
{
    m_presetNo = index;
    if(index == 0)
        return;

    Range newHounsfield = m_presets.at(index-1);

    m_keepWindowIfAsked = false;
    m_minSlider->setDoubleValue(newHounsfield.min());
    m_maxSlider->setDoubleValue(newHounsfield.max());
    m_keepWindowIfAsked = true;

    m_presetsComboBox->setCurrentIndex(index);
}

// The 'setMinimum' method
void HounsfieldWidget::setMinimum(double min)
{
    Range range = m_minSlider->doubleRange();
    range.min() = min;

    m_minSpinBox->setMinimum(min);
    m_minSlider->setDoubleRange(range, 0.1);
    m_maxSpinBox->setMinimum(min);
    m_maxSlider->setDoubleRange(range, 0.1);
}

// The 'setMaximum' method
void HounsfieldWidget::setMaximum(double max)
{
    Range range = m_minSlider->doubleRange();
    range.max() = max;

    m_minSpinBox->setMaximum(max);
    m_minSlider->setDoubleRange(range, 0.1);
    m_maxSpinBox->setMaximum(max);
    m_maxSlider->setDoubleRange(range, 0.1);
}
