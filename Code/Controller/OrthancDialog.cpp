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
//! \file OrthancDialog.cpp
//! \brief The OrthancDialog.cpp file contains the definition of
//!        non-inline methods of the OrthancDialog class.
//!
//! \author Quentin Smetz
//!

#include "OrthancDialog.h"
#include "Controller/ViewerWindow.h"
using namespace std;
using namespace customwidget;

// Constructor
OrthancDialog::OrthancDialog(QWidget* parent)
    : OkCancelDialog(parent), m_loadSeriesThread(0)
{
    m_orthancConnectionDialog = new OrthancConnectionDialog(this);
    m_orthancConnectionDialog->exec();
    connectOrthanc();

    setModal(true);
    setMinimumSize(640, 480);
    setWindowTitle("Chargement depuis Orthanc");

    // Build the orthanc dialog
    QVBoxLayout* vLayout = new QVBoxLayout();

    m_refreshButton = new PushButton("Actualiser");
    vLayout->addWidget(m_refreshButton);

    m_orthancView = new TreeView();
    m_orthancView->setModel(&m_orthancModel);
    m_orthancView->setIconSize(QSize(32, 32));
    m_orthancView->setIndentation(40);
    vLayout->addWidget(m_orthancView);

    m_progressBar = new ProgressBar();
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(100);
    m_progressBarTimer.setInterval(250);
    vLayout->addWidget(m_progressBar);

    setLayout(vLayout);

    // Connect signals and slots
    connect(m_refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));
    connect(m_orthancView, SIGNAL(doubleClicked(QModelIndex const&)), this, SLOT(checkSelection(QModelIndex const&)));
    connect(this, SIGNAL(seriesSelected(QString const&)), this, SLOT(updateProgressBar()));
    connect(&m_progressBarTimer, SIGNAL(timeout()), this, SLOT(updateProgressBar()));
}

// Destructor
OrthancDialog::~OrthancDialog()
{
    if(m_loadSeriesThread != 0)
        delete m_loadSeriesThread;
}

// The 'connectOrthanc' slot
void OrthancDialog::connectOrthanc()
{
    if(m_orthancConnectionDialog->orthanc() == 0)
        throw OrthancClient::OrthancClientException("");

    if(m_loadSeriesThread != 0)
        delete m_loadSeriesThread;

    m_loadSeriesThread = new LoadSeriesThread(*(m_orthancConnectionDialog->orthanc()));
    connect(this, SIGNAL(seriesSelected(QString const&)), m_loadSeriesThread, SLOT(load(QString const&)));
    connect(m_loadSeriesThread, SIGNAL(seriesLoaded(SeriesData*)), this, SLOT(sendLoadedSeries(SeriesData*)));

    refresh();
}

// The 'checkSelection' slot
void OrthancDialog::checkSelection(QModelIndex const& index)
{
    QVariant selection = m_orthancModel.itemFromIndex(index)->data();
    if(!selection.isNull())
        accept();
}

// The 'accept' slot
void OrthancDialog::accept()
{
    QItemSelectionModel* selectionModel = m_orthancView->selectionModel();
    QModelIndex selIndex = selectionModel->currentIndex();

    QVariant selection;
    if(selIndex.isValid())
        selection = m_orthancModel.itemFromIndex(selIndex)->data();

    if(!selIndex.isValid() || selection.isNull())
        QMessageBox::warning(this, "Aucune série sélectionnée", "Vous devez sélectionner une série avant de valider !");
    else
    {
        setEnabled(false);
        ViewerWindow::instance()->changeStatusBarMessage("Chargement de la série en cours...", 0);
        emit seriesSelected(selection.toString());
    }
}

// The 'sendLoadedSeries' slot
void OrthancDialog::sendLoadedSeries(SeriesData* series)
{
    setEnabled(true);
    ViewerWindow::instance()->clearStatusBarMessage();
    if(series != 0)
    {
        emit seriesLoaded(series);
        OkCancelDialog::accept();
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Orthanc n'a pas pu charger cette série !");
    }
}

// The 'updateProgressBar' slot
void OrthancDialog::updateProgressBar()
{
    m_progressBar->setValue(ceil(m_loadSeriesThread->progressValue()*100));
    if(m_progressBar->value() < 100)
        m_progressBarTimer.start();
    else
        m_progressBar->setValue(0);
}

// The 'refresh' slot
void OrthancDialog::refresh()
{
    if(m_orthancConnectionDialog->orthanc() == 0)
        return;

    m_orthancConnectionDialog->orthanc()->Reload();
    constructOrthancModel();
    repaint();
}

// The 'constructOrthancModel' private method
void OrthancDialog::constructOrthancModel()
{
    m_orthancModel.clear();
    if(m_orthancConnectionDialog->orthanc() == 0)
        return;

    // Run through patient list
    for(unsigned int i = 0 ; i < m_orthancConnectionDialog->orthanc()->GetPatientCount() ; i++)
    {
        OrthancClient::Patient patient = m_orthancConnectionDialog->orthanc()->GetPatient(i);

        QString id = patient.GetMainDicomTag("PatientID", "?").c_str();
        QString name = patient.GetMainDicomTag("PatientName", "?").c_str();

        StandardItem* patientItem = new StandardItem(QString("%1 - %2").arg(id).arg(name), StandardItem::PATIENT);
        patientItem->setFlags(patientItem->flags() & ~Qt::ItemIsEditable);
        m_orthancModel.appendRow(patientItem);

        // Run through study list
        for(unsigned int j = 0 ; j < patient.GetStudyCount() ; j++)
        {
            OrthancClient::Study study = patient.GetStudy(j);

            QString date = study.GetMainDicomTag("StudyDate", "").c_str();
            QString name = study.GetMainDicomTag("StudyDescription", "?").c_str();

            StandardItem* studyItem = new StandardItem(QString("%1 - %2").arg(date).arg(name), StandardItem::STUDY);
            studyItem->setFlags(studyItem->flags() & ~Qt::ItemIsEditable);
            patientItem->appendRow(studyItem);

            // Run through series list
            for(unsigned int k = 0 ; k < study.GetSeriesCount() ; k++)
            {
                OrthancClient::Series series = study.GetSeries(k);
                QString name = series.GetMainDicomTag("SeriesDescription", "?").c_str();

                if(series.Is3DImage())
                {
                    StandardItem* seriesItem = new StandardItem(QString("%1 (%2)").arg(name).arg(series.GetInstanceCount()), StandardItem::SERIES);
                    seriesItem->setFlags(seriesItem->flags() & ~Qt::ItemIsEditable);
                    seriesItem->setData(QString(series.GetId().c_str()));
                    studyItem->appendRow(seriesItem);
                }
            }
        }
    }
}
