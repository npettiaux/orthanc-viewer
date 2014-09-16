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
//! \file OrthancDialog.h
//! \brief The OrthancDialog.h file contains the interface of the
//!        OrthancDialog class and the definitions of its inline
//!        methods.
//!
//! \author Quentin Smetz
//!

#ifndef ORTHANCDIALOG_H
#define ORTHANCDIALOG_H

#include <iostream>
#include <cmath>

#include <QLayout>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QTimer>
#include <QTreeView>

#include "View/Qt/customwidget/PushButton.h"
#include "View/Qt/customwidget/ProgressBar.h"
#include "View/Qt/customwidget/PushButton.h"
#include "View/Qt/customwidget/StandardItem.h"
#include "View/Qt/customwidget/TreeView.h"

#include "Model/SeriesData.h"
#include "View/Qt/OkCancelDialog.h"
#include "Controller/LoadSeriesThread.h"
#include "Controller/OrthancConnectionDialog.h"

//!
//! \brief The OrthancDialog class maintains a connexion with the Orthanc
//!        server and permits to the user to select and load a series.
//!
class OrthancDialog : public OkCancelDialog
{
    Q_OBJECT

    public:
        //!
        //! \brief The OrthancDialog constructor constructs the dialog and
        //!        opens a connexion on the Orthanc server.
        //!
        //! \param parent A pointer to the QWidget parent.
        //!
        OrthancDialog(QWidget* parent = 0);

        //!
        //! \brief The OrthancDialog destructor.
        //!
        ~OrthancDialog();

    public slots:
        //!
        //! \brief The connectOrthanc slot uses the internal connection of its
        //!        connection dialog to load the Orthanc content and initialize
        //!        the series loading thread.
        //!
        //! The method does nothing if the orthanc connection had not been
        //! initialized.
        //!
        //! \return Nothing.
        //!
        void connectOrthanc();

        //!
        //! \brief The checkSelection slot invokes the accept slot if the
        //!        selection is a series (and not a patient or a study).
        //!
        //! \param index A QModelIndex object which indicates the item
        //!              selection.
        //!
        //! \return Nothing.
        //!
        void checkSelection(QModelIndex const& index);

        //!
        //! \brief The accept slot checks the selection in the tree view widget.
        //!        If it is a series, it emits the corresponding signal and if
        //!        not, it runs a message box to indicate it to the user.
        //!
        //! \section emit
        //! The seriesSelected(QString) signal is emitted, if the selection is
        //! a series (and not a patient or a study).
        //!
        //! \return Nothing.
        //!
        void accept();

        //!
        //! \brief The sendLoadedSeries slot indicates that the dialog has
        //!        been valided (and so close it) and transfers the loaded
        //!        series.
        //!
        //! \section emit
        //! The seriesLoaded(SeriesData*) signal is emitted.
        //!
        //! \param series A pointer to the SeriesData object which contains the
        //!               loaded series.
        //!
        //! \return Nothing.
        //!
        void sendLoadedSeries(SeriesData* series);

        //!
        //! \brief The updateProgressBar slot updates the progress bar value
        //!        according to the progression value of the LoadSeriesThread
        //!        and then starts the progress bar timer.
        //!
        //! \return Nothing.
        //!
        void updateProgressBar();

        //!
        //! \brief The refresh slot refreshes the orthanc patient list.
        //!
        //! \return Nothing.
        //!
        void refresh();

    signals:
        //!
        //! \brief The seriesSelected signal, once emitted, indicates the series
        //!        id which have been valided by the user.
        //!
        //! \param seriesId A QString object which contains the series id.
        //!
        void seriesSelected(QString const& seriesId);

        //!
        //! \brief The seriesLoaded signal, once emitted, give access to a
        //!        loaded series data.
        //!
        //! \param series A pointer to a SeriesData object which contains
        //!               the fully loaded 3D series.
        //!
        void seriesLoaded(SeriesData* series);

    private:
        OrthancConnectionDialog* m_orthancConnectionDialog;

        customwidget::PushButton* m_refreshButton;

        customwidget::TreeView* m_orthancView;      // For Orthanc content
        QStandardItemModel m_orthancModel;          // For Orthanc content
        customwidget::ProgressBar* m_progressBar;   // For loading progression
        QTimer m_progressBarTimer;          // To control progression updating

        LoadSeriesThread* m_loadSeriesThread;        // For loading the series

        //!
        //! \brief The constructOrthancModel private method loads Orthanc data
        //!        from the server to the model which will be visualized in the
        //!        OrthancDialog.
        //!
        //! \return Nothing.
        //!
        void constructOrthancModel();
};

#endif
