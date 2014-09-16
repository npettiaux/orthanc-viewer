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
//! \file ViewConfiguration.cpp
//! \brief The ViewConfiguration.cpp file contains the definition of non-inline
//!        methods of the ViewConfiguration class.
//!
//! \author Quentin Smetz
//!

#include "ViewConfiguration.h"
using namespace std;

// Constructor
ViewConfiguration::ViewConfiguration()
{}

// Destructor
ViewConfiguration::~ViewConfiguration()
{}

// The 'setHounsfield' method
void ViewConfiguration::setHounsfield(Range const& hounsfield, Range const& hounsfieldMaxRange)
{
    m_hounsfield = hounsfield;
    m_hounsfieldMaxRange = hounsfieldMaxRange;
}

// The 'setColormap' method
void ViewConfiguration::setColormap(Colormap const& colormap)
{
    m_colormap = colormap;
}

// The 'setTranslation' method
void ViewConfiguration::setTranslation(Vector3D const& translation)
{
    m_translation = translation;
}

// The 'setRotation' method
void ViewConfiguration::setRotation(Vector3D const& rotation)
{
    m_rotation = rotation;
}
