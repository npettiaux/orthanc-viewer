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
//! \file FusionDialog.h
//! \brief The FusionDialog.h file contains the interface of the
//!        FusionDialog class and the definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef FUSIONDIALOG_H
#define FUSIONDIALOG_H

#include <QBoxLayout>
#include <QInputDialog>

#include "View/Qt/customwidget/CheckBox.h"
#include "View/Qt/customwidget/ComboBox.h"
#include "View/Qt/customwidget/Dialog.h"
#include "View/Qt/customwidget/PushButton.h"

#include "Controller/MergedSeriesInterface.h"
#include "Controller/SeriesInterface.h"

//!
//! \brief The FusionDialog class is a dedicated dialog to control the fusion
//!        of a set of series.
//!
class FusionDialog : public customwidget::Dialog
{
    Q_OBJECT

    public:
        //!
        //! \brief The FusionDialog constructor creates the interface of the
        //!        dialog.
        //!
        //! \param parent A pointer to the parent of the dialog.
        //!
        FusionDialog(QWidget* parent = 0);

        //!
        //! \brief The FusionDialog destructor.
        //!
        ~FusionDialog();

        //!
        //! \brief The addNewSeries method adds a series interface to be
        //!        potentially chosen to be part as a fusion.
        //!
        //! \param interface A pointer to the series interface to add.
        //!
        //! \return Nothing.
        //!
        void addNewSeries(SeriesInterface* interface);

        //!
        //! \brief The addNewFusion method adds a merged series interface to
        //!        the list of customizable ones.
        //!
        //! \param interface A pointer to the merged series interface to add.
        //!
        //! \return Nothing.
        //!
        void addNewFusion(MergedSeriesInterface* interface);

        //!
        //! \brief The removeSeries method removes a series interface and
        //!        release it from any fusion it is potentially in.
        //!
        //! \param interface A pointer to the series interface to remove.
        //!
        //! \return Nothing.
        //!
        void removeSeries(SeriesInterface* interface);

        //!
        //! \brief The removeFusion method removes a merged series interface and
        //!        release each of the series interface it contains.
        //!
        //! \param interface A pointer to the merged series interface to remove.
        //!
        //! \return Nothing.
        //!
        void removeFusion(MergedSeriesInterface* interface);

    public slots:
        //!
        //! \brief The askNewFusion slot permits to signal that the dialog
        //!        wants a merged series interface to be added.
        //!
        //! A title is chosen during the function.
        //!
        //! \param ifNone A boolean which is true if the method must do
        //!               something only if no fusion is already created and
        //!               false if it is to be ignored.
        //!
        //! \return Nothing.
        //!
        void askNewFusion(bool ifNone = false);

        //!
        //! \brief The updateCheckboxes updates the list of selected series
        //!        series interface according to the selected merged series
        //!        interface.
        //!
        //! \return Nothing.
        //!
        void updateCheckboxes();

        //!
        //! \brief The moveSeriesInterfaceIntoFusion adds/removes the series
        //!        the series interface (specified by index) according to its
        //!        checked/unchecked selection to/from the selected merged
        //!        series interface.
        //!
        //! \param index The index of the series interface to add/remove.
        //!
        //! \return Nothing.
        //!
        void moveSeriesInterfaceIntoFusion(int index);

    signals:
        //!
        //! \brief The newFusionAsked signal, once emitted, specifies the title
        //!        of the new merged series interface that need to be created.
        //!
        void newFusionAsked(QString const& title);

    private:
        customwidget::ComboBox* m_mergedSeriesComboBox;
        std::vector<MergedSeriesInterface*> m_mergedSeriesInterfaces;
        std::vector<SeriesInterface*> m_seriesInterfaces;

        QButtonGroup m_checkBoxesGroup;
        std::vector<customwidget::CheckBox*> m_seriesCheckBoxes;
        QVBoxLayout* m_seriesLayout;

        customwidget::PushButton* m_addMergedSeriesButton;
};

#endif
