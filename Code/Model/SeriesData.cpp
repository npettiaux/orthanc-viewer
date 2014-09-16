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
//! \file SeriesData.cpp
//! \brief The SeriesData.cpp file contains the definition of non-inline methods
//!        of the SeriesData class.
//!
//! \author Quentin Smetz
//!

#include "SeriesData.h"
using namespace std;

// Constructor
SeriesData::SeriesData() : vtkImageData(), m_patientName(""), m_studyDesc(""),
    m_seriesDesc(""), m_modality("?")
{
    setRescaleInterceptAndSlope(0, 1);
}

// Destructor
SeriesData::~SeriesData()
{}

// The 'setRescaleInterceptAndSlope' method
void SeriesData::setRescaleInterceptAndSlope(double intercept, double slope)
{
    // Convert the HU values with the current intercept and slope
    for(unsigned int i = 0 ; i < m_basicWindowCenters.size() ; i++)
    {
        m_basicWindowCenters.at(i) = convertFromHU(m_basicWindowCenters.at(i));
        m_basicWindowWidths.at(i) = convertFromHU(m_basicWindowWidths.at(i), true);
    }

    // Update the intercept and slope
    m_rescaleIntercept = intercept;
    m_rescaleSlope = slope;

    // Convert in HU values with the new intercept and slope
    for(unsigned int i = 0 ; i < m_basicWindowCenters.size() ; i++)
    {
        m_basicWindowCenters.at(i) = convertToHU(m_basicWindowCenters.at(i));
        m_basicWindowWidths.at(i) = convertToHU(m_basicWindowWidths.at(i), true);
    }
}

// The 'addBasicWindow' method
void SeriesData::addBasicWindow()
{
    double scalarRange[2];
    GetScalarRange(scalarRange);

    addBasicWindow(convertToHU((scalarRange[1]+scalarRange[0])/2),
                   convertToHU(scalarRange[1]-scalarRange[0], true));
}

// The 'addBasicWindow' method
void SeriesData::addBasicWindow(double center, double width)
{
    m_basicWindowCenters.push_back(center);
    m_basicWindowWidths.push_back(width);
}

// The 'convertToHU' method
double SeriesData::convertToHU(double value, bool isSize) const
{
    if(isSize)
        return value * m_rescaleSlope;
    else
        return value * m_rescaleSlope + m_rescaleIntercept;
}

// The 'convertFromHU' method
double SeriesData::convertFromHU(double value, bool isSize) const
{
    if(isSize)
        return value / m_rescaleSlope;
    else
        return (value - m_rescaleIntercept) / m_rescaleSlope;
}

// The 'computeBasicHounsfieldRanges' method
vector<Range> SeriesData::computeBasicHounsfieldRanges() const
{
    vector<Range> basicHounsfield;
    for(unsigned int i = 0 ; i < m_basicWindowCenters.size() ; i++)
    {
        double wCenter = m_basicWindowCenters.at(i);
        double wWidth = m_basicWindowWidths.at(i);

        basicHounsfield.push_back(Range(wCenter-wWidth/2, wCenter+wWidth/2));
    }

    return basicHounsfield;
}

// The 'getBasicHounsfield' method
Range SeriesData::getBasicHounsfield() const
{
    unsigned int index = 1;
    if(m_basicWindowCenters.size() <= 1)
        index = 0;

    double wCenter = m_basicWindowCenters.at(index);
    double wWidth = m_basicWindowWidths.at(index);
    return Range(wCenter-wWidth/2, wCenter+wWidth/2);
}
