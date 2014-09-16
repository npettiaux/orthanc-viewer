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
//! \file MergedSeriesViewer.h
//! \brief The MergedSeriesViewer.h file contains the interface of the
//!        MergedSeriesViewer class and the definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef MERGEDSERIESVIEWER_H
#define MERGEDSERIESVIEWER_H

#include <vector>

#include <vtkOpenGLRenderer.h>
#include <vtkImageActor.h>

#include "SeriesViewer.h"

//!
//! \brief The MergedSeriesViewer class represents a specific widget to
//!        visualize multiple SeriesData objects.
//!
//! A MergedSeriesViewer object uses the functionnalities of many SeriesViewer
//! objects to achieve the merging.
//!
class MergedSeriesViewer : public Viewer
{
    Q_OBJECT

    public:
        //!
        //! \brief The MergedSeriesViewer destructor.
        //!
        ~MergedSeriesViewer();

        //!
        //! \brief The linkSeriesViewer method associates a series viewer to the
        //!        viewer to combine it with the previous linked ones.
        //!
        //! \param seriesViewer A pointer to the series viewer to link.
        //!
        //! \return Nothing.
        //!
        virtual void linkSeriesViewer(SeriesViewer* seriesViewer);

        //!
        //! \brief The unlinkSeriesViewer method remove a series viewer from the
        //!        viewer combination it make with the other linked ones.
        //!
        //! \param seriesViewer A pointer to the series viewer to unlink.
        //!
        //! \return Nothing.
        //!
        virtual void unlinkSeriesViewer(SeriesViewer* seriesViewer);

        //!
        //! \brief The minSlice method must return the minimum slice position of the
        //!        series 3D volume(s) which can accessed given the current
        //!        configuration.
        //!
        //! \return The minimum position of a slice of the series 3D volume(s) which
        //!         can be accessed given the current configuration.
        //!
        virtual double minSlice() const;

        //!
        //! \brief The maxSlice method must return the maximum slice position of the
        //!        series 3D volume which can accessed given the current
        //!        configuration.
        //!
        //! \return The maximum position of a slice of the series 3D volume(s) which
        //!         can be accessed given the current configuration.
        //!
        virtual double maxSlice() const;

    protected:
        //!
        //! \brief The MergedSeriesViewer constructor initializes the viewer.
        //!
        //! This is protected as the MergedSeriesViewer class is abstract.
        //!
        MergedSeriesViewer();

        //!
        //! \brief The updateHounsfield method updates the viewer according to
        //!        the hounsfield range specified by the ViewConfiguration.
        //!
        //! This is an implementation of the Viewer::updateHounsfield()
        //! method.
        //!
        //! As the merged viewer take profit of the natural update of the
        //! viewers it combines, this method does nothing but repaint the viewer.
        //!
        //! \param config The ViewConfiguration object to work on.
        //!
        //! \return Nothing.
        //!
        virtual void updateHounsfield(ViewConfiguration const& config);

        //!
        //! \brief The updateColormap method updates the viewer according to
        //!        the colormap specified by the ViewConfiguration.
        //!
        //! This is an implementation of the Viewer::updateColormap()
        //! method.
        //!
        //! As the merged viewer take profit of the natural update of the
        //! viewers it combines, this method does nothing but repaint the viewer.
        //!
        //! \param config The ViewConfiguration object to work on.
        //!
        //! \return Nothing.
        //!
        virtual void updateColormap(ViewConfiguration const& config);

        //!
        //! \brief The updateTranslation method updates the viewer according to
        //!        the translation specified by the ViewConfiguration.
        //!
        //! This is an implementation of the Viewer::updateTranslation()
        //! method.
        //!
        //! As the merged viewer take profit of the natural update of the
        //! viewers it combines, this method does nothing but repaint the viewer.
        //!
        //! \param config The ViewConfiguration object to work on.
        //!
        //! \return Nothing.
        //!
        virtual void updateTranslation(ViewConfiguration const& config);

        //!
        //! \brief The updateRotation method updates the viewer according to
        //!        the rotation specified by the ViewConfiguration.
        //!
        //! This is an implementation of the Viewer::updateRotation()
        //! method.
        //!
        //! As the merged viewer take profit of the natural update of the
        //! viewers it combines, this method does nothing but repaint the viewer.
        //!
        //! \param config The ViewConfiguration object to work on.
        //!
        //! \return Nothing.
        //!
        virtual void updateRotation(ViewConfiguration const& config);

        //! The list of the series viewers that the viewer combines.
        std::vector<SeriesViewer*> m_seriesViewers;
};

#endif
