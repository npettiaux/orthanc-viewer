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
//! \file DisplayInterface.cpp
//! \brief The DisplayInterface.cpp file contains the definition of non-inline
//!        methods of the DisplayInterface class.
//!
//! \author Quentin Smetz
//!

#include "DisplayInterface.h"
#include "Model/ProgramConfiguration.h"
#include "Controller/ViewerWindow.h"
using namespace customwidget;
using namespace std;

// Constructor
DisplayInterface::DisplayInterface(QWidget* parent) : Widget(parent), m_title("")
{
    // Create the interface
    QVBoxLayout* mainVLayout = new QVBoxLayout();
    mainVLayout->setDirection(QBoxLayout::BottomToTop);

    m_horizontalSplitter = new Splitter(Qt::Horizontal);

    m_leftSplitter = new Splitter(Qt::Vertical);
    m_rightSplitter = new Splitter(Qt::Vertical);

    m_horizontalSplitter->addWidget(m_leftSplitter);
    m_horizontalSplitter->addWidget(m_rightSplitter);

    mainVLayout->addWidget(m_horizontalSplitter);

    setLayout(mainVLayout);

    // Creates the toolbar
    m_toolBar = new ToolBar();
    m_toolBar->setIconSize(QSize(25, 25));

    QString imgDir(ProgramConfiguration::instance()->imageDirectory().c_str());

    m_axesAction = new QAction(QIcon(imgDir + "/axes_icon.png"),
                               "Montrer les axes", m_toolBar);
    m_axesAction->setCheckable(true);
    m_toolBar->addAction(m_axesAction);

    m_toolBar->addSeparator();

    m_layoutActionGroup = new QActionGroup(this);
    QAction* action1 = new QAction(QIcon(imgDir + "/layout_1_icon.png"),
                                   "Layout 1", m_layoutActionGroup);
    action1->setCheckable(true);
    action1->setChecked(true);
    m_toolBar->addAction(action1);
    QAction* action2 = new QAction(QIcon(imgDir + "/layout_2_icon.png"),
                                   "Layout 2", m_layoutActionGroup);
    action2->setCheckable(true);
    m_toolBar->addAction(action2);

    m_toolBar->addSeparator();

    m_toWindowAction = new QAction(QIcon(imgDir + "/towindow_icon.png"),
                                   "Mode fenêtré", m_toolBar);
    m_toWindowAction->setCheckable(true);
    m_toolBar->addAction(m_toWindowAction);

    m_fullScreenAction = new QAction(QIcon(imgDir + "/fullscreen_icon.png"),
                                     "Plein écran", m_toolBar);
    m_fullScreenAction->setCheckable(true);
    m_toolBar->addAction(m_fullScreenAction);

    m_toolBar->addSeparator();

    setToolBar(m_toolBar);

    // Event connections
    connect(m_axesAction, SIGNAL(toggled(bool)), this, SLOT(showAxes(bool)));
    connect(m_layoutActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(setSelectedLayoutStyle()));
    connect(m_toWindowAction, SIGNAL(toggled(bool)), this, SLOT(setSelectedWindowMode()));
    connect(m_fullScreenAction, SIGNAL(toggled(bool)), this, SLOT(setSelectedWindowMode()));
}

// Destructor
DisplayInterface::~DisplayInterface()
{}

// The 'setLayoutStyle' slot
void DisplayInterface::setLayoutStyle(LayoutStyle layoutStyle)
{
    // Remove the viewers from their splitter
    for(unsigned int i = 0 ; i < 4 ; i++)
        m_subInterface[i]->setParent(0);

    QList<int> horizontalSizes;
    QList<int> leftSizes, rightSizes;

    switch(layoutStyle)
    {
        case DisplayInterface::MODE1:
            // Put the viewers in their corresponding splitter ...
            m_subInterface[VOLUME]->setParent(m_leftSplitter);
            m_subInterface[SAGITTAL_SLICE]->setParent(m_leftSplitter);
            m_subInterface[FRONTAL_SLICE]->setParent(m_rightSplitter);
            m_subInterface[TRANSVERSE_SLICE]->setParent(m_rightSplitter);
            // ... and compute their size
            leftSizes.append(this->geometry().height()/2);
            leftSizes.append(this->geometry().height()/2);
            rightSizes.append(this->geometry().height()/2);
            rightSizes.append(this->geometry().height()/2);
            break;

        case DisplayInterface::MODE2:
            // Put the viewers in their corresponding splitter ...
            m_subInterface[VOLUME]->setParent(m_leftSplitter);
            m_subInterface[SAGITTAL_SLICE]->setParent(m_rightSplitter);
            m_subInterface[FRONTAL_SLICE]->setParent(m_rightSplitter);
            m_subInterface[TRANSVERSE_SLICE]->setParent(m_rightSplitter);
            // ... and compute their size
            leftSizes.append(this->geometry().height());
            rightSizes.append(this->geometry().height()/3);
            rightSizes.append(this->geometry().height()/3);
            rightSizes.append(this->geometry().height()/3);
            break;
    }

    horizontalSizes.append(this->geometry().width()/2);
    horizontalSizes.append(this->geometry().width()/2);

    // Adjust the size of the splitter elements
    m_horizontalSplitter->setSizes(horizontalSizes);
    m_leftSplitter->setSizes(leftSizes);
    m_rightSplitter->setSizes(rightSizes);

    // Show the viewers
    for(unsigned int i = 0 ; i < 4 ; i++)
        m_subInterface[i]->show();
}

// The 'setSelectedLayoutStyle' slot
void DisplayInterface::setSelectedLayoutStyle()
{
    // Check the checked style action...
    QList<QAction*> actionsList = m_layoutActionGroup->actions();
    int index = actionsList.indexOf(m_layoutActionGroup->checkedAction());

    // ... and convert it in LayoutStyle constant to apply it
    setLayoutStyle(static_cast<LayoutStyle>(index));
}

// The 'setSubInterfaceMaximized' slot
void DisplayInterface::setSubInterfaceMaximized(SubInterface* subInterface)
{
    // Check if a subinterface is already maximized...
    bool isAlreadyMaximized = false;
    for(unsigned int i = 0 ; i < 4 ; i++)
        isAlreadyMaximized = isAlreadyMaximized || m_subInterface[i]->isHidden();

    if(!isAlreadyMaximized)
    {
        // ... if not, hide them all and then show the asked one
        for(unsigned int i = 0 ; i < 4 ; i++)
            m_subInterface[i]->hide();

        subInterface->show();
    }
    else
    {
        // ... if yes, show them all
        for(unsigned int i = 0 ; i < 4 ; i++)
            m_subInterface[i]->show();
    }
}

// The 'updateViewConfiguration' slot
void DisplayInterface::updateViewConfiguration(ViewConfiguration const& config, ViewConfiguration::ViewParam param)
{
    for(unsigned int i = 0 ; i < 4 ; i++)
        m_subInterface[i]->viewer()->updateView(config, param);

    emit viewConfigurationChanged(config, param);
}

// The 'setSelectedWindowMode' slot
void DisplayInterface::setSelectedWindowMode()
{
    bool windowMode = m_toWindowAction->isChecked();
    bool fullscreenMode = m_fullScreenAction->isChecked();

    setParent(0);

    if(fullscreenMode)
        showFullScreen();
    else
    {
        if(windowMode)
            showNormal();
        else
            ViewerWindow::instance()->attachInterface(this);
    }

    repaint();
}

// The 'showAxes' slot
void DisplayInterface::showAxes(bool show)
{
    for(unsigned int i = 0 ; i < 4 ; i++)
        m_subInterface[i]->viewer()->showAxes(show);
}

// The 'closeEvent' method
void DisplayInterface::closeEvent(QCloseEvent* event)
{
    emit closed(this);
    Widget::closeEvent(event);
}

// The 'setToolBar' method
void DisplayInterface::setToolBar(QToolBar* toolBar)
{
    layout()->addWidget(toolBar);
}
