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
//! \file SeriesViewer.cpp
//! \brief The SeriesViewer.cpp file contains the definition of non-inline
//!        methods of the SeriesViewer class.
//!
//! \author Quentin Smetz
//!

#include "SeriesViewer.h"
using namespace std;
using namespace customwidget;

// Constructor
SeriesViewer::SeriesViewer(SeriesData* series)
    : Viewer(), m_series(series), m_vtkProp3D(0)
{}

// Destructor
SeriesViewer::~SeriesViewer()
{
    if(m_vtkProp3D != 0)
        m_vtkProp3D->Delete();
}

// The 'allowFusion' method
void SeriesViewer::allowFusion(bool allow)
{
    if(allow)
    {
        renderer()->RemoveViewProp(m_vtkProp3D);
        setPropOpacity(0.5);
    }
    else
    {
        setPropOpacity(1.0);
        renderer()->AddViewProp(m_vtkProp3D);
        renderer()->ResetCamera();
    }
}
