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
//! \file LoadSeriesThread.h
//! \brief The LoadSeriesThread.h file contains the interface of the
//!        LoadSeriesThread class and the definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef LOADSERIESTHREAD_H
#define LOADSERIESTHREAD_H

#include <iostream>

#include <QStringList>
#include <QThread>

#include <vtkSmartPointer.h>
#include <vtkImageReslice.h>

#include "orthanc/OrthancCppClient.h"

#include "Model/SeriesData.h"
#include "Model/Vector3D.h"

//!
//! \brief The LoadSeriesThread class permits to load a series from Orthanc
//!        server in a dedicated thread.
//!
class LoadSeriesThread : public QThread
{
    Q_OBJECT

    public:
        //!
        //! \brief The LoadSeriesThread constructor links the thread with a
        //!        specified Orthanc connexion.
        //!
        //! \param orthanc A reference to an active OrthancConnection object.
        //!
        LoadSeriesThread(OrthancClient::OrthancConnection& orthanc);

        //!
        //! \brief The LoadSeriesThread destructor.
        //!
        ~LoadSeriesThread();

        //!
        //! \brief The progressValue method returns a reference which permits
        //!        to read the loading progression.
        //!
        //! The method is inline.
        //!
        //! \return A floating point value between 0 and 1 which indicates the
        //!         percentage of progression.
        //!
        inline float const& progressValue() const;

    public slots:
        //!
        //! \brief The load slot launches the loading of a series specified
        //!        by its id.
        //!
        //! \param seriesId A QString object which contains the series id.
        //!
        //! \return Nothing.
        //!
        void load(QString const& seriesId);

    protected:
        //!
        //! \brief The run method loads the series while updating the
        //!        progression value.
        //!
        //! This is an implementation of the QThread method.
        //!
        //! \section emit
        //! The seriesLoaded(SeriesData*) signal is emitted.
        //!
        //! \see void QThread::run()
        //!
        //! \return Nothing.
        //!
        void run();

    signals:
        //!
        //! \brief The seriesLoaded signal, once emitted, give access to a
        //!        loaded series data (pointer).
        //!
        //! \param series A pointer to a SeriesData object which contains
        //!               the series.
        //!
        void seriesLoaded(SeriesData* series);

    protected:
        //!
        //! \brief The computeSliceSpacing method computes the spacing between
        //!        the slices of the given Orthanc series.
        //!
        //! \param series A reference to the Orthanc Series to work on.
        //!
        //! \return The spacing between the series slices.
        //!
        double computeSliceSpacing(OrthancClient::Series& series) const;

    private:
        OrthancClient::OrthancConnection& m_orthanc; // Connexion to Orthanc
        QString m_seriesToLoadId;                    // The series id
        float m_progressValue;                       // The loading progression
};

// The 'progressValue' inline method
inline float const& LoadSeriesThread::progressValue() const
{ return m_progressValue; }

#endif
