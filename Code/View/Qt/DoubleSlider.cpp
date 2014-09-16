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
//! \file DoubleSlider.cpp
//! \brief The DoubleSlider.cpp file contains the definition of non-inline
//!        methods of the DoubleSlider class.
//!
//! \author Quentin Smetz
//!

#include "DoubleSlider.h"
using namespace std;
using namespace customwidget;

// Constructor
DoubleSlider::DoubleSlider(Qt::Orientation orientation, QWidget* parent)
    : Slider(orientation, parent)
{
    setDoubleRange(Range(0, 1), 0.1);
    setDoubleValue(0);
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(sendDoubleValue()));
}

// Destructor
DoubleSlider::~DoubleSlider()
{}

// The 'setDoubleRange' method
void DoubleSlider::setDoubleRange(Range const& doubleRange, double precision)
{
    m_doubleRange = doubleRange;
    setRange(0, ceil(m_doubleRange.size()/precision));
    m_precision = precision;
}

// The 'doubleValue' method
double DoubleSlider::doubleValue() const
{
    return m_doubleRange.absolute(Range(minimum(), maximum()).relative(value()));
}

// The 'setDoubleValue' method
void DoubleSlider::setDoubleValue(double value)
{
    if(fabs(value - doubleValue()) <= m_precision)
        return;

    setValue(Range(minimum(), maximum()).absolute(m_doubleRange.relative(value)));
    sendDoubleValue();
}

// The 'sendDoubleValue' slot
void DoubleSlider::sendDoubleValue()
{
    emit doubleValueChanged(doubleValue());
}
