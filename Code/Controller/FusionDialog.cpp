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
//! \file FusionDialog.cpp
//! \brief The FusionDialog.cpp file contains the definition of non-inline
//!        methods of the FusionDialog class.
//!
//! \author Quentin Smetz
//!

#include "FusionDialog.h"
using namespace std;
using namespace customwidget;

// Constructor
FusionDialog::FusionDialog(QWidget* parent) : Dialog(parent)
{
    setWindowTitle("Fusion");

    // Create the interface
    QVBoxLayout* vLayout = new QVBoxLayout();

    m_addMergedSeriesButton = new PushButton("Ajouter une fusion");
    vLayout->addWidget(m_addMergedSeriesButton);

    m_mergedSeriesComboBox = new ComboBox();
    vLayout->addWidget(m_mergedSeriesComboBox);

    GroupBox* seriesBox = new GroupBox("Séries");
    m_seriesLayout = new QVBoxLayout();
    seriesBox->setLayout(m_seriesLayout);
    vLayout->addWidget(seriesBox);

    m_checkBoxesGroup.setExclusive(false); 

    PushButton* okButton = new PushButton("Ok");
    vLayout->addWidget(okButton);

    setLayout(vLayout);

    // Event connections
    connect(m_mergedSeriesComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateCheckboxes()));
    connect(&m_checkBoxesGroup, SIGNAL(buttonClicked(int)), this, SLOT(moveSeriesInterfaceIntoFusion(int)));
    connect(m_addMergedSeriesButton, SIGNAL(clicked()), this, SLOT(askNewFusion()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
}

// Destructor
FusionDialog::~FusionDialog()
{}

// The 'addNewSeries' method
void FusionDialog::addNewSeries(SeriesInterface* interface)
{
    m_seriesInterfaces.push_back(interface);

    CheckBox* checkbox = new CheckBox(interface->title());
    m_seriesCheckBoxes.push_back(checkbox);
    m_seriesLayout->addWidget(checkbox);
    m_checkBoxesGroup.addButton(checkbox, m_seriesCheckBoxes.size()-1);

    updateCheckboxes();
}

// The 'addNewFusion' method
void FusionDialog::addNewFusion(MergedSeriesInterface* interface)
{
    m_mergedSeriesInterfaces.push_back(interface);
    m_mergedSeriesComboBox->addItem(interface->title());
}

// The 'removeSeries' method
void FusionDialog::removeSeries(SeriesInterface* interface)
{
    unsigned int index;
    for(unsigned int i = 0 ; i < m_seriesInterfaces.size() ; i++)
    {
        if(m_seriesInterfaces.at(i) == interface)
            index = i;
    }

    for(unsigned int i = 0 ; i < m_mergedSeriesInterfaces.size() ; i++)
        m_mergedSeriesInterfaces.at(i)->removeSeriesInterface(interface);

    m_seriesInterfaces.erase(m_seriesInterfaces.begin()+index);

    CheckBox* checkbox = m_seriesCheckBoxes.at(index);
    m_seriesLayout->removeWidget(checkbox);
    m_checkBoxesGroup.removeButton(checkbox);
    for(unsigned int i = index+1 ; i < m_seriesCheckBoxes.size() ; i++)
        m_checkBoxesGroup.setId(m_checkBoxesGroup.button(i), i-1);
    m_seriesCheckBoxes.erase(m_seriesCheckBoxes.begin()+index);
    delete checkbox;

    updateCheckboxes();
}

// The 'removeFusion' method
void FusionDialog::removeFusion(MergedSeriesInterface* interface)
{
    unsigned int index = 0;
    for(vector<MergedSeriesInterface*>::iterator iter = m_mergedSeriesInterfaces.begin()
         ; iter != m_mergedSeriesInterfaces.end() ; iter++)
    {
        if(*iter == interface)
        {
            m_mergedSeriesInterfaces.erase(iter);
            break;
        }
        index++;
    }

    interface->removeAllSeriesInterface();

    m_mergedSeriesComboBox->removeItem(index);

    updateCheckboxes();
}

// The 'askNewFusion' slot
void FusionDialog::askNewFusion(bool ifNone)
{
    if(!ifNone || m_mergedSeriesInterfaces.empty())
    {
        bool ok;
        QString title = QInputDialog::getText(this, "Choix du titre",
                                              "Entrez le titre de l'interface de fusion",
                                              QLineEdit::Normal, "Fusion", &ok);
        if(ok)
            emit newFusionAsked(title);
    }
}

// The 'updateCheckboxes' slot
void FusionDialog::updateCheckboxes()
{
    m_checkBoxesGroup.blockSignals(true);

    for(unsigned int i = 0 ; i < m_seriesCheckBoxes.size() ; i++)
    {
        m_seriesCheckBoxes.at(i)->setChecked(false);
        m_seriesCheckBoxes.at(i)->setEnabled(m_mergedSeriesInterfaces.size() > 0);
    }

    if(m_mergedSeriesInterfaces.size() > 0)
    {
        MergedSeriesInterface const* msi = m_mergedSeriesInterfaces.at(m_mergedSeriesComboBox->currentIndex());
        for(unsigned int i = 0 ; i < m_seriesInterfaces.size() ; i++)
        {
            if(msi->containsSeriesInterface(m_seriesInterfaces.at(i)))
                m_seriesCheckBoxes.at(i)->setChecked(true);
        }

        for(unsigned int i = 0 ; i < m_seriesCheckBoxes.size() ; i++)
        {
            bool checkedSomewhere = false;

            for(vector<MergedSeriesInterface*>::iterator iter = m_mergedSeriesInterfaces.begin()
                 ; iter != m_mergedSeriesInterfaces.end() && !checkedSomewhere ; iter++)
            {
                if(*iter == msi)
                    continue;

                if((*iter)->containsSeriesInterface(m_seriesInterfaces.at(i)))
                    checkedSomewhere = true;
            }

            if(checkedSomewhere)
                m_seriesCheckBoxes.at(i)->setEnabled(false);
        }
    }

    m_checkBoxesGroup.blockSignals(false);
}

// The 'moveSeriesInterfaceIntoFusion' slot
void FusionDialog::moveSeriesInterfaceIntoFusion(int index)
{
    MergedSeriesInterface* msi = m_mergedSeriesInterfaces.at(m_mergedSeriesComboBox->currentIndex());

    if(m_seriesCheckBoxes.at(index)->isChecked())
        msi->addSeriesInterface(m_seriesInterfaces.at(index));
    else
        msi->removeSeriesInterface(m_seriesInterfaces.at(index));
}
