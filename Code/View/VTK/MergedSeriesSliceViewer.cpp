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
//! \file MergedSeriesSliceViewer.cpp
//! \brief The MergedSeriesSliceViewer.cpp file contains the definition of
//!        non-inline methods of the MergedSeriesSliceViewer class.
//!
//! \author Quentin Smetz
//!

#include "MergedSeriesSliceViewer.h"
using namespace std;

// Constructor
MergedSeriesSliceViewer::MergedSeriesSliceViewer(SliceOrientation orientation)
    : MergedSeriesViewer(), m_orientation(orientation), m_currentSlice(0)
{
    renderWindow()->GetInteractor()->SetInteractorStyle(vtkSmartPointer<vtkInteractorStyleImage>::New());

    vtkCamera* camera = renderer()->GetActiveCamera();
    camera->ParallelProjectionOn();
    switch(orientation)
    {
        case SAGITTAL:
            camera->SetPosition(1, 0, 0);
            camera->SetFocalPoint(0, 0, 0);
            camera->SetViewUp(0, 0, 1);
            break;

        case FRONTAL:
            camera->SetPosition(0, -1, 0);
            camera->SetFocalPoint(0, 0, 0);
            camera->SetViewUp(0, 0, 1);
            break;

        case TRANSVERSE:
            camera->SetPosition(0, 0, -1);
            camera->SetFocalPoint(0, 0, 0);
            camera->SetViewUp(0, -1, 0);
            break;
    }
}

// Destructor
MergedSeriesSliceViewer::~MergedSeriesSliceViewer()
{}

// The 'changeCurrentSlice' slot
void MergedSeriesSliceViewer::changeCurrentSlice(double value)
{
    m_currentSlice = value;

    for(unsigned int i = 0 ; i < m_seriesViewers.size() ; i++)
    {
        SeriesSliceViewer* ssv = dynamic_cast<SeriesSliceViewer*>(m_seriesViewers.at(i));
        ssv->changeCurrentSlice(value);

        vtkImageActor* actor = dynamic_cast<vtkImageActor*>(ssv->getVtkProp3D());
        double* display = actor->GetDisplayBounds();
        double* position = actor->GetPosition();

        switch(m_orientation)
        {
            case SAGITTAL:
                actor->SetPosition(-display[0]+i, position[1], position[2]);
                break;

            case FRONTAL:
                actor->SetPosition(position[0], -display[2]-i, position[2]);
                break;

            case TRANSVERSE:
                actor->SetPosition(position[0], position[1], -display[4]-i);
                break;
        }
    }

    renderer()->ResetCameraClippingRange();
    repaint();
}

// The 'updateTranslation' method
void MergedSeriesSliceViewer::updateTranslation(ViewConfiguration const& config)
{
    changeCurrentSlice(m_currentSlice);
    repaint();
}
