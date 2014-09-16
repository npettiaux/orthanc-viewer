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
//! \file SubInterface.cpp
//! \brief The SubInterface.cpp file contains the definition of non-inline
//!        methods of the SubInterface class.
//!
//! \author Quentin Smetz
//!

#include "SubInterface.h"
using namespace std;
using namespace customwidget;

// Constructor
SubInterface::SubInterface() : Widget()
{   
    m_viewer = 0;

    // Create the interface
    m_gridLayout = new QGridLayout();
    setLayout(m_gridLayout);
}

// Destructor
SubInterface::~SubInterface()
{}

// The 'setViewer' method
void SubInterface::setViewer(Viewer* viewer)
{
    m_viewer = viewer;

    // Create the central layout
    QHBoxLayout* centralLayout = new QHBoxLayout();

    centralLayout->addWidget(m_viewer);

    m_gridLayout->addLayout(centralLayout, 1, 1); // Permits to add components in subclasses

    // To check event from the viewer
    m_viewer->installEventFilter(this);
}

// The 'eventFilter' method
bool SubInterface::eventFilter(QObject* object, QEvent* event)
{
    // If the event is from the viewer
    if(object == m_viewer)
    {
        // Simulate a double click if the viewer was double clicked
        if(event->type() == QEvent::MouseButtonDblClick)
            mouseDoubleClickEvent(static_cast<QMouseEvent*>(event));
    }

    return Widget::eventFilter(object, event);
}

// The 'mouseDoubleClickEvent' method
void SubInterface::mouseDoubleClickEvent(QMouseEvent* event)
{
    // Emits a signal which indicates the SubInterface was double clicked
    emit doubleClicked(this);
    event->accept(); // Stop the event
}
