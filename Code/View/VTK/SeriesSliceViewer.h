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
//! \file SeriesSliceViewer.h
//! \brief The SeriesSliceViewer.h file contains the interface of the
//!        SeriesSliceViewer class and the definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef SLICESUBVIEWER_H
#define SLICESUBVIEWER_H

#include <iostream>
#include <set>

#include <QBoxLayout>

#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindow.h>
#include <vtkImageMapToRGBA.h>
#include <vtkColorTransferFunction.h>

#include <vtkImageActor.h>

#include "View/Qt/customwidget/Widget.h"

#include "SeriesViewer.h"
#include "main.h"

//!
//! \brief The SliceOrientation enum lists a series of constants which identify
//!        how to cut a volume to get a slice.
//!
enum SliceOrientation
{
    SAGITTAL, FRONTAL, TRANSVERSE
};

//!
//! \brief The SeriesSliceViewer class is a SeriesViewer which is specialized in
//!        visualizing a slice of a 3D volume.
//!
class SeriesSliceViewer : public SeriesViewer
{
    Q_OBJECT

    public:
        //!
        //! \brief The SeriesSliceViewer constructor initialize the render window
        //!        to visualize a slice of a given series.
        //!
        //! \param series The series to visualize.
        //! \param orientation The orientation of the slice.
        //!
        SeriesSliceViewer(SeriesData* series, SliceOrientation orientation);

        //!
        //! \brief The SeriesSliceViewer destructor.
        //!
        ~SeriesSliceViewer();

        //!
        //! \brief The minSlice method returns the minimum position of a slice
        //!        which can be shown in the internal orientation.
        //!
        //! This is an implementation of the SeriesViewer::minSlice() method.
        //!
        //! \return The minimum position of a slice which can be shown in the
        //!         internal orientation.
        //!
        double minSlice() const;

        //!
        //! \brief The maxSlice method returns the maximum position of a slice
        //!        which can be shown in the internal orientation.
        //!
        //! This is an implementation of the SeriesViewer::maxSlice() method.
        //!
        //! \return The maximum position of a slice which can be shown in the
        //!         internal orientation.
        //!
        double maxSlice() const;

        //!
        //! \brief The getPropOpacity method returns the opacity of the VTK
        //!        slice the viewer is showing.
        //!
        //! \return The opacity of the VTK slice the viewer is showing.
        //!
        double getPropOpacity() const;

        //!
        //! \brief The setPropOpacity method sets the opacity of the VTK slice
        //!        the viewer is showing.
        //!
        //! \param opacity The new opacity of the VTK slice the viewer is
        //!                showing.
        //!
        //! \return Nothing.
        //!
        void setPropOpacity(double opacity);

    public slots:
        //!
        //! \brief The changeCurrentSlice method update the slice which is
        //!        currently visualized.
        //!
        //! \return Nothing.
        //!
        void changeCurrentSlice(double value);

    protected:
        //!
        //! \brief The updateHounsfield method updates the viewer according to
        //!        the hounsfield window specified by the ViewConfiguration.
        //!
        //! This is an implementation of the SeriesViewer::updateHounsfield()
        //! method.
        //! The slice is drawn according to a colormap which is adapted with
        //! the hounsfield window. This applies that a call to updateHounsfield
        //! method is equal to a call to updateColormap method.
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
        //! Notice that it also refresh the slice which is visualized as the
        //! translation may have changed it.
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
        // The vtk mapper and its properties
        vtkImageMapToRGBA* m_vtkMapper;
        vtkSmartPointer<vtkColorTransferFunction> m_colorFunction;

        SliceOrientation m_orientation;

        Range m_sliceRange, m_sliceIndexRange;
        double m_sliceOffset;
        double m_currentSlice;
};

#endif
