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
//! \file SeriesData.h
//! \brief The SeriesData.h file contains the interface of the SeriesData class
//!        and the definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef SERIESDATA_H
#define SERIESDATA_H

#include <vector>
#include <string>

#include <vtkImageData.h>

#include "Range.h"

//!
//! \brief The SeriesData class acts as a vtkImageData on which some information
//!        can be added or computed.
//!
//! This is an extension of the vtkImageData for a DICOM series.
//!
class SeriesData : public vtkImageData
{
    public:
        //!
        //! \brief The SeriesData constructor initializes an empty series with
        //!        a rescale intercept equal to zero and a slope equal to 1.
        //!
        SeriesData();

        //!
        //! \brief The SeriesData destructor.
        //!
        ~SeriesData();

        //!
        //! \brief The patientName method returns the patient name in a QString
        //!        format.
        //!
        //! The method is inline.
        //! The patient name is an empty string if it was not previously set.
        //!
        //! \return A QString object which contains the patient name.
        //!
        inline std::string patientName() const;

        //!
        //! \brief The setPatientName method sets the patient name.
        //!
        //! The method is inline.
        //!
        //! \param patientName A QString object which contains the new patient
        //!                    name.
        //!
        //! \return Nothing.
        //!
        inline void setPatientName(std::string const& patientName);

        //!
        //! \brief The studyDescription method returns the study description in
        //!        a QString format.
        //!
        //! The method is inline.
        //! The study description is an empty string if it was not previously set.
        //!
        //! \return A QString object which contains the study description.
        //!
        inline std::string studyDescription() const;

        //!
        //! \brief The setStudyDescription method sets the study description.
        //!
        //! The method is inline.
        //!
        //! \param studyDesc A QString object which contains the new study
        //!                  description.
        //!
        //! \return Nothing.
        //!
        inline void setStudyDescription(std::string const& studyDesc);

        //!
        //! \brief The seriesDescription method returns the series description in
        //!        a QString format.
        //!
        //! The method is inline.
        //! The series description is an empty string if it was not previously set.
        //!
        //! \return A QString object which contains the series description.
        //!
        inline std::string seriesDescription() const;

        //!
        //! \brief The setSeriesDescription method sets the series description.
        //!
        //! The method is inline.
        //!
        //! \param seriesDesc A QString object which contains the new series
        //!                   description.
        //!
        //! \return Nothing.
        //!
        inline void setSeriesDescription(std::string const& seriesDesc);

        //!
        //! \brief The modality method returns the series modality in
        //!        a QString format.
        //!
        //! The method is inline.
        //! The modality is the "?" string if it had not been set before.
        //!
        //! \return A QString object which contains the modality.
        //!
        inline std::string modality() const;

        //!
        //! \brief The setModality method sets the series modality.
        //!
        //! The method is inline.
        //!
        //! \param mod A QString object which contains the new series modality.
        //!
        //! \return Nothing.
        //!
        inline void setModality(std::string const& mod);

        //!
        //! \brief The setRescaleInterceptAndSlope method sets the rescale
        //!        intercept and slope and recomputes the internal windows
        //!        centers and widths.
        //!
        //! \param intercept The new rescale intercept.
        //! \param slope The new rescale slope.
        //!
        //! \return Nothing.
        //!
        void setRescaleInterceptAndSlope(double intercept, double slope);

        //!
        //! \brief The addBasicWindow method checks the scalar range of the 3D
        //!        image and adds a hounsfield window that covers it.
        //!
        //! \return Nothing.
        //!
        void addBasicWindow();

        //!
        //! \brief The addBasicWindow method adds a hounsfield window for the
        //!        series.
        //!
        //! \param center The center hu value of the new window.
        //! \param width The width in hu values of the new window.
        //!
        //! \return Nothing.
        //!
        void addBasicWindow(double center, double width);

        //!
        //! \brief The convertToHU method converts an internal series value in
        //!        hounsfield value by using the stored intercept and slope
        //!        values.
        //!
        //! \param value The value to convert in hounsfield units.
        //! \param isSize A boolean which is true if the value is a size.
        //!
        //! \return The value converted in hounsfield units.
        //!
        double convertToHU(double value, bool isSize = false) const;

        //!
        //! \brief The convertFromHU method converts a hounsfield unit value in
        //!        an internal series value by using the stored intercept and
        //!        slope values.
        //!
        //! \param value The value to convert from hounsfield units.
        //! \param isSize A boolean which is true if the value is a size.
        //!
        //! \return The hounsfield unit converted in an internal series value.
        //!
        double convertFromHU(double value, bool isSize = false) const;

        //!
        //! \brief The computeBasicHounsfieldRanges calculates the hounsfield
        //!        ranges the DICOM series contains (on basis of the windows
        //!        and levels) and returns them.
        //!
        //! \return A vector which contains the hounsfield ranges the DICOM
        //!         series indicates.
        //!
        std::vector<Range> computeBasicHounsfieldRanges() const;

        //!
        //! \brief The getBasicHounsfield method returns the common hounsfield
        //!        range of the DICOM series.
        //!
        //! If the DICOM series indicates some hounsfield ranges, the first one
        //! is returned. If none is given, a range which covers the whole
        //! hounsfield values in DICOM series is returned.
        //!
        //! \return The common hounsfield range of the DICOM series (see
        //!         description for more details).
        //!
        Range getBasicHounsfield() const;

    private:
        //!
        //! \brief The SeriesData copy constructor is set as private to block
        //!        the possibility to copy a series data.
        //!
        //! \param seriesData The SeriesData object to copy.
        //!
        SeriesData(SeriesData const& seriesData);

        //!
        //! \brief The operator= method is set as private to block
        //!        the possibility to copy a series data.
        //!
        //! \param seriesData The SeriesData object to copy.
        //!
        //! \return A reference to the current series data.
        //!
        SeriesData& operator=(SeriesData const& SeriesData);

        // Attributs
        std::string m_patientName, m_studyDesc, m_seriesDesc, m_modality;
        double m_rescaleIntercept, m_rescaleSlope;
        std::vector<double> m_basicWindowCenters, m_basicWindowWidths;
};

// The 'patientName' method
inline std::string SeriesData::patientName() const { return m_patientName; }

// The 'setPatientName' method
inline void SeriesData::setPatientName(std::string const& patientName)
{ m_patientName = patientName; }

// The 'studyDescription' method
inline std::string SeriesData::studyDescription() const { return m_studyDesc; }

// The 'setStudyDescription' method
inline void SeriesData::setStudyDescription(std::string const& studyDesc)
{ m_studyDesc = studyDesc; }

// The 'seriesDescription' method
inline std::string SeriesData::seriesDescription() const { return m_seriesDesc; }

// The 'setSeriesDescription' method
inline void SeriesData::setSeriesDescription(std::string const& seriesDesc)
{ m_seriesDesc = seriesDesc; }

// The 'modality' method
inline std::string SeriesData::modality() const { return m_modality; }

// The 'setModality' method
inline void SeriesData::setModality(std::string const& mod) { m_modality = mod; }

#endif
