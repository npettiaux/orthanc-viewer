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
//! \file MergedSeriesViewer.cpp
//! \brief The MergedSeriesViewer.cpp file contains the definition of
//!        non-inline methods of the MergedSeriesViewer class.
//!
//! \author Quentin Smetz
//!

#include "MergedSeriesViewer.h"
using namespace std;

// Constructor
MergedSeriesViewer::MergedSeriesViewer() : Viewer(), m_seriesViewers()
{}

// Destructor
MergedSeriesViewer::~MergedSeriesViewer()
{}

// The 'linkSeriesViewer' method
void MergedSeriesViewer::linkSeriesViewer(SeriesViewer* seriesViewer)
{   
    m_seriesViewers.push_back(seriesViewer);

    vtkProp3D* newProp = seriesViewer->getVtkProp3D();
    renderer()->AddViewProp(newProp);

    renderer()->ResetCamera();
    repaint();
}

// The 'unlinkSeriesViewer' method
void MergedSeriesViewer::unlinkSeriesViewer(SeriesViewer* seriesViewer)
{
    for(vector<SeriesViewer*>::iterator iter = m_seriesViewers.begin()
         ; iter != m_seriesViewers.end() ; iter++)
    {
        if(seriesViewer == *iter)
        {
            m_seriesViewers.erase(iter);
            break;
        }
    }

    vtkProp3D* prevProp = seriesViewer->getVtkProp3D();
    renderer()->RemoveViewProp(prevProp);

    renderer()->ResetCamera();
    repaint();
}

// The 'minSlice' method
double MergedSeriesViewer::minSlice() const
{
    if(m_seriesViewers.size() == 0)
        return 0;

    double min = dynamic_cast<SeriesViewer*>(m_seriesViewers.at(0))->minSlice();
    for(unsigned int i = 1 ; i < m_seriesViewers.size() ; i++)
    {
        double newMin = dynamic_cast<SeriesViewer*>(m_seriesViewers.at(i))->minSlice();
        if(newMin < min)
            min = newMin;
    }

    return min;
}

// The 'maxSlice' method
double MergedSeriesViewer::maxSlice() const
{
    if(m_seriesViewers.size() == 0)
        return 0;

    double max = dynamic_cast<SeriesViewer*>(m_seriesViewers.at(0))->maxSlice();
    for(unsigned int i = 1 ; i < m_seriesViewers.size() ; i++)
    {
        double newMax = dynamic_cast<SeriesViewer*>(m_seriesViewers.at(i))->maxSlice();
        if(newMax > max)
            max = newMax;
    }

    return max;
}

// The 'updateHounsfield' method
void MergedSeriesViewer::updateHounsfield(ViewConfiguration const& config)
{
    repaint();
}

// The 'updateColormap' method
void MergedSeriesViewer::updateColormap(ViewConfiguration const& config)
{
    repaint();
}

// The 'updateTranslation' method
void MergedSeriesViewer::updateTranslation(ViewConfiguration const& config)
{
    repaint();
}

// The 'updateRotation' method
void MergedSeriesViewer::updateRotation(ViewConfiguration const& config)
{
    repaint();
}
