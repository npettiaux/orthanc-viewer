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
//! \file LoadSeriesThread.cpp
//! \brief The LoadSeriesThread.cpp file contains the definition of
//!        non-inline methods of the LoadSeriesThread class.
//!
//! \author Quentin Smetz
//!

#include "LoadSeriesThread.h"
using namespace std;

// Constructor
LoadSeriesThread::LoadSeriesThread(OrthancClient::OrthancConnection& orthanc)
    : QThread(), m_orthanc(orthanc), m_seriesToLoadId(""), m_progressValue(0.0)
{}

// Destructor
LoadSeriesThread::~LoadSeriesThread()
{}

// The 'load' slot
void LoadSeriesThread::load(QString const& seriesId)
{
    m_seriesToLoadId = seriesId;
    start();
}

// The 'run' protected method
void LoadSeriesThread::run()
{
    if(m_seriesToLoadId.isEmpty())
        return;

    // Load the series data from Orthanc
    OrthancClient::Series series(m_orthanc, m_seriesToLoadId.toStdString());

    int w = series.GetWidth(), h = series.GetHeight(), nbInst = series.GetInstanceCount();
    if(w != 0 && h != 0 && nbInst != 0)
    {
        // If the series is valid, prepare a series data
        vtkSmartPointer<SeriesData> seriesData(new SeriesData());
        seriesData->SetDimensions(w, h, nbInst);
        seriesData->SetScalarType(VTK_UNSIGNED_SHORT);
        seriesData->AllocateScalars();

        // Load 3D image from Orthanc
        try{
            series.Load3DImage(seriesData->GetScalarPointer(0, 0, 0),
                               Orthanc::PixelFormat_SignedGrayscale16, 2*w, 2*w*h, &m_progressValue);
        }
        catch(OrthancClient::OrthancClientException& e)
        {
            cerr << e.What() << endl;
            m_seriesToLoadId = "";
            emit seriesLoaded(0);
            return;
        }

        // Load other information
        OrthancClient::Instance instance = series.GetInstance(0);

        instance.LoadTagContent("0010-0010"); // Patient name
        seriesData->setPatientName(instance.GetLoadedTagContent()); // TODO encoding problem
        instance.LoadTagContent("0008-1030"); // Study description
        seriesData->setStudyDescription(instance.GetLoadedTagContent());
        instance.LoadTagContent("0008-103E"); // Series description
        seriesData->setSeriesDescription(instance.GetLoadedTagContent());

        // Voxel size and spacing
        seriesData->SetSpacing(series.GetVoxelSizeX(), series.GetVoxelSizeY(),
                               computeSliceSpacing(series));

        try {
            instance.LoadTagContent("0008-0060"); // Modality
            seriesData->setModality(instance.GetLoadedTagContent());
        }
        catch(exception& e)
        {}

        try {
            instance.LoadTagContent("0028-1052"); // Rescale intercept
            double intercept = QString(instance.GetLoadedTagContent().c_str()).toDouble();
            instance.LoadTagContent("0028-1053"); // Rescale slope
            double slope = QString(instance.GetLoadedTagContent().c_str()).toDouble();
            seriesData->setRescaleInterceptAndSlope(intercept, slope);
        }
        catch(exception& e)
        {}

        try {
            seriesData->addBasicWindow();

            instance.LoadTagContent("0028-1050"); // Window center
            QString centersString = instance.GetLoadedTagContent().c_str();
            instance.LoadTagContent("0028-1051"); // Window width
            QString widthsString = instance.GetLoadedTagContent().c_str();

            QStringList centers = centersString.split("\\");
            QStringList widths = widthsString.split("\\");

            if(centers.size() == widths.size())
            {
                for(int i = 0 ; i < centers.size() ; i++)
                    seriesData->addBasicWindow(centers.at(i).toDouble(),
                                               widths.at(i).toDouble());
            }
        }
        catch(exception& e)
        {}

        // Signal the data...
        SeriesData* data = seriesData.GetPointer();
        seriesData = 0;
        // ... after having transformed it
        instance.LoadTagContent("0020-0037");
        QString or1 = instance.GetLoadedTagContent().c_str();
        QStringList or1s = or1.split("\\");
        Vector3D v(or1s.at(0).toDouble(), or1s.at(1).toDouble(), or1s.at(2).toDouble());
        Vector3D w(or1s.at(3).toDouble(), or1s.at(4).toDouble(), or1s.at(5).toDouble());
        Vector3D cross = v.crossProduct(w);
        vtkSmartPointer<vtkImageReslice> transform = vtkSmartPointer<vtkImageReslice>::New();
        transform->SetInput(data);
        transform->SetResliceAxesDirectionCosines(v.x(), v.y(), v.z(),
                                                  w.x(), w.y(), w.z(),
                                                  cross.x(), cross.y(), cross.z());
        transform->Update();
        data->DeepCopy(transform->GetOutput());
        emit seriesLoaded(data);
    }

    m_seriesToLoadId = "";
}

// The 'computeSliceSpacing' method
double LoadSeriesThread::computeSliceSpacing(OrthancClient::Series& series) const
{
    OrthancClient::Instance instance = series.GetInstance(0);
    instance.LoadTagContent("0020-0032"); // ImagePositionPatient
    QString pos1 = instance.GetLoadedTagContent().c_str();
    QStringList pos1s = pos1.split("\\");
    Vector3D pos1vector(pos1s.at(0).toDouble(), pos1s.at(1).toDouble(),
                        pos1s.at(2).toDouble());

    instance.LoadTagContent("0020-0037"); // ImageOrientationPatient
    QString or1 = instance.GetLoadedTagContent().c_str();
    QStringList or1s = or1.split("\\");
    Vector3D v(or1s.at(0).toDouble(), or1s.at(1).toDouble(), or1s.at(2).toDouble());
    Vector3D w(or1s.at(3).toDouble(), or1s.at(4).toDouble(), or1s.at(5).toDouble());

    Vector3D n = v.crossProduct(w);
    n.normalize();

    double min = -1;
    for(unsigned int i = 1 ; i < series.GetInstanceCount() ; i++) // TODO normally one can be enough but some bug forced me to search for the minest
    {
        OrthancClient::Instance instance2 = series.GetInstance(i);
        instance2.LoadTagContent("0020-0032");
        QString pos2 = instance2.GetLoadedTagContent().c_str();
        QStringList pos2s = pos2.split("\\");
        Vector3D pos2vector(pos2s.at(0).toDouble(), pos2s.at(1).toDouble(),
                            pos2s.at(2).toDouble());

        double dist = fabs(n.dotProduct(pos1vector-pos2vector));
        if(dist < min || min < 0)
            min = dist;
    }

    return min;
}
