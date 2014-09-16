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
//! \file SeriesInterface.h
//! \brief The SeriesInterface.h file contains the interface of the
//!        SeriesInterface class and the definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef SERIESINTERFACE_H
#define SERIESINTERFACE_H

#include <QGridLayout>
#include <QButtonGroup>

#include "Model/SeriesData.h"
#include "Model/ViewConfiguration.h"

#include "View/Qt/HounsfieldColormapDialog.h"
#include "View/Qt/TranslationRotationDialog.h"

#include "Controller/DisplayInterface.h"
#include "Controller/SliceSubInterface.h"
#include "Controller/VolumeSubInterface.h"

//!
//! \brief The SeriesInterface class is a widget which contains all the
//!        components to ensure and custom the visualization of a DICOM
//!        series.
//!
class SeriesInterface : public DisplayInterface
{
    Q_OBJECT

    public:
        //!
        //! \brief The SeriesInterface constructor initializes the viewers and
        //!        components and event connections.
        //!
        //! \param series A pointer to the series data the interface have to
        //!               visualize.
        //! \param parent The QWidget parent of the interface.
        //!
        SeriesInterface(SeriesData* series, QWidget* parent = 0);

        //!
        //! \brief The SeriesInterface destructor.
        //!
        ~SeriesInterface();

        //!
        //! \brief The series method returns a reference to read the SeriesData
        //!        the interface visualizes.
        //!
        //! The method is inline.
        //!
        //! \return A reference tor read the SeriesData the interface
        //!         visualizes.
        //!
        inline SeriesData const& series() const;

        //!
        //! \brief The allowFusion method does all it is needed to prepare the
        //!        series interface to join/leave a fusion.
        //!
        //! If the preparation is for joining, the method hide the interface. If
        //! it is to leave, the interface becomes visible.
        //! The method also prepares the viewers the interface contains.
        //!
        //! \param allow A boolean which is true if the interface must prepare
        //!              to join a fusion and false if the interface must
        //!              prepare to leave a fusion.
        //!
        //! \return Nothing.
        //!
        void allowFusion(bool allow);

        //!
        //! \brief The getCustomActions method returns the custom actions the
        //!        series interface provides to the user.
        //!
        //! \return A QList object which contains the custom actions the
        //!         interface provides to the user.
        //!
        QList<QAction*> getCustomActions() const;

        //!
        //! \brief The getPropsOpacity method returns the opacity of the vtkProp
        //!        the first viewer shows (assuming all other viewer are using
        //!        the same opacity).
        //!
        //! \return The opacity of the vtkProp the first viewer shows (assuming
        //!         all other viewer are using the same opacity).
        //!
        double getPropsOpacity() const;

        //!
        //! \brief The setPropsOpacity method sets the opacity of each vtkProp
        //!        the viewers show.
        //!
        //! \param opacity The new opacity for the vtk actors the viewers show.
        //!
        //! \return Nothing.
        //!
        void setPropsOpacity(double opacity);

    private:
        vtkSmartPointer<SeriesData> m_series; // The series the interface visualizes

        // Toolbar components
        QAction* m_hounsfieldColormapAction;
        QAction* m_translationRotationAction;

        // Dialog tools
        HounsfieldColormapDialog* m_hounsfieldColormapDialog;
        TranslationRotationDialog* m_translationRotationDialog;
};

// The 'series' method
inline SeriesData const& SeriesInterface::series() const
{ return *(m_series.GetPointer()); }

#endif
