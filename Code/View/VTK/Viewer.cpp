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
//! \file Viewer.cpp
//! \brief The Viewer.cpp file contains the definition of non-inline methods
//!        of the Viewer class.
//!
//! \author Quentin Smetz
//!

#include "Viewer.h"
using namespace std;
using namespace customwidget;

// Constructor
Viewer::Viewer() : VTKWidget()
{
    m_renderWindow = GetRenderWindow();
    m_renderer = vtkOpenGLRenderer::New();
    m_renderer->SetBackground(0, 0, 0);
    m_renderWindow->AddRenderer(m_renderer);

    vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
    transform->Translate(0.0, 0.0, 0.0);
    transform->Scale(100, 100, 100);
    m_axes = vtkSmartPointer<vtkAxesActor>::New();
    m_axes->SetUserTransform(transform);
    m_renderer->AddActor(m_axes);

    showAxes(false);

    m_renderer->ResetCamera();
}

// Destructor
Viewer::~Viewer()
{}

// The 'updateView' slot
void Viewer::updateView(ViewConfiguration const& config, ViewConfiguration::ViewParam param)
{
    // Check the param to update and call the corresponding function
    switch(param)
    {
        case ViewConfiguration::ALL:
            updateHounsfield(config);
            updateColormap(config);
            updateTranslation(config);
            updateRotation(config);
            break;

        case ViewConfiguration::HOUNSFIELD:
            updateHounsfield(config);
            break;

        case ViewConfiguration::COLORMAP:
            updateColormap(config);
            break;

        case ViewConfiguration::TRANSLATION:
            updateTranslation(config);
            break;

        case ViewConfiguration::ROTATION:
            updateRotation(config);
            break;
    }

    // Repaint the viewer to really apply the update
    repaint();
}

// The 'showAxes' method
void Viewer::showAxes(bool show)
{
    m_axes->SetVisibility(show);
    m_renderer->ResetCameraClippingRange();
    repaint();
}
