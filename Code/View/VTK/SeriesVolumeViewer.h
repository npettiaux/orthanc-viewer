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
//! \file SeriesVolumeViewer.h
//! \brief The SeriesVolumeViewer.h file contains the interface of the
//!        SeriesVolumeViewer class and the definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef VOLUMESUBVIEWER_H
#define VOLUMESUBVIEWER_H

#include <QBoxLayout>

#include <vtkSmartPointer.h>
#include <vtkVolume.h>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkOpenGLRenderer.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeRayCastFunction.h>
#include <vtkVolumeRayCastCompositeFunction.h>
#include <vtkVolumeRayCastIsosurfaceFunction.h>
#include <vtkVolumeRayCastMIPFunction.h>

#include <vtkVolumeProperty.h>
#include <vtkVolumeRayCastMapper.h>

#include <vtkRendererCollection.h>
#include <vtkPropCollection.h>

#include "View/Qt/customwidget/Widget.h"

#include "SeriesViewer.h"
#include "main.h"

//!
//! \brief The SeriesVolumeViewer class is a SeriesViewer which is specialized in
//!        visualizing a 3D volume.
//!
class SeriesVolumeViewer : public SeriesViewer
{
    Q_OBJECT

    public:
        //!
        //! \brief The SeriesVolumeViewer constructor initialize the render window
        //!        to visualize a 3D volume of a given series.
        //!
        //! \param series The series to visualize.
        //!
        SeriesVolumeViewer(SeriesData* series);

        //!
        //! \brief The SeriesVolumeViewer destructor.
        //!
        ~SeriesVolumeViewer();

        //!
        //! \brief The minSlice method returns 0.
        //!
        //! This method exists for possible evolutions.
        //!
        //! This is an implementation of the SeriesViewer::minSlice() method.
        //!
        //! \return 0.
        //!
        double minSlice() const;

        //!
        //! \brief The maxSlice method returns 0.
        //!
        //! This method exists for possible evolutions.
        //!
        //! This is an implementation of the SeriesViewer::maxSlice() method.
        //!
        //! \return 0.
        //!
        double maxSlice() const;

        //!
        //! \brief The getPropOpacity method returns the maximum opacity which
        //!        is used to build the VTK volume the viewer shows.
        //!
        //! \return The maximum opacity which is used to build the VTK volume
        //!         the viewer shows.
        //!
        double getPropOpacity() const;

        //!
        //! \brief The setPropOpacity method sets the maximum opacity which
        //!        is used to build the VTK volume the viewer shows.
        //!
        //! \param opacity The new maximum opacity which is used to build the
        //!                VTK volume the viewer shows.
        //!
        //! \return Nothing.
        //!
        void setPropOpacity(double opacity);

    public slots:
        //!
        //! \brief The enableMip slot enable or disable the maximum intensity
        //!        projection mode of the volume rendering.
        //!
        //! \param enable A boolean which is true to enable and false to
        //!               disable the MIP.
        //!
        //! \return Nothing.
        //!
        void enableMip(bool enable);

    protected:
        //!
        //! \brief The updateHounsfield method updates the viewer according to
        //!        the hounsfield range specified by the ViewConfiguration.
        //!
        //! This is an implementation of the SeriesViewer::updateHounsfield()
        //! method.
        //! Notice that a method implies also an internal call to the
        //! updateColormap() method as hounsfield range and colormap are linked.
        //!
        //! \param config The ViewConfiguration object to work on.
        //!
        //! \return Nothing.
        //!
        void updateHounsfield(ViewConfiguration const& config);

        //!
        //! \brief The updateColormap method updates the viewer according to
        //!        the colormap specified by the ViewConfiguration.
        //!
        //! This is an implementation of the SeriesViewer::updateColormap()
        //! method.
        //!
        //! \param config The ViewConfiguration object to work on.
        //!
        //! \return Nothing.
        //!
        void updateColormap(ViewConfiguration const& config);

        //!
        //! \brief The updateTranslation method updates the viewer according to
        //!        the translation specified by the ViewConfiguration.
        //!
        //! This is an implementation of the SeriesViewer::updateTranslation()
        //! method.
        //!
        //! \param config The ViewConfiguration object to work on.
        //!
        //! \return Nothing.
        //!
        void updateTranslation(ViewConfiguration const& config);

        //!
        //! \brief The updateRotation method updates the viewer according to
        //!        the rotation specified by the ViewConfiguration.
        //!
        //! This is an implementation of the SeriesViewer::updateRotation()
        //! method.
        //!
        //! \param config The ViewConfiguration object to work on.
        //!
        //! \return Nothing.
        //!
        void updateRotation(ViewConfiguration const& config);

    private:
        vtkSmartPointer<vtkVolumeRayCastMapper> m_mapper;

        vtkSmartPointer<vtkPiecewiseFunction> m_opacityFunction;
        vtkSmartPointer<vtkColorTransferFunction> m_colorFunction;
        vtkVolumeRayCastFunction* m_rayCastFunction;

        double m_opacity;
};

#endif
