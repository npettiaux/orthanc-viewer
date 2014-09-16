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
//! \file ColorbarWidget.cpp
//! \brief The ColorbarWidget.cpp file contains the definition of non-inline
//!        methods of the ColorbarWidget class.
//!
//! \author Quentin Smetz
//!

#include "ColorbarWidget.h"
using namespace std;
using namespace customwidget;

// Constructor
ColorbarWidget::ColorbarWidget(Colormap* colormap, Range const& barRange, QWidget* parent)
    : Widget(parent), m_colormap(colormap), m_barRange(barRange), m_focusRange(barRange)
{
    // Construct interface...
    QVBoxLayout* vLayout = new QVBoxLayout();

    QHBoxLayout* hLayout = new QHBoxLayout();

    // ... label for min bar range
    m_labelMin = new Label(QString::number(m_barRange.min()));
    m_labelMin->setAlignment(Qt::AlignCenter);
    hLayout->addWidget(m_labelMin);

    // ... label for max bar range
    m_labelMax = new Label(QString::number(m_barRange.max()));
    m_labelMax->setAlignment(Qt::AlignCenter);
    hLayout->addWidget(m_labelMax);

    vLayout->addLayout(hLayout);

    // ... splitter
    m_splitter = new Splitter(Qt::Horizontal);
    m_splitter->setFrameStyle(QFrame::Box | QFrame::Raised);
    m_splitter->setFixedHeight(30);
    m_splitter->setLineWidth(2);
    m_splitter->setHandleWidth(1);
    m_splitter->setChildrenCollapsible(true);
    vLayout->addWidget(m_splitter);

    setLayout(vLayout);

    // Event connections
    connect(m_splitter, SIGNAL(splitterMoved(int, int)), this, SLOT(updateColorStart(int, int)));

    connect(m_colormap, SIGNAL(colorAdded(uint, double, QColor)), this, SLOT(addColor()));
    connect(m_colormap, SIGNAL(colorModified(uint,double,QColor)), this, SLOT(repaint()));
    connect(m_colormap, SIGNAL(interpolationModeModified(InterpolationMode)), this, SLOT(repaint()));
    connect(m_colormap, SIGNAL(modified()), this, SLOT(repaint()));
}

// Destructor
ColorbarWidget::~ColorbarWidget()
{}

// The 'setFocusRange' method
void ColorbarWidget::setFocusRange(Range const& range)
{
    m_focusRange = range;
    repaint();
}

// The 'getBoundaries' method
Range ColorbarWidget::getBoundaries() const
{
    switch(m_colormap->interpolationMode())
    {
        case Colormap::LINEAR:
            return m_focusRange;

        case Colormap::CONSTANT:
            return m_barRange;
    }

    return Range(0, 0);
}

// The 'addColor' slot
void ColorbarWidget::addColor(bool mustRepaint)
{
    // Creates and add a new widget for the color at the right position
    QWidget* colorW = new QWidget();
    colorW->setAutoFillBackground(true);
    m_splitter->addWidget(colorW);

    if(mustRepaint)
        repaint();
}

// The 'updateColorStart' slot
void ColorbarWidget::updateColorStart(int pos, int index)
{
    static int fromIndex = -1;

    // Update colormap according to the new splitter handle pos
    double width = static_cast<double>(m_splitter->geometry().width());
    m_colormap->setStartAt(index-1, static_cast<double>(pos)/width);

    // Check if splitter handle is against another and update it if any
    if(index != 1 && m_splitter->widget(index-1)->geometry().width() == 0 && index-1 != fromIndex)
    {
        fromIndex = index;
        updateColorStart(pos, index-1);
    }
    if(index != m_splitter->count()-1 && m_splitter->widget(index)->geometry().width() == 0 && index+1 != fromIndex)
    {
        fromIndex = index;
        updateColorStart(pos, index+1);
    }

    fromIndex = -1;

    repaint();
}

// The 'repaint' method
void ColorbarWidget::repaint()
{
    m_splitter->blockSignals(true);

    // Check if there is the correct number of color widget
    while(static_cast<int>(m_colormap->count()) < m_splitter->count()-1)
        delete m_splitter->widget(0);
    while(static_cast<int>(m_colormap->count()) > m_splitter->count()-1)
        addColor(false);

    // Adapt labels
    Range boundaries = getBoundaries();
    m_labelMin->setText(QString::number(boundaries.min()));
    m_labelMax->setText(QString::number(boundaries.max()));

    // Some initializations
    double const width = m_splitter->geometry().width() - m_splitter->lineWidth()*2 - m_colormap->count();
    QList<int> s;
    double start = 0, end = 1;

    // Check the size of the default color
    if(m_colormap->count() > 0)
        s.append(floor(0.5 + (m_colormap->startAt(0)-start) * width));
    else
        s.append(floor(0.5+width));

    // Adjust widget size one by one
    for(unsigned int i = 0 ; i < m_colormap->count() ; i++)
    {
        // Check start and end position of the current color widget
        start = m_colormap->startAt(i);
        if(i+1 < m_colormap->count())
            end = m_colormap->startAt(i+1);
        else
            end = 1;

        // Compute size of the widget and gradient color
        s.append(floor(0.5+(end-start) * width));
    }

    // Updates sizes
    m_splitter->setSizes(s);

    // Create the transfert function
    vtkSmartPointer<vtkColorTransferFunction> func;
    func.TakeReference(m_colormap->computeVTKColorTransferFunction(m_barRange, m_focusRange));
    // Get color table for each of splitter's pixels
    double* table = new double[3 * static_cast<int>(width)];
    func->GetTable(boundaries.min(), boundaries.max(), width, table);

    // Add color according to effective sizes
    s = m_splitter->sizes();
    unsigned int sum = 0;
    for(int i = 0 ; i < s.count() ; i++)
    {
        paint(m_splitter->widget(i), Range(sum, sum+s.at(i)), table);
        sum += s.at(i);
    }

    // Free
    delete[] table;

    m_splitter->blockSignals(false);
    Widget::repaint();
}

// The 'paint' protected method
void ColorbarWidget::paint(QWidget* w, Range const& range, double table[])
{
    // Linear gradient with a color for each (int) pos of the range, according the table
    QLinearGradient gradient(0, 0, range.size(), 0);
    for(unsigned int c = range.min() ; c < range.max() ; c++)
        gradient.setColorAt(range.relative(c),
                            QColor::fromRgbF(table[3*c], table[3*c+1], table[3*c+2]));

    // Update palette according the gradient and apply it on the widget
    QPalette pal(palette());
    QBrush brush = QBrush(gradient);
    pal.setBrush(QPalette::Window, brush);
    w->setPalette(pal);
}

// The 'resizeEvent' protected method
void ColorbarWidget::resizeEvent(QResizeEvent* event)
{
    repaint();
    Widget::resizeEvent(event);
}
