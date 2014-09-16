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
//! \file SubInterface.h
//! \brief The SubInterface.h file contains the interface of the
//!        SubInterface class and the definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef SUBINTERFACE_H
#define SUBINTERFACE_H

#include <iostream>

#include <QLayout>
#include <QMouseEvent>

#include "View/Qt/customwidget/Widget.h"

#include "View/VTK/Viewer.h"

//!
//! \brief The SubInterface class is a widget which encapsulate a SeriesViewer
//!        to be able to add external capabilities for it.
//!
class SubInterface : public customwidget::Widget
{
    Q_OBJECT

    public:
        //!
        //! \brief The SubInterface destructor.
        //!
        ~SubInterface();

        //!
        //! \brief The viewer method returns a pointer to the viewer the
        //!        SubInterface controls (constant version).
        //!
        //! The method is inline.
        //!
        //! \return A pointer to the viewer the SubInterface controls.
        //!
        inline Viewer const* viewer() const;

        //!
        //! \brief The viewer method returns a pointer to the viewer the
        //!        SubInterface controls.
        //!
        //! The method is inline.
        //!
        //! \return A pointer to the viewer the SubInterface controls.
        //!
        inline Viewer* viewer();

    signals:
        //!
        //! \brief The doubleClicked signal, once emitted, give access to the
        //!        object himself.
        //!
        //! \param subInterface A pointer to the object himself (which was
        //!                     double clicked.
        //!
        void doubleClicked(SubInterface* subInterface);

    protected:
        //!
        //! \brief The SubInterface constructor initialize its interface by
        //!        adding the viewer at the center of a QGridLayout.
        //!
        //! The SubInterface layout is a QGridLayout which can be modified by
        //! subclasses to show some other components than the viewer.
        //!
        SubInterface();

        //!
        //! \brief The setViewer method sets the viewer the SubInterface controls.
        //!
        //! \param viewer A pointer to the Viewer object the SubInterface controls.
        //!
        //! \return Nothing.
        //!
        void setViewer(Viewer* viewer);

        //!
        //! \brief The eventFilter method filters the event which are sent by
        //!        the viewer.
        //!
        //! This is a redefinition of the QObject::eventFilter method
        //!
        //! \param object A pointer to the object which sent the event.
        //! \param event A pointer to the event to object sent.
        //!
        //! \return A boolean which is true if the event must stop its
        //!         propagation after the call to this function.
        //!
        bool eventFilter(QObject* object, QEvent* event);

        //!
        //! \brief The mouseDoubleClickEvent signals that the subinterface has
        //!        been double clicked and stops the corresponding QMouseEvent.
        //!
        //! \section emit
        //! The doubleClicked(SubInterface*) signal is emitted.
        //!
        //! \param event The QMouseEvent which contains the double click
        //!              information.
        //!
        //! \return Nothing.
        //!
        virtual void mouseDoubleClickEvent(QMouseEvent* event);

        //! The m_gridLayout property give access for subclasses to the main
        //! layout of the interface. The SeriesViewer is placed in the cell (1, 1).
        QGridLayout* m_gridLayout;

        //! The m_viewer property give access for subclasses to the viewer
        //! which visualizes.
        Viewer* m_viewer;
};

// The 'viewer' method
inline Viewer const* SubInterface::viewer() const { return m_viewer; }

// The 'viewer' method
inline Viewer* SubInterface::viewer() { return m_viewer; }

#endif
