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
//! \file ViewerWindow.h
//! \brief The ViewerWindow.h file contains the interface of the
//!        ViewerWindow class and the definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include <iostream>
#include <vector>

#include "View/Qt/customwidget/MainWindow.h"
#include "View/Qt/customwidget/Menu.h"
#include "View/Qt/customwidget/MenuBar.h"
#include "View/Qt/customwidget/StatusBar.h"
#include "View/Qt/customwidget/SplashScreen.h"
#include "View/Qt/customwidget/TabWidget.h"
#include "View/Qt/customwidget/ToolButton.h"

#include "Controller/FusionDialog.h"
#include "Controller/MergedSeriesInterface.h"
#include "Controller/OrthancDialog.h"
#include "Controller/SeriesInterface.h"

//!
//! \brief The ViewerWindow class is the main window of the Orthanc viewer
//!        application.
//!
//! This class cannot be publicly instanciated. Static methods provides access
//! to a singleton which must be freed if created.
//!
class ViewerWindow : public customwidget::MainWindow
{
    Q_OBJECT

    public:
        //!
        //! \brief The instance static method provides an access to the unique
        //!        viewer window.
        //!
        //! The first call to this method will initialize the window and its
        //! dedicated memory.
        //!
        //! \return A pointer to the unique ViewerWindow object.
        //!
        static ViewerWindow* instance();

        //!
        //! \brief The free static method will free the memory of the unique
        //!        viewer window if it was created and permits to create a new
        //!        one later if necessary.
        //!
        //! \return Nothing.
        //!
        static void free();

        //!
        //! \brief The ViewerWindow destructor.
        //!
        ~ViewerWindow();

        //!
        //! \brief The signalOrthancProblemAndExit method shows a MessageBox
        //!        which tells to the user the Orthanc server is not activated
        //!        and then launch a timer of one second before closing the app.
        //!
        //! \return Nothing.
        //!
        void signalOrthancProblemAndExit();

        //!
        //! \brief The attachInterface method attaches the given display
        //!        interface in the dedicated tab widget and select it as the
        //!        current one.
        //!
        //! \param interface A pointer to the display interface to attach.
        //!
        //! \return Nothing.
        //!
        void attachInterface(DisplayInterface* interface);

    public slots:
        //!
        //! \brief The createNewSeries slot creates a new series interface
        //!        for the given series data and add it to the tabwidget.
        //!
        //! \param series The series to visualize in the new series interface.
        //!
        //! \return Nothing.
        //!
        void createNewSeries(SeriesData* series);

        //!
        //! \brief The createNewMergedSeries slot creates a new merged series
        //!        interface (empty fusion) with the specified title.
        //!
        //! \param title A QString object which contains the title for the
        //!              fusion interface.
        //!
        //! \return Nothing.
        //!
        void createNewMergedSeries(QString const& title);

        //!
        //! \brief The closeInterface slot closes the interface (and frees
        //!        its memory) identified by its index in the tabwidget.
        //!
        //! \param index The index of the interface in the tabwidget
        //!              to close and free.
        //!
        //! \return Nothing.
        //!
        void closeInterface(int index);

        //!
        //! \brief The closeInterface slot closes the interface given in
        //!        parameter (and frees its memory).
        //!
        //! \param interface The interface to close and free.
        //!
        //! \return Nothing.
        //!
        void closeInterface(DisplayInterface* interface);

        //!
        //! \brief The changeStatusBarMessage slot changes the message which is
        //!        shown in the window status bar.
        //!
        //! \param message A QString object which contains the message to be
        //!                shown in the status bar.
        //! \param time The amount of milliseconds the message must stay (or 0
        //!             if it must stay forever).
        //!
        //! \return Nothing.
        //!
        void changeStatusBarMessage(QString const& message, unsigned int time = 3000);

        //!
        //! \brief The clearStatusBarMessage slot removes any message which is
        //!        show in the window status bar.
        //!
        //! \return Nothing.
        //!
        void clearStatusBarMessage();

        //!
        //! \brief The showFusionDialog slot shows the fusion dialog and ask it
        //!        to create a fusion if none has already been created.
        //!
        //! \return Nothing.
        //!
        void showFusionDialog();

    private:
        static ViewerWindow* s_viewerWindow; // The singleton (unique viewer window)

        //!
        //! \brief The ViewerWindow constructor initializes the window, related
        //!        dialogs and user interactions.
        //!
        //! \param parent A pointer to the parent widget (default: no parent).
        //!
        ViewerWindow(QWidget* parent = 0);

        // Actions list
        QAction* m_openOrthancAction;
        QAction* m_newMergedSeriesAction;
        QAction* m_quitAction;
        QTimer m_closeTimer;

        // Menus list
        customwidget::MenuBar* m_menuBar;
        customwidget::Menu* m_fileMenu;

        // Interface
        customwidget::TabWidget* m_tabbedSeries;
        std::vector<DisplayInterface*> m_interfacesList;    // The opened interfaces

        // Status bar
        customwidget::StatusBar* m_statusBar;

        // Dialogs
        OrthancDialog* m_orthancDialog;                     // Orthanc loader dialog
        FusionDialog* m_fusionDialog;
};

#endif
