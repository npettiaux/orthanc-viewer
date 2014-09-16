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
//! \file SeriesVolumeViewer.cpp
//! \brief The SeriesVolumeViewer.cpp file contains the definition of non-inline
//!        methods of the SeriesVolumeViewer class.
//!
//! \author Quentin Smetz
//!

#include "SeriesVolumeViewer.h"
using namespace std;
using namespace customwidget;

// Constructor
SeriesVolumeViewer::SeriesVolumeViewer(SeriesData* series) : SeriesViewer(series), m_opacity(1.0)
{
    // Create the vtkProp3D (volume)
    vtkVolume* volume = vtkVolume::New();
    m_vtkProp3D = volume;

    // Add it to the renderer and setup interaction
    renderer()->AddViewProp(m_vtkProp3D);
    renderWindow()->GetInteractor()->SetInteractorStyle(vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New());

    // Custom properties
    m_opacityFunction.TakeReference(vtkPiecewiseFunction::New());
    m_colorFunction.TakeReference(vtkColorTransferFunction::New());
    m_rayCastFunction = 0;

    vtkSmartPointer<vtkVolumeProperty> property = vtkSmartPointer<vtkVolumeProperty>::New();
    property->SetScalarOpacity(m_opacityFunction);
    property->SetColor(m_colorFunction);
    property->DisableGradientOpacityOn();
    property->SetInterpolationTypeToLinear();
    volume->SetProperty(property);

    // Create the mapper
    m_mapper = vtkSmartPointer<vtkVolumeRayCastMapper>::New();
    m_mapper->SetInput(series);
    volume->SetMapper(m_mapper);

    // Update the properties according to current parameters
    enableMip(false);

    // Update view
    renderer()->ResetCamera();

    double* bound = m_vtkProp3D->GetBounds();
    m_vtkProp3D->SetOrigin((bound[1]-bound[0])/2, (bound[3]-bound[2])/2, (bound[5]-bound[4])/2);
}

// Destructor
SeriesVolumeViewer::~SeriesVolumeViewer()
{}

// The 'minSlice' method
double SeriesVolumeViewer::minSlice() const
{
    return 0;
}

// The 'maxSlice' method
double SeriesVolumeViewer::maxSlice() const
{
    return 0;
}

// The 'getPropOpacity' method
double SeriesVolumeViewer::getPropOpacity() const
{
    return m_opacity;
}

// The 'setPropOpacity' method
void SeriesVolumeViewer::setPropOpacity(double opacity)
{
    m_opacity = opacity;

    double tab1[4];
    m_opacityFunction->GetNodeValue(2, tab1);
    tab1[1] = m_opacity;
    double tab2[4];
    m_opacityFunction->GetNodeValue(3, tab2);
    tab2[1] = m_opacity;

    m_opacityFunction->SetNodeValue(2, tab1);
    m_opacityFunction->SetNodeValue(3, tab2);
}

// The 'enableMip' slot
void SeriesVolumeViewer::enableMip(bool enable)
{
    if(m_rayCastFunction != 0)
        m_rayCastFunction->Delete();

    if(enable)
    {
        m_rayCastFunction = vtkVolumeRayCastMIPFunction::New();
        static_cast<vtkVolumeRayCastMIPFunction*>(m_rayCastFunction)->SetMaximizeMethodToOpacity();
    }
    else
    {
        m_rayCastFunction = vtkVolumeRayCastCompositeFunction::New();
    }

    m_mapper->SetVolumeRayCastFunction(m_rayCastFunction);
    repaint();
}

// The 'updateHounsfield' method
void SeriesVolumeViewer::updateHounsfield(ViewConfiguration const& config)
{
    // Load hounsfield boundaries
    Range huRange = m_series->computeBasicHounsfieldRanges().at(0);

    // Compute the opacity function
    vtkSmartPointer<vtkPiecewiseFunction> func = vtkSmartPointer<vtkPiecewiseFunction>::New();
    func->AddPoint(m_series->convertFromHU(huRange.min()), 0.0);
    func->AddPoint(m_series->convertFromHU(config.hounsfield().min()), 0.0);
    func->AddPoint(m_series->convertFromHU(config.hounsfield().max()), m_opacity);
    func->AddPoint(m_series->convertFromHU(huRange.max()), m_opacity);
    m_opacityFunction->DeepCopy(func);

    // The colormap must change if hounsfield window changed
    updateColormap(config);
}

// The 'updateColormap' method
void SeriesVolumeViewer::updateColormap(ViewConfiguration const& config)
{
    // Load hounsfield boundaries
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
void SeriesVolumeViewer::updateTranslation(ViewConfiguration const& config)
{
    m_vtkProp3D->SetPosition(config.translation().x(),
                             config.translation().y(),
                             config.translation().z());
}

// The 'updateRotation' method
void SeriesVolumeViewer::updateRotation(ViewConfiguration const& config)
{
    m_vtkProp3D->SetOrientation(config.rotation().x(),
                                config.rotation().y(),
                                config.rotation().z());
}
