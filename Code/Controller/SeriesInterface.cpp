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
//! \file SeriesInterface.cpp
//! \brief The SeriesInterface.cpp file contains the definition of non-inline
//!        methods of the SeriesInterface class.
//!
//! \author Quentin Smetz
//!

#include "SeriesInterface.h"
#include "Model/ProgramConfiguration.h"
using namespace std;
using namespace customwidget;

// Constructor
SeriesInterface::SeriesInterface(SeriesData* series, QWidget* parent)
    : DisplayInterface(parent)
{
    cout << "Building series interface... " << flush;

    m_series.TakeReference(series);
    m_title = "";
    m_title += QString("[") + QString(m_series->modality().c_str()) + QString("] ");
    m_title += QString(m_series->patientName().c_str());
    //m_title += QString("\n") + QString(m_series->studyDescription().c_str());
    //m_title += QString("\n") + QString(m_series->seriesDescription().c_str());
    setWindowTitle(m_title);

    // Update the toolbar
    QString imgDir(ProgramConfiguration::instance()->imageDirectory().c_str());
    m_hounsfieldColormapAction = new QAction(QIcon(imgDir + "/colormap_icon.png"),
                                             "Fenêtrage et couleurs", m_toolBar);
    m_translationRotationAction = new QAction(QIcon(imgDir + "/geometric_transformation_icon.png"),
                                              "Translation et rotation", m_toolBar);
    m_toolBar->addActions(getCustomActions());

    // Create the subinterfaces
    m_subInterface[VOLUME] = new VolumeSubInterface(m_series);
    m_subInterface[SAGITTAL_SLICE] = new SliceSubInterface(m_series, SAGITTAL);
    m_subInterface[FRONTAL_SLICE] = new SliceSubInterface(m_series, FRONTAL);
    m_subInterface[TRANSVERSE_SLICE] = new SliceSubInterface(m_series, TRANSVERSE);

    setLayoutStyle(DisplayInterface::MODE1);

    // Dialog tools
    m_hounsfieldColormapDialog = new HounsfieldColormapDialog
            (m_series->computeBasicHounsfieldRanges(), m_series->getBasicHounsfield(), this);
    m_translationRotationDialog = new TranslationRotationDialog(this);

    // Event connections
    connect(m_hounsfieldColormapAction, SIGNAL(triggered()), m_hounsfieldColormapDialog, SLOT(show()));
    connect(m_hounsfieldColormapDialog, SIGNAL(newConfiguration(ViewConfiguration const&, ViewConfiguration::ViewParam)), this, SLOT(updateViewConfiguration(ViewConfiguration const&, ViewConfiguration::ViewParam)));
    m_hounsfieldColormapDialog->reset();

    connect(m_translationRotationAction, SIGNAL(triggered()), m_translationRotationDialog, SLOT(show()));
    connect(m_translationRotationDialog, SIGNAL(newConfiguration(ViewConfiguration const&, ViewConfiguration::ViewParam)), this, SLOT(updateViewConfiguration(ViewConfiguration const&, ViewConfiguration::ViewParam)));
    m_translationRotationDialog->reset();

    for(unsigned int i = 0 ; i < 4 ; i++)
        connect(m_subInterface[i], SIGNAL(doubleClicked(SubInterface*)), this, SLOT(setSubInterfaceMaximized(SubInterface*)));

    // This code is normally useless but can counter a VTK initialization bug
    dynamic_cast<SliceSubInterface*>(m_subInterface[SAGITTAL_SLICE])->resetSlider();
    dynamic_cast<SliceSubInterface*>(m_subInterface[FRONTAL_SLICE])->resetSlider();
    dynamic_cast<SliceSubInterface*>(m_subInterface[TRANSVERSE_SLICE])->resetSlider();

    cout << "done." << endl;
}

// Destructor
SeriesInterface::~SeriesInterface()
{
    cout << "Series interface freed." << endl;
}

// The 'allowFusion' method
void SeriesInterface::allowFusion(bool allow)
{
    for(unsigned int i = 0 ; i < 4 ; i++)
    {
        SeriesViewer* sv = dynamic_cast<SeriesViewer*>(m_subInterface[i]->viewer());
        sv->allowFusion(allow);
    }

    if(allow)
    {
        setParent(0);
        hide();
    }
    else
    {
        setSelectedWindowMode();
        dynamic_cast<SliceSubInterface*>(m_subInterface[SAGITTAL_SLICE])->updateSliceRange();
        dynamic_cast<SliceSubInterface*>(m_subInterface[FRONTAL_SLICE])->updateSliceRange();
        dynamic_cast<SliceSubInterface*>(m_subInterface[TRANSVERSE_SLICE])->updateSliceRange();
    }
}

// The 'getCustomActions' method
QList<QAction*> SeriesInterface::getCustomActions() const
{
    QList<QAction*> actions;
    actions.append(m_hounsfieldColormapAction);
    actions.append(m_translationRotationAction);

    return actions;
}

// The 'getPropsOpacity' method
double SeriesInterface::getPropsOpacity() const
{
    return dynamic_cast<SeriesViewer*>(m_subInterface[0]->viewer())->getPropOpacity();
}

// The 'setPropsOpacity' method
void SeriesInterface::setPropsOpacity(double opacity)
{
    for(unsigned int i = 0 ; i < 4 ; i++)
        dynamic_cast<SeriesViewer*>(m_subInterface[i]->viewer())->setPropOpacity(opacity);
}
