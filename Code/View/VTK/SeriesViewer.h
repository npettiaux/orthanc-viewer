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
//! \file SeriesViewer.h
//! \brief The SeriesViewer.h file contains the interface of the
//!        SeriesViewer class and the definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef SUBVIEWER_H
#define SUBVIEWER_H

#include <iostream>
#include <vector>

#include <vtkProp.h>

#include "Viewer.h"
#include "Model/SeriesData.h"
#include "Model/Colormap.h"

//!
//! \brief The SeriesViewer class represents a specific widget to visualize a
//!        SeriesData object.
//!
//! The class is abstract. It permits to initialize some information about the
//! given series data and to trigger some useful event for subclasses. However,
//! the visualisation part is left for the subclasses.
//!
class SeriesViewer : public Viewer
{
    Q_OBJECT

    public:
        //!
        //! \brief The SeriesViewer destructor.
        //!
        ~SeriesViewer();

        //!
        //! \brief The getVtkProp3D method returns the VTK object which
        //!        represents the series.
        //!
        //! The method is inline.
        //!
        //! \return The VTK object which represents the series.
        //!
        inline vtkProp3D* getVtkProp3D() const;

        //!
        //! \brief The allowFusion method does all it is needed to prepare the
        //!        series viewer to join/leave a fusion.
        //!
        //! If the preparation is for joining, the method removes the visualisation
        //! actor from the vtkRenderer. It must be done to be available for the fusion.
        //! If it is to leave, re-adds the visualisation actor in the vtkRenderer
        //! and update the vtkCamera.
        //!
        //! \param allow A boolean which is true if the viewer must prepare
        //!              to join a fusion and false if the viewer must
        //!              prepare to leave a fusion.
        //!
        //! \return Nothing.
        //!
        void allowFusion(bool allow);

        //!
        //! \brief The getPropOpacity method returns the opacity of the VTK
        //!        prop which is shown by the viewer.
        //!
        //! The method is pure virtual, it must be redefined in subclasses.
        //!
        //! \return The opacity of the VTK prop which is shown by the viewer.
        //!
        virtual double getPropOpacity() const = 0;

        //!
        //! \brief The setPropOpacity method sets the opacity of the VTK prop
        //!        the viewer shows.
        //!
        //! The method is pure virtual, it must be redefined in subclasses.
        //!
        //! \param opacity The new opacity for the VTK prop the viewer shows.
        //!
        //! \return Nothing.
        //!
        virtual void setPropOpacity(double opacity) = 0;

    protected:
        //!
        //! \brief The SeriesViewer constructor initializes the SeriesViewer
        //!        parameters according to the given SeriesData.
        //!
        //! \param series The series data to visualize.
        //!
        SeriesViewer(SeriesData* series);

        //! The m_series property keep an access to the series data which is
        //! visualized by the viewer.
        SeriesData const* m_series;

        //! The VTK object that will represent the series in the viewer
        //! (volume / slice).
        vtkProp3D* m_vtkProp3D;
};

// The 'vtkProp3D' method
inline vtkProp3D* SeriesViewer::getVtkProp3D() const { return m_vtkProp3D; }

#endif
