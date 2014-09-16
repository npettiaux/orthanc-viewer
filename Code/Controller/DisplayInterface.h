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
//! \file DisplayInterface.h
//! \brief The DisplayInterface.h file contains the interface of the
//!        DisplayInterface class and the definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef DISPLAYINTERFACE_H
#define DISPLAYINTERFACE_H

#include <iostream>

#include <QAction>
#include <QActionGroup>

#include "View/Qt/customwidget/Widget.h"
#include "View/Qt/customwidget/ToolBar.h"
#include "View/Qt/customwidget/Splitter.h"

#include "Controller/SubInterface.h"

//!
//! \brief The DisplayInterface class is a widget which contains all the
//!        components to ensure and custom the visualization of a volume and
//!        three slices.
//!
class DisplayInterface : public customwidget::Widget
{
    Q_OBJECT

    public:
        //!
        //! \brief The DisplayInterface destructor.
        //!
        ~DisplayInterface();

        //!
        //! \brief The LayoutStyle enum regroups a series of constants to
        //!        distinguish the layout style for organizing the viewers.
        //!
        enum LayoutStyle
        {
            MODE1, MODE2
        };

        //!
        //! \brief The viewer method returns the Viewer object associated with
        //!        the SubInterface object of specified index (constant version).
        //!
        //! The method is inline.
        //!
        //! \param index The index of the SubInterface whose viewer is returned.
        //!
        //! \return A pointer to the Viewer object associated with the
        //!         SubInterface object of specified index.
        //!
        inline Viewer const* viewer(unsigned int index) const;

        //!
        //! \brief The viewer method returns the Viewer object associated with
        //!        the SubInterface object of specified index.
        //!
        //! The method is inline.
        //!
        //! \param index The index of the SubInterface whose viewer is returned.
        //!
        //! \return A pointer to the Viewer object associated with the
        //!         SubInterface object of specified index.
        //!
        inline Viewer* viewer(unsigned int index);

        //!
        //! \brief The title method returns the title's interface, which should
        //!        be used to name it.
        //!
        //! \return A reference to the constant string which contains the
        //!         title's interface.
        //!
        inline QString const& title() const;

    public slots:
        //!
        //! \brief The setLayoutStyle slot apply the given layout style for the
        //!        visualization.
        //!
        //! \param layoutStyle The layout style to apply (identified by its
        //!                    corresponding constant).
        //!
        //! \return Nothing.
        //!
        void setLayoutStyle(LayoutStyle layoutStyle);

        //!
        //! \brief The setSelectedLayoutStyle slot checks the toolbar to know
        //!        which layout style is activated and then apply it.
        //!
        //! The style is applied through a call to the other setLayoutStyle
        //! slot.
        //!
        //! \return Nothing.
        //!
        void setSelectedLayoutStyle();

        //!
        //! \brief The setSubInterfaceMaximized slot maximizes the given
        //!        subinterface and hide the others. If a subinterface is
        //!        already maximized, then all of them are minimized.
        //!
        //! Notice that the slot is set to work with a subinterface which is
        //! owned by the current series interface.
        //!
        //! \param subInterface A pointer to the subinterface to maximize if
        //!                     possible.
        //!
        //! \return Nothing.
        //!
        void setSubInterfaceMaximized(SubInterface* subInterface);

        //!
        //! \brief The updateViewConfiguration slot updates the view
        //!        configuration of each Viewer object the interface contains.
        //!
        //! \param config The new view configuration.
        //! \param param The view configuration parameter that need to be changed.
        //!
        //! \return Nothing.
        //!
        void updateViewConfiguration(ViewConfiguration const& config, ViewConfiguration::ViewParam param);

        //!
        //! \brief The setSelectedWindowMode slot updates the window mode of the
        //!        interface (attached window or not) according to the current
        //!        selection.
        //!
        //! \return Nothing.
        //!
        void setSelectedWindowMode();

        //!
        //! \brief The showAxes slot shows or hides the axes in the viewers.
        //!
        //! \param show A boolean which is true if the axes must be showed and
        //!             false if they must be hidden.
        //!
        //! \return Nothing.
        //!
        void showAxes(bool show = true);

    signals:
        //!
        //! \brief The closed signal, once emitted, give access to the
        //!        DisplayInterface which was closed.
        //!
        //! \param interface A pointer to the DisplayInterface which was
        //!                  closed.
        //!
        void closed(DisplayInterface* interface);

        //!
        //! \brief The viewConfigurationChanged signal, once emitted, indicates
        //!        the new view configuration and the parameter that changed.
        //!
        //! \param config The new view configuration.
        //! \param param The view parameter that changed.
        //!
        void viewConfigurationChanged(ViewConfiguration const& config, ViewConfiguration::ViewParam param);

    protected:
        //!
        //! \brief The DisplayInterface constructor initializes the interface
        //!        parts and arrange them in the main layout.
        //!
        //! \param parent The widget parent of the interface.
        //!
        DisplayInterface(QWidget* parent = 0);

        //!
        //! \brief The closeEvent method process a QCloseEvent and so close the
        //!        window.
        //!
        //! This is a redefinition.
        //!
        //! \section emit
        //! The method emits a closed signal.
        //!
        //! \param event A pointer to the QCloseEvent to process.
        //!
        //! \return Nothing.
        //!
        void closeEvent(QCloseEvent* event);

        //!
        //! \brief The setToolBar method adds a toolbar to the interface and
        //!        display it above the viewers.
        //!
        //! \param toolBar The toolbar to add on the interface.
        //!
        //! \return Nothing.
        //!
        void setToolBar(QToolBar* toolBar);

        //! Identifies the volume viewer.
        static unsigned int const VOLUME = 0;
        //! Identifies the sagittal slice viewer.
        static unsigned int const SAGITTAL_SLICE = 1;
        //! Identifies the frontal slice viewer.
        static unsigned int const FRONTAL_SLICE = 2;
        //! Identifies the transverse slice viewer.
        static unsigned int const TRANSVERSE_SLICE = 3;

        //! The interface parts which will contain the viewers.
        SubInterface* m_subInterface[4];
        //! The title of the interface.
        QString m_title;

        //! The toolbar of the interface.
        customwidget::ToolBar* m_toolBar;

    private:
        QActionGroup* m_layoutActionGroup;
        QAction* m_toWindowAction;
        QAction* m_fullScreenAction;
        QAction* m_axesAction;

        // Interface components
        customwidget::Splitter* m_horizontalSplitter;
        customwidget::Splitter *m_leftSplitter, *m_rightSplitter;
};

// The 'viewer' method
inline Viewer const* DisplayInterface::viewer(unsigned int index) const
{ return m_subInterface[index]->viewer(); }

// The 'viewer' method
inline Viewer* DisplayInterface::viewer(unsigned int index)
{ return m_subInterface[index]->viewer(); }

// The 'title' method
inline QString const& DisplayInterface::title() const { return m_title; }

#endif
