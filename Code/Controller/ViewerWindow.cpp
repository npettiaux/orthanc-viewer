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
//! \file ViewerWindow.cpp
//! \brief The ViewerWindow.cpp file contains the definition of non-inline
//!        methods of the ViewerWindow class.
//!
//! \author Quentin Smetz
//!

#include "ViewerWindow.h"
#include "Model/ProgramConfiguration.h"
using namespace std;
using namespace customwidget;

// Initialize the singleton to null
ViewerWindow* ViewerWindow::s_viewerWindow = 0;

// The 'viewerWindow' static method
ViewerWindow* ViewerWindow::instance()
{
    if(s_viewerWindow == 0)
        s_viewerWindow = new ViewerWindow();

    return s_viewerWindow;
}

// The 'free' static method
void ViewerWindow::free()
{
    if(s_viewerWindow != 0)
    {
        delete s_viewerWindow;
        s_viewerWindow = 0;
    }
}

// Constructor
ViewerWindow::ViewerWindow(QWidget* parent) : MainWindow(parent)
{
    QString imgDir(ProgramConfiguration::instance()->imageDirectory().c_str());
    QPixmap orthancPix(imgDir + "/orthanc.png");
    SplashScreen* splash = new SplashScreen(this, orthancPix.scaledToWidth(300));
    splash->show();
    splash->finish(this);

    cout << "Building viewer window... " << flush;

    // Build the window
    setMinimumSize(QSize(800, 600));
    setWindowTitle("Orthanc Viewer");
    setWindowIcon(QIcon(imgDir + "/orthanc_icon.png"));
    setIconSize(QSize(50, 50));

    // Create the actions
    m_openOrthancAction = new QAction("Ouvrir depuis Orthanc...", this);
    m_newMergedSeriesAction = new QAction("Fusionner...", this);
    m_quitAction = new QAction("Quitter", this);

    // Create the menus
    m_menuBar = new MenuBar();

    m_fileMenu = new Menu("&Fichier");
    m_fileMenu->addAction(m_openOrthancAction);
    m_fileMenu->addAction(m_newMergedSeriesAction);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_quitAction);
    m_menuBar->addMenu(m_fileMenu);

    setMenuBar(m_menuBar);

    // Create the interface
    QVBoxLayout* vLayout = new QVBoxLayout();

    m_tabbedSeries = new TabWidget();
    m_tabbedSeries->setTabsClosable(true);
    m_tabbedSeries->setTabPosition(QTabWidget::West);
    m_tabbedSeries->setMovable(true);
    m_tabbedSeries->setUsesScrollButtons(true);
    vLayout->addWidget(m_tabbedSeries);

    QWidget* cWidget = new QWidget();
    cWidget->setLayout(vLayout);

    setCentralWidget(cWidget);

    // Create the status bar
    m_statusBar = new StatusBar();
    m_statusBar->addPermanentWidget(new ToolButton(m_openOrthancAction));
    m_statusBar->addPermanentWidget(new ToolButton(m_newMergedSeriesAction));
    setStatusBar(m_statusBar);

    // Fusion dialog
    m_fusionDialog = new FusionDialog(this);
    connect(m_newMergedSeriesAction, SIGNAL(triggered()), this, SLOT(showFusionDialog()));
    connect(m_fusionDialog, SIGNAL(newFusionAsked(QString const&)), this, SLOT(createNewMergedSeries(QString const&)));

    // Event connections
    connect(m_tabbedSeries, SIGNAL(tabCloseRequested(int)), this, SLOT(closeInterface(int)));
    connect(m_quitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(&m_closeTimer, SIGNAL(timeout()), this, SLOT(close()));

    cout << "done." << endl;

    // Create orthanc dialog loader
    try {
        m_orthancDialog = new OrthancDialog(this);
        m_orthancDialog->show();
        changeStatusBarMessage("En attente de l'ouverture d'une série !");

        // Connect signals and slots
        connect(m_openOrthancAction, SIGNAL(triggered()), m_orthancDialog, SLOT(show()));
        connect(m_orthancDialog, SIGNAL(seriesLoaded(SeriesData*)), this, SLOT(createNewSeries(SeriesData*)));
    }
    catch(OrthancClient::OrthancClientException& e) {
        signalOrthancProblemAndExit();
    }

    // Show the viewer
    this->show(); 
}

// Destructor
ViewerWindow::~ViewerWindow()
{
    for(int i = m_interfacesList.size()-1 ; i >= 0 ; i--)
        delete m_interfacesList.at(i);

    cout << "Viewer window memory freed." << endl;
}

// The 'signalOrthancProblemAndExit' method
void ViewerWindow::signalOrthancProblemAndExit()
{
    cout << "Orthanc problem, exit." << endl;
    QMessageBox::critical(this, "Erreur", "Orthanc n'est pas en service !");
    changeStatusBarMessage("Fermeture du logiciel en cours...");
    m_closeTimer.start(1500);
}

// The 'attachInterface' method
void ViewerWindow::attachInterface(DisplayInterface* interface)
{
    int index = m_tabbedSeries->addTab(interface, interface->title());
    m_tabbedSeries->setCurrentIndex(index);
}

// The 'createNewSeries' slot
void ViewerWindow::createNewSeries(SeriesData* series)
{
    // Create the series interface
    SeriesInterface* newSeries = new SeriesInterface(series);
    connect(newSeries, SIGNAL(closed(DisplayInterface*)), this, SLOT(closeInterface(DisplayInterface*)));

    // Add it to the list and to the tabwidget
    m_interfacesList.push_back(newSeries);
    attachInterface(newSeries);

    m_fusionDialog->addNewSeries(newSeries);

    changeStatusBarMessage("Nouvelle série ouverte !");
}

// The 'createNewMergedSeries' slot
void ViewerWindow::createNewMergedSeries(QString const& title)
{
    // Create the merged series interface
    MergedSeriesInterface* newMergedSeries = new MergedSeriesInterface(title);
    connect(newMergedSeries, SIGNAL(closed(DisplayInterface*)), this, SLOT(closeInterface(DisplayInterface*)));

    // Add it to the list and to the tabwidget
    m_interfacesList.push_back(newMergedSeries);
    attachInterface(newMergedSeries);

    m_fusionDialog->addNewFusion(newMergedSeries);

    changeStatusBarMessage("Nouvelle interface de fusion créée !");
}

// The 'closeInterface' slot
void ViewerWindow::closeInterface(int index)
{
    DisplayInterface* interface = 0;

    // Get the interface from its index in the tabwidget and suppress it from
    // the list...
    QWidget* w = m_tabbedSeries->widget(index);
    for(vector<DisplayInterface*>::iterator iter = m_interfacesList.begin()
        ; iter != m_interfacesList.end() ; iter++)
    {
        if(w == *iter)
        {
            interface = *iter;
            m_interfacesList.erase(iter);
            break;
        }
    }
    // ... and from the tabwidget
    m_tabbedSeries->removeTab(index);

    if(dynamic_cast<SeriesInterface*>(interface) != 0)
        m_fusionDialog->removeSeries(dynamic_cast<SeriesInterface*>(interface));
    else if(dynamic_cast<MergedSeriesInterface*>(interface) != 0)
        m_fusionDialog->removeFusion(dynamic_cast<MergedSeriesInterface*>(interface));

    // Then frees its memory
    delete w;

    changeStatusBarMessage("Interface fermée !");

    // If it was the last opened series, propose to open a new one
    if(m_interfacesList.size() == 0)
    {
        m_orthancDialog->show();
        changeStatusBarMessage("En attente de l'ouverture d'une série !");
    }
}

// The 'closeInterface' slot
void ViewerWindow::closeInterface(DisplayInterface* interface)
{
    // Suppress the interface from the tab widget if it is inside...
    for(int i = 0 ; i < m_tabbedSeries->count() ; i++)
    {
        if(m_tabbedSeries->widget(i) == interface)
        {
            m_tabbedSeries->removeTab(i);
            break;
        }
    }

    // ... then from the list
    for(vector<DisplayInterface*>::iterator iter = m_interfacesList.begin()
        ; iter != m_interfacesList.end() ; iter++)
    {
        if(interface == *iter)
        {
            m_interfacesList.erase(iter);
            break;
        }
    }

    if(dynamic_cast<SeriesInterface*>(interface) != 0)
        m_fusionDialog->removeSeries(dynamic_cast<SeriesInterface*>(interface));
    else
        m_fusionDialog->removeFusion(dynamic_cast<MergedSeriesInterface*>(interface));

    // Then, frees its memory
    delete interface;

    changeStatusBarMessage("Interface fermée !");

    // If it was the last opened series, propose to open a new one
    if(m_interfacesList.size() == 0)
    {
        m_orthancDialog->show();
        changeStatusBarMessage("En attente de l'ouverture d'une série !");
    }
}

// The 'changeStatusBarMessage' method
void ViewerWindow::changeStatusBarMessage(QString const& message, unsigned int time)
{
    m_statusBar->showMessage(message, time);
}

// The 'clearStatusBarMessage' method
void ViewerWindow::clearStatusBarMessage()
{
    m_statusBar->clearMessage();
}

// The 'showFusionDialog' method
void ViewerWindow::showFusionDialog()
{
    m_fusionDialog->show();
    m_fusionDialog->askNewFusion(true);
}
