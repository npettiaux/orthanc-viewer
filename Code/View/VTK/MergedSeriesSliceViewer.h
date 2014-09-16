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
//! \file MergedSeriesSliceViewer.h
//! \brief The MergedSeriesSliceViewer.h file contains the interface of the
//!        MergedSeriesSliceViewer class and the definitions of its inline
//!        methods.
//!
//! \author Quentin Smetz
//!

#ifndef MERGEDSERIESSLICEVIEWER_H
#define MERGEDSERIESSLICEVIEWER_H

#include <vtkCamera.h>
#include <vtkInteractorStyleImage.h>

#include "MergedSeriesViewer.h"
#include "SeriesSliceViewer.h"

//!
//! @brief The MergedSeriesSliceViewer class represents a specific widget to
//!        visualize multiple SeriesData slice objects.
//!
class MergedSeriesSliceViewer : public MergedSeriesViewer
{
    Q_OBJECT

    public:
        //!
        //! \brief The MergedSeriesSliceViewer constructor update camera view
        //!        to visualize the right slice orientation.
        //!
        //! \param orientation The desired slice orientation.
        //!
        MergedSeriesSliceViewer(SliceOrientation orientation);

        //!
        //! \brief The MergedSeriesSliceViewer destructor.
        //!
        ~MergedSeriesSliceViewer();

        //!
        //! \brief The orientation method returns the slice orientation of the
        //!        ones which are visualized by the viewer.
        //!
        //! The method is inline.
        //!
        //! \return The slice orientation the viewer is using.
        //!
        inline SliceOrientation orientation() const;

    public slots:
        //!
        //! \brief The changeCurrentSlice method update the slice which is
        //!        currently visualized.
        //!
        //! \return Nothing.
        //!
        void changeCurrentSlice(double value);

    protected:
        //!
        //! \brief The updateTranslation method updates the viewer according to
        //!        the translation specified by the ViewConfiguration.
        //!
        //! This is an implementation of the MergedSeriesViewer::updateTranslation()
        //! method.
        //!
        //! As the merged viewer take profit of the natural update of the
        //! viewers it combines, this method does nothing but repaint the viewer.
        //! Notice that it also refresh the slice which is visualized as the
        //! translation may have changed it.
        //!
        //! \param config The ViewConfiguration object to work on.
        //!
        //! \return Nothing.
        //!
        void updateTranslation(ViewConfiguration const& config);

    private:
        SliceOrientation m_orientation;
        double m_currentSlice;
};

// The 'orientation' method
inline SliceOrientation MergedSeriesSliceViewer::orientation() const
{ return m_orientation; }

#endif
