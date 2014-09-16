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
//! \file MergedSeriesInterface.cpp
//! \brief The MergedSeriesInterface.cpp file contains the definition of
//!        non-inline methods of the MergedSeriesInterface class.
//!
//! \author Quentin Smetz
//!

#include "MergedSeriesInterface.h"
using namespace std;
using namespace customwidget;

// Constructor
MergedSeriesInterface::MergedSeriesInterface(QString const& title, QWidget* parent)
    : DisplayInterface(parent), m_seriesInterfaces()
{
    cout << "Building merged series interface... " << flush;

    m_title = title;
    setWindowTitle(m_title);

    m_seriesSelector = new ComboBox();
    m_seriesSelector->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    m_toolBar->addWidget(m_seriesSelector);
    m_lastSelectedSeriesIndex = -1;

    m_opacitySlider = new DoubleSlider(Qt::Horizontal);
    m_opacitySlider->setDoubleRange(Range(0, 1), 0.05);
    m_opacitySlider->setMaximumWidth(100);
    m_opacitySlider->setEnabled(false);
    m_toolBar->addWidget(m_opacitySlider);

    m_subInterface[VOLUME] = new VolumeSubInterface(new MergedSeriesVolumeViewer());
    m_subInterface[SAGITTAL_SLICE] = new SliceSubInterface(
                new MergedSeriesSliceViewer(SAGITTAL));
    m_subInterface[FRONTAL_SLICE] = new SliceSubInterface(
                new MergedSeriesSliceViewer(FRONTAL));
    m_subInterface[TRANSVERSE_SLICE] = new SliceSubInterface(
                new MergedSeriesSliceViewer(TRANSVERSE));

    setLayoutStyle(DisplayInterface::MODE1);

    for(unsigned int i = 0 ; i < 4 ; i++)
        connect(m_subInterface[i], SIGNAL(doubleClicked(SubInterface*)), this, SLOT(setSubInterfaceMaximized(SubInterface*)));

    connect(m_seriesSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(updateToolBarForSeries(int)));
    connect(m_opacitySlider, SIGNAL(doubleValueChanged(double)), this, SLOT(changeCurrentSeriesOpacity(double)));

    cout << "done." << endl;
}

// Destructor
MergedSeriesInterface::~MergedSeriesInterface()
{
     cout << "Merged series interface freed." << endl;
}

// The 'addSeriesInterface' method
void MergedSeriesInterface::addSeriesInterface(SeriesInterface* interface)
{
    if(containsSeriesInterface(interface))
        return;

    m_seriesInterfaces.push_back(interface);

    interface->allowFusion(true);
    m_seriesSelector->addItem(interface->title());
    connect(interface, SIGNAL(viewConfigurationChanged(ViewConfiguration const&, ViewConfiguration::ViewParam)),
            this, SLOT(updateViewConfiguration(ViewConfiguration const&, ViewConfiguration::ViewParam)));

    for(unsigned int i = 0 ; i < 4 ; i++)
    {
        MergedSeriesViewer* msv = 0;
        msv = dynamic_cast<MergedSeriesViewer*>(m_subInterface[i]->viewer());

        SeriesViewer* sv = dynamic_cast<SeriesViewer*>(interface->viewer(i));
        msv->linkSeriesViewer(sv);
    }

    dynamic_cast<SliceSubInterface*>(m_subInterface[SAGITTAL_SLICE])->updateSliceRange();
    dynamic_cast<SliceSubInterface*>(m_subInterface[FRONTAL_SLICE])->updateSliceRange();   
    dynamic_cast<SliceSubInterface*>(m_subInterface[TRANSVERSE_SLICE])->updateSliceRange();

    if(m_seriesInterfaces.size() == 1)
        m_opacitySlider->setDoubleValue(1.0);
}

// The 'removeSeriesInterface' method
void MergedSeriesInterface::removeSeriesInterface(SeriesInterface* interface, bool checkIfContains)
{
    if(checkIfContains && !containsSeriesInterface(interface))
        return;

    for(unsigned int i = 0 ; i < 4 ; i++)
    {
        MergedSeriesViewer* msv = 0;
        msv = dynamic_cast<MergedSeriesViewer*>(m_subInterface[i]->viewer());

        SeriesViewer* sv = dynamic_cast<SeriesViewer*>(interface->viewer(i));
        msv->unlinkSeriesViewer(sv);
    }

    disconnect(interface, SIGNAL(viewConfigurationChanged(ViewConfiguration const&, ViewConfiguration::ViewParam)),
               this, SLOT(updateViewConfiguration(ViewConfiguration const&, ViewConfiguration::ViewParam)));
    QList<QAction*> actions = interface->getCustomActions();
    for(int i = 0 ; i < actions.size() ; i++)
        m_toolBar->removeAction(actions.at(i));

    interface->allowFusion(false);

    unsigned int pos = 0;
    vector<SeriesInterface*>::iterator iter = m_seriesInterfaces.begin();
    while(iter != m_seriesInterfaces.end())
    {
        if(interface == *iter)
            break;

        pos++;
        iter++;
    }
    updateToolBarForSeries(-1);
    m_seriesSelector->blockSignals(true);
    m_seriesSelector->removeItem(pos);
    m_seriesInterfaces.erase(iter);
    m_seriesSelector->blockSignals(false);
    if(m_seriesSelector->count() > 0)
    {
        m_seriesSelector->setCurrentIndex(0);
        updateToolBarForSeries(0);
    }

    dynamic_cast<SliceSubInterface*>(m_subInterface[SAGITTAL_SLICE])->updateSliceRange();
    dynamic_cast<SliceSubInterface*>(m_subInterface[FRONTAL_SLICE])->updateSliceRange(); 
    dynamic_cast<SliceSubInterface*>(m_subInterface[TRANSVERSE_SLICE])->updateSliceRange();

    if(m_seriesInterfaces.size() == 1)
        m_opacitySlider->setDoubleValue(1.0);
}

// The 'removeAllSeriesInterface' method
void MergedSeriesInterface::removeAllSeriesInterface()
{
    while(m_seriesInterfaces.size() > 0)
        removeSeriesInterface(m_seriesInterfaces.at(0));
}

// The 'containsSeriesInterface' method
bool MergedSeriesInterface::containsSeriesInterface(SeriesInterface const* interface) const
{
    for(vector<SeriesInterface*>::const_iterator iter = m_seriesInterfaces.begin()
         ; iter != m_seriesInterfaces.end() ; iter++)
    {
        if(*iter == interface)
            return true;
    }

    return false;
}

// The 'updateToolBarForSeries' slot
void MergedSeriesInterface::updateToolBarForSeries(int index)
{
    // Opacity
    if(index >= 0)
    {
        m_opacitySlider->setEnabled(true);
        SeriesInterface* interface = m_seriesInterfaces.at(m_seriesSelector->currentIndex());
        m_opacitySlider->setDoubleValue(interface->getPropsOpacity());
    }
    else
        m_opacitySlider->setEnabled(false);

    // Actions
    if(m_lastSelectedSeriesIndex >= 0)
    {
        QList<QAction*> actions = m_seriesInterfaces.at(m_lastSelectedSeriesIndex)->getCustomActions();
        for(int i = 0 ; i < actions.size() ; i++)
            m_toolBar->removeAction(actions.at(i));
    }

    if(index >= 0)
        m_toolBar->addActions(m_seriesInterfaces.at(index)->getCustomActions());

    m_lastSelectedSeriesIndex = index;
}

// The 'changeCurrentSeriesOpacity' slot
void MergedSeriesInterface::changeCurrentSeriesOpacity(double opacity)
{
    if(m_seriesInterfaces.size() == 0)
        return;

    SeriesInterface* interface = m_seriesInterfaces.at(m_seriesSelector->currentIndex());
    interface->setPropsOpacity(opacity);
    for(unsigned int i = 0 ; i < 4 ; i++)
        viewer(i)->repaint();
}
