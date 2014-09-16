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
//! \file Viewer.h
//! \brief The Viewer.h file contains the interface of the
//!        Viewer class and the definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef VIEWER_H
#define VIEWER_H

#include <iostream>

#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkOpenGLRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkRenderWindowInteractor.h>

#include "View/Qt/customwidget/VTKWidget.h"

#include "Model/ViewConfiguration.h"

//!
//! \brief The Viewer class represents a specific widget to visualize
//!        customizable object with the ViewConfiguration class.
//!
class Viewer : public customwidget::VTKWidget
{
    Q_OBJECT

    public:
        //!
        //! \brief The Viewer destructor.
        //!
        ~Viewer();

        //!
        //! \brief The minSlice method must return the minimum slice position of the
        //!        series 3D volume which can accessed given the current
        //!        configuration.
        //!
        //! The method is pure virtual, it must be redefined in subclasses.
        //!
        //! \return The minimum position of a slice of the series 3D volume which
        //!         can be accessed given the current configuration.
        //!
        virtual double minSlice() const = 0;

        //!
        //! \brief The maxSlice method must return the maximum slice position of the
        //!        series 3D volume which can accessed given the current
        //!        configuration.
        //!
        //! The method is pure virtual, it must be redefined in subclasses.
        //!
        //! \return The maximum position of a slice of the series 3D volume which
        //!         can be accessed given the current configuration.
        //!
        virtual double maxSlice() const = 0;

    public slots:
        //!
        //! \brief The updateView slot calls the specific routines according to
        //!        view parameter that need to be modified and use the view
        //!        configuration to do it.
        //!
        //! \param config The view configuration parameters.
        //! \param param A constant which identifies the view parameter type to
        //!              be modified.
        //!
        //! \return Nothing.
        //!
        void updateView(ViewConfiguration const& config, ViewConfiguration::ViewParam param);

        //!
        //! \brief The showAxes slot shows or hides the axes in the viewer.
        //!
        //! \param show A boolean which is true if the axes must be showed and
        //!             false if they must be hidden.
        //!
        //! \return Nothing.
        //!
        void showAxes(bool show = true);

    protected:
        //!
        //! \brief The Viewer constructor initializes render window and renderer.
        //!
        Viewer();

        //!
        //! \brief The updateHounsfield method must update the viewer so that
        //!        it takes into account the hounsfield window specified by
        //!        the ViewConfiguration.
        //!
        //! The method is pure virtual, it must be redefined in subclasses.
        //!
        //! \param config The ViewConfiguration object to work on.
        //!
        //! \return Nothing.
        //!
        virtual void updateHounsfield(ViewConfiguration const& config) = 0;

        //!
        //! \brief The updateColormap method must update the viewer so that
        //!        it takes into account the colormap specified by
        //!        the ViewConfiguration.
        //!
        //! The method is pure virtual, it must be redefined in subclasses.
        //!
        //! \param config The ViewConfiguration object to work on.
        //!
        //! \return Nothing.
        //!
        virtual void updateColormap(ViewConfiguration const& config) = 0;

        //!
        //! \brief The updateTranslation method must update the viewer so that
        //!        it takes into account the translation specified by
        //!        the ViewConfiguration.
        //!
        //! The method is pure virtual, it must be redefined in subclasses.
        //!
        //! \param config The ViewConfiguration object to work on.
        //!
        //! \return Nothing.
        //!
        virtual void updateTranslation(ViewConfiguration const& config) = 0;

        //!
        //! \brief The updateRotation method must update the viewer so that
        //!        it takes into account the rotation specified by
        //!        the ViewConfiguration.
        //!
        //! The method is pure virtual, it must be redefined in subclasses.
        //!
        //! \param config The ViewConfiguration object to work on.
        //!
        //! \return Nothing.
        //!
        virtual void updateRotation(ViewConfiguration const& config) = 0;

        //!
        //! \brief The renderWindow method returns the render window the Viewer
        //!        is using.
        //!
        //! The method is inline.
        //!
        //! \return A pointer to the render window the Viewer is using.
        //!
        inline vtkRenderWindow* renderWindow() const;

        //!
        //! \brief The renderer method returns the renderer the render window
        //!        is using.
        //!
        //! The method is inline.
        //!
        //! \return A pointer to the renderer the Viewer is using.
        //!
        inline vtkRenderer* renderer() const;

    private:
        //! The vtk object which contains the window for visualization.
        vtkRenderWindow* m_renderWindow;
        vtkRenderer* m_renderer;

        vtkSmartPointer<vtkAxesActor> m_axes;
};

// The 'renderWindow' method
inline vtkRenderWindow* Viewer::renderWindow() const { return m_renderWindow; }

// The 'renderer' method
inline vtkRenderer* Viewer::renderer() const { return m_renderer; }

#endif
