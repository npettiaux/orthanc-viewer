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
//! \file SeriesSliceViewer.cpp
//! \brief The SeriesSliceViewer.cpp file contains the definition of non-inline
//!        methods of the SeriesSliceViewer class.
//!
//! \author Quentin Smetz
//!

#include "SeriesSliceViewer.h"
using namespace std;
using namespace customwidget;

// Constructor
SeriesSliceViewer::SeriesSliceViewer(SeriesData* series, SliceOrientation orientation)
    : SeriesViewer(series), m_orientation(orientation), m_sliceOffset(0), m_currentSlice(0)
{
    // Create the vtkProp3D (ImageActor)
    vtkImageActor* imageActor = vtkImageActor::New();
    m_vtkProp3D = imageActor;

    // Add it to the renderer and setup interaction
    renderer()->AddViewProp(m_vtkProp3D);
    renderWindow()->GetInteractor()->SetInteractorStyle(vtkSmartPointer<vtkInteractorStyleImage>::New());

    // Custom properties
    m_colorFunction.TakeReference(vtkColorTransferFunction::New());

    // Create the mapper
    m_vtkMapper = vtkImageMapToRGBA::New();
    m_vtkMapper->SetOutputFormatToRGBA();
    m_vtkMapper->PassAlphaToOutputOn();
    m_vtkMapper->SetInput(series);
    m_vtkMapper->SetLookupTable(m_colorFunction);
    imageActor->SetInput(m_vtkMapper->GetOutput());

    // Update camera
    int* ext = series->GetExtent();
    vtkCamera* camera = renderer()->GetActiveCamera();
    camera->ParallelProjectionOn();
    double* bound = series->GetBounds();
    m_vtkProp3D->SetOrigin((bound[1]-bound[0])/2, (bound[3]-bound[2])/2, (bound[5]-bound[4])/2);
    switch(orientation)
    {
        case SAGITTAL:
            camera->SetPosition(1, 0, 0);
            camera->SetFocalPoint(0, 0, 0);
            camera->SetViewUp(0, 0, 1);
            imageActor->SetDisplayExtent(0, 0, ext[2], ext[3], ext[4], ext[5]);
            m_sliceRange.min() = bound[0];
            m_sliceRange.max() = bound[1];
            break;

        case FRONTAL:
            camera->SetPosition(0, -1, 0);
            camera->SetFocalPoint(0, 0, 0);
            camera->SetViewUp(0, 0, 1);
            imageActor->SetDisplayExtent(ext[0], ext[1], 0, 0, ext[4], ext[5]);
            m_sliceRange.min() = bound[2];
            m_sliceRange.max() = bound[3];
            break;

        case TRANSVERSE:
            camera->SetPosition(0, 0, -1);
            camera->SetFocalPoint(0, 0, 0);
            camera->SetViewUp(0, -1, 0);
            imageActor->SetDisplayExtent(ext[0], ext[1], ext[2], ext[3], 0, 0);
            m_sliceRange.min() = bound[4];
            m_sliceRange.max() = bound[5];
            break;
    }
    renderer()->ResetCamera();

    m_sliceIndexRange.min() = imageActor->GetSliceNumberMin();
    m_sliceIndexRange.max() = imageActor->GetSliceNumberMax();
}

// Destructor
SeriesSliceViewer::~SeriesSliceViewer()
{}

// The 'minSlice' method
double SeriesSliceViewer::minSlice() const
{
    return m_sliceRange.min();
}

// The 'maxSlice' method
double SeriesSliceViewer::maxSlice() const
{
    return m_sliceRange.max();
}

// The 'getPropOpacity' method
double SeriesSliceViewer::getPropOpacity() const
{
    return dynamic_cast<vtkImageActor*>(m_vtkProp3D)->GetOpacity();
}

// The 'setPropOpacity' method
void SeriesSliceViewer::setPropOpacity(double opacity)
{
    dynamic_cast<vtkImageActor*>(m_vtkProp3D)->SetOpacity(opacity);
}

// The 'changeCurrentSlice' slot
void SeriesSliceViewer::changeCurrentSlice(double value)
{
    m_currentSlice = value;
    value -= m_sliceOffset; // TODO maybe update slider ranges in subinterface could be useful (but not required)

    vtkImageActor* actor = dynamic_cast<vtkImageActor*>(m_vtkProp3D);
    int* ext = actor->GetDisplayExtent();

    if(value < minSlice() || value > maxSlice())
        actor->VisibilityOff();
    else
    {
        int slice = floor(0.5+m_sliceIndexRange.absolute(m_sliceRange.relative(value)));
        actor->VisibilityOn();
        switch(m_orientation)
        {
            case SAGITTAL:
                actor->SetDisplayExtent(slice, slice, ext[2], ext[3], ext[4], ext[5]);
                break;

            case FRONTAL:
                actor->SetDisplayExtent(ext[0], ext[1], slice, slice, ext[4], ext[5]);
                break;

            case TRANSVERSE:
                actor->SetDisplayExtent(ext[0], ext[1], ext[2], ext[3], slice, slice);
                break;
        }
    }

    renderer()->ResetCameraClippingRange();
    repaint();
}

// The 'updateHounsfield' method
void SeriesSliceViewer::updateHounsfield(ViewConfiguration const& config)
{
    updateColormap(config); // Hounsfield is controled by the place where the colormap is applied
}

// The 'updateColormap' method
void SeriesSliceViewer::updateColormap(ViewConfiguration const& config)
{
    // Add hounsfield boundaries in a custom colormap
    Range maxRange = config.hounsfieldMaxRange();
    Range huRange(m_series->convertFromHU(maxRange.min()),
                  m_series->convertFromHU(maxRange.max()));

    // Compute color transfert function
    Range hu(m_series->convertFromHU(config.hounsfield().min()),
             m_series->convertFromHU(config.hounsfield().max()));
    vtkSmartPointer<vtkColorTransferFunction> func;
    func.TakeReference(config.colormap().computeVTKColorTransferFunction(huRange, hu));
    m_colorFunction->DeepCopy(func);
}

// The 'updateTranslation' method
void SeriesSliceViewer::updateTranslation(ViewConfiguration const& config)
{
    double* position = m_vtkProp3D->GetPosition();

    switch(m_orientation)
    {
        case SAGITTAL:
            m_sliceOffset = config.translation().x();
            m_vtkProp3D->SetPosition(position[0], config.translation().y(), config.translation().z());
            break;

        case FRONTAL:
            m_sliceOffset = config.translation().y();
            m_vtkProp3D->SetPosition(config.translation().x(), position[1], config.translation().z());
            break;

        case TRANSVERSE:
            m_sliceOffset = config.translation().z();
            m_vtkProp3D->SetPosition(config.translation().x(), config.translation().y(), position[2]);
            break;
    }

    changeCurrentSlice(m_currentSlice);
}

// The 'updateRotation' method
void SeriesSliceViewer::updateRotation(ViewConfiguration const& config)
{
    switch(m_orientation)
    {
        case SAGITTAL:
            m_vtkProp3D->SetOrientation(config.rotation().x(), 0, 0);
            break;

        case FRONTAL:
            m_vtkProp3D->SetOrientation(0, config.rotation().y(), 0);
            break;

        case TRANSVERSE:
            m_vtkProp3D->SetOrientation(0, 0, config.rotation().z());
            break;
    }
}
