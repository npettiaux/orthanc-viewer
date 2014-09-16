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
//! \file ColormapWidget.cpp
//! \brief The ColormapWidget.cpp file contains the definition of non-inline
//!        methods of the ColormapWidget class.
//!
//! \author Quentin Smetz
//!

#include "ColormapWidget.h"
#include "Model/ProgramConfiguration.h"
using namespace std;
using namespace customwidget;

// Constructor
ColormapWidget::ColormapWidget(Range const& barRange, QWidget* parent)
    : Widget(parent), m_colormap()
{
    // Create the interface
    QVBoxLayout* colorLayout = new QVBoxLayout();

    m_colorbar = new ColorbarWidget(&m_colormap, barRange);
    colorLayout->addWidget(m_colorbar);

    QHBoxLayout* hLayout = new QHBoxLayout();

    QVBoxLayout* colorViewLayout = new QVBoxLayout();

    m_hueDial = new Dial();
    m_hueDial->setRange(0, 32);
    m_hueDial->setValue(0);
    m_hueDial->setSingleStep(1);
    m_hueDial->setNotchesVisible(true);
    m_hueDial->setWrapping(true);
    colorViewLayout->addWidget(m_hueDial);

    m_colorViewWidget = new Widget();
    m_colorViewWidget->setMinimumHeight(20);
    m_colorViewWidget->setAutoFillBackground(true);
    colorViewLayout->addWidget(m_colorViewWidget);

    hLayout->addLayout(colorViewLayout);

    m_satSlider = new DoubleSlider(Qt::Vertical);
    m_satSlider->setDoubleRange(Range(0, 1), 0.05);
    m_satSlider->setDoubleValue(1);
    hLayout->addWidget(m_satSlider);

    m_valSlider = new DoubleSlider(Qt::Vertical);
    m_valSlider->setDoubleRange(Range(0, 1), 0.05);
    m_valSlider->setDoubleValue(1);
    hLayout->addWidget(m_valSlider);

    updateColorViewWidget();

    GroupBox* modeGroupBox = new GroupBox("Interpolation");

    QVBoxLayout* modeLayout = new QVBoxLayout();
    m_interpolationModeButtons[Colormap::LINEAR] = new RadioButton("Linéaire");
    m_interpolationModeButtons[Colormap::LINEAR]->setChecked(true);
    modeLayout->addWidget(m_interpolationModeButtons[Colormap::LINEAR]);
    m_modeButtonGroup.addButton(m_interpolationModeButtons[Colormap::LINEAR],
                                static_cast<int>(Colormap::LINEAR));
    m_interpolationModeButtons[Colormap::CONSTANT] = new RadioButton("Constant");
    modeLayout->addWidget(m_interpolationModeButtons[Colormap::CONSTANT]);
    m_modeButtonGroup.addButton(m_interpolationModeButtons[Colormap::CONSTANT],
                                static_cast<int>(Colormap::CONSTANT));
    modeGroupBox->setLayout(modeLayout);

    hLayout->addWidget(modeGroupBox);

    QVBoxLayout* buttonsLayout = new QVBoxLayout();

    QHBoxLayout* addLayout = new QHBoxLayout();

    m_addColorButton = new PushButton("Ajouter une couleur");
    addLayout->addWidget(m_addColorButton);

    m_startSpinBox = new DoubleSpinBox();
    m_startSpinBox->setRange(barRange.min(), barRange.max());
    m_startSpinBox->setValue(barRange.max());
    addLayout->addWidget(m_startSpinBox);

    buttonsLayout->addLayout(addLayout);

    m_clearColorsButton = new PushButton("Supprimer tout");
    buttonsLayout->addWidget(m_clearColorsButton);

    m_loadLutButton = new PushButton("Charger LUT");
    buttonsLayout->addWidget(m_loadLutButton);

    hLayout->addLayout(buttonsLayout);

    colorLayout->addLayout(hLayout);

    setLayout(colorLayout);

    // Event connections
    connect(m_addColorButton, SIGNAL(clicked()), this, SLOT(addNewColor()));
    connect(m_clearColorsButton, SIGNAL(clicked()), &m_colormap, SLOT(clear()));
    connect(m_hueDial, SIGNAL(valueChanged(int)), this, SLOT(updateColorViewWidget()));
    connect(m_satSlider, SIGNAL(doubleValueChanged(double)), this, SLOT(updateColorViewWidget()));
    connect(m_valSlider, SIGNAL(doubleValueChanged(double)), this, SLOT(updateColorViewWidget()));

    connect(&m_colormap, SIGNAL(colorAdded(uint, double, QColor)), this, SIGNAL(newValues()));
    connect(&m_colormap, SIGNAL(colorModified(uint, double, QColor)), this, SIGNAL(newValues()));
    connect(&m_colormap, SIGNAL(interpolationModeModified(InterpolationMode)), this, SIGNAL(newValues()));
    connect(&m_colormap, SIGNAL(modified()), this, SIGNAL(newValues()));

    connect(&m_modeButtonGroup, SIGNAL(buttonClicked(int)), this, SLOT(updateColormap()));
    connect(m_loadLutButton, SIGNAL(clicked()), this, SLOT(loadLut()));
}

// Destructor
ColormapWidget::~ColormapWidget()
{}

// The 'setColormap' method
void ColormapWidget::setColormap(Colormap const& colormap)
{
    m_colormap = colormap;
    repaint();
    emit newValues();
}

// The 'setFocusRange' method
void ColormapWidget::setFocusRange(Range const& range)
{
    m_colorbar->setFocusRange(range);
}

// The 'updateColormap' slot
void ColormapWidget::updateColormap()
{
    m_colormap.setInterpolationMode(
                static_cast<Colormap::InterpolationMode>(m_modeButtonGroup.checkedId()));
}

// The 'addNewColor' slot
void ColormapWidget::addNewColor()
{
    double start = m_colorbar->getBoundaries().relative(m_startSpinBox->value());
    m_colormap.addColor(start, getCurrentColor());
}

// The 'updateColorViewWidget' slot
void ColormapWidget::updateColorViewWidget()
{
    QPalette pal(palette());
    pal.setColor(QPalette::Button, getCurrentColor());
    pal.setColor(QPalette::Window, getCurrentColor());
    pal.setColor(QPalette::WindowText, getCurrentColor());

    m_colorViewWidget->setPalette(pal);
}


// The 'loadLut' slot
void ColormapWidget::loadLut()
{
    QString name = QFileDialog::getOpenFileName(this, "Charger LUT",
                                                ProgramConfiguration::instance()->lutDirectory().c_str(),
                                                "LUT Files (*.lut)");
    if(name.isEmpty())
        return;

    if(!m_colormap.loadFromLutFile(name))
        QMessageBox::critical(this, "Erreur", "Le fichier LUT n'est pas valide");
}

// The 'repaint' slot
void ColormapWidget::repaint()
{
    m_interpolationModeButtons[m_colormap.interpolationMode()]->setChecked(true);
    Widget::repaint();
}

// The 'getCurrentColor' method
QColor ColormapWidget::getCurrentColor() const
{
    double hue = static_cast<double>(m_hueDial->value());
    hue /= static_cast<double>(m_hueDial->maximum());

    double sat = m_satSlider->doubleValue();

    double val = m_valSlider->doubleValue();

    return QColor::fromHsvF(hue, sat, val);
}
