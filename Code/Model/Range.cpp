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
//! \file Range.cpp
//! \brief The Range.cpp file contains the definition of non-inline methods
//!        of the Range class.
//!
//! \author Quentin Smetz
//!

#include "Range.h"
using namespace std;

double Range::EQUAL_PRECISION = 1e-16;

// Constructor
Range::Range() : m_min(0), m_max(0)
{}

// Constructor
Range::Range(double min, double max) : m_min(min), m_max(max)
{}

// Destructor
Range::~Range()
{}

// The 'operator==' method
bool Range::operator==(Range const& range) const
{
    return fabs(m_min - range.m_min) < EQUAL_PRECISION
            && fabs(m_max - range.m_max) < EQUAL_PRECISION;
}

// The 'toString' method
std::string Range::toString() const
{
    ostringstream oss;
    oss << "[" << m_min << ", " << m_max << "]";
    return oss.str();
}

// The 'bound' method
double Range::bound(double value) const
{
    if(value < m_min)
        return m_min;
    else if(value > m_max)
        return m_max;
    else
        return value;
}

// The 'contains' method
bool Range::contains(double value) const
{
    return (value >= m_min && value <= m_max);
}

// The 'relative' method
double Range::relative(double value, bool mustBound) const
{
    double pos = (value - m_min) / (m_max - m_min);
    if(mustBound)
    {
        Range range(0, 1);
        pos = range.bound(pos);
    }
    return pos;
}

// The 'relative' method
Range Range::relative(Range range, bool mustBound) const
{
    return Range(relative(range.min(), mustBound), relative(range.max(), mustBound));
}

// The 'absolute' method
double Range::absolute(double value, bool mustBound) const
{
    double pos = m_min + value * (m_max - m_min);
    if(mustBound)
        pos = bound(pos);
    return pos;
}

// The 'absolute' method
Range Range::absolute(Range range, bool mustBound) const
{
    return Range(absolute(range.min(), mustBound), absolute(range.max(), mustBound));
}
