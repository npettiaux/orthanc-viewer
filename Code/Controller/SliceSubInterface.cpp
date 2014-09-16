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
//! \file SliceSubInterface.cpp
//! \brief The SliceSubInterface.cpp file contains the definition of non-inline
//!        methods of the SliceSubInterface class.
//!
//! \author Quentin Smetz
//!

#include "SliceSubInterface.h"
using namespace std;
using namespace customwidget;

// Constructor
SliceSubInterface::SliceSubInterface(SeriesData* series, SliceOrientation orien)
    : SubInterface(), m_orientation(orien)
{
    setViewer(new SeriesSliceViewer(series, orien));
    initInterface();
}

// Constructor II
SliceSubInterface::SliceSubInterface(MergedSeriesSliceViewer* viewer) : SubInterface()
{
    m_orientation = viewer->orientation();
    setViewer(viewer);
    initInterface();
}

// Destructor
SliceSubInterface::~SliceSubInterface()
{}

// The 'updateSliceRange' method
void SliceSubInterface::updateSliceRange()
{
    m_sliceSlider->setDoubleRange(Range(m_viewer->minSlice(), m_viewer->maxSlice()), (m_viewer->maxSlice()-m_viewer->minSlice())/1000.0);
    resetSlider();
}

// The 'initInterface' method
void SliceSubInterface::initInterface()
{
    // Create the interface
    QVBoxLayout* vLayout = new QVBoxLayout();

    m_sliceSlider = new DoubleSlider(Qt::Vertical);
    m_sliceSlider->setSingleStep(1);
    m_sliceSlider->setPageStep(1);
    m_sliceSlider->setFixedWidth(30);
    vLayout->addWidget(m_sliceSlider);

    m_resetSliderButton = new PushButton("");
    m_resetSliderButton->setFixedSize(QSize(30, 30));
    vLayout->addWidget(m_resetSliderButton);

    m_gridLayout->addLayout(vLayout, 1, 2);

    Label* label = new Label("");
    switch(m_orientation)
    {
        case SAGITTAL:
            m_sliceSlider->setInvertedAppearance(true);
            m_sliceSlider->setInvertedControls(true);
            label->setText("Sagittal");
            break;

        case FRONTAL:
            label->setText("Frontal");
            break;

        case TRANSVERSE:
            label->setText("Transverse");
            break;
    }
    m_gridLayout->addWidget(label, 0, 1);

    // Event connection
    connect(m_sliceSlider, SIGNAL(doubleValueChanged(double)), m_viewer, SLOT(changeCurrentSlice(double)));
    connect(m_resetSliderButton, SIGNAL(clicked()), this, SLOT(resetSlider()));

    // Some post-init
    updateSliceRange();
}

// The 'resetSlider' method
void SliceSubInterface::resetSlider()
{
    Range const& range = m_sliceSlider->doubleRange();
    m_sliceSlider->setDoubleValue(range.max()); // To force the slider to move
    m_sliceSlider->setDoubleValue((range.min()+range.max())/2);
}
