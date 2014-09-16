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
//! \file OkCancelDialog.cpp
//! \brief The OkCancelDialog.cpp file contains the definition of
//!        non-inline methods of the OkCancelDialog class.
//!
//! \author Quentin Smetz
//!

#include "OkCancelDialog.h"
using namespace std;
using namespace customwidget;

// Constructor
OkCancelDialog::OkCancelDialog(QWidget* parent) : Dialog(parent)
{
    setWindowOpacity(0.9);

    // Create the basic interface
    QVBoxLayout* vLayout = new QVBoxLayout();

    m_dialogArea = new QWidget();
    vLayout->addWidget(m_dialogArea);

    QHBoxLayout* hLayout = new QHBoxLayout();
    m_okButton = new PushButton("Valider");
    hLayout->addWidget(m_okButton);
    m_cancelButton = new PushButton("Annuler");
    hLayout->addWidget(m_cancelButton);

    vLayout->addLayout(hLayout);

    Dialog::setLayout(vLayout);

    // Event connections
    connect(m_okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(m_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

// Destructor
OkCancelDialog::~OkCancelDialog()
{}

// The layout method
QLayout* OkCancelDialog::layout() const
{
    return m_dialogArea->layout();
}

// The setLayout method
void OkCancelDialog::setLayout(QLayout* layout)
{
    layout->setMargin(0);
    m_dialogArea->setLayout(layout);
}
