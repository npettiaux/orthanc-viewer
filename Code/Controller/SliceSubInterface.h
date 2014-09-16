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
//! \file SliceSubInterface.h
//! \brief The SliceSubInterface.h file contains the interface of the
//!        SliceSubInterface class and the definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef SLICESUBINTERFACE_H
#define SLICESUBINTERFACE_H

#include "View/Qt/customwidget/Label.h"
#include "View/Qt/customwidget/PushButton.h"

#include "View/Qt/DoubleSlider.h"
#include "View/VTK/SeriesSliceViewer.h"
#include "View/VTK/MergedSeriesSliceViewer.h"

#include "Controller/SubInterface.h"

//!
//! \brief The SliceSubInterface class is a SubInterface which is specialized
//!        for the visualization of slice of a 3D Volume.
//!
//! A slider is added at the right of the viewer to control the slice which is
//! shown.
//!
class SliceSubInterface : public SubInterface
{
    Q_OBJECT

    public:
        //!
        //! \brief The SliceSubInterface constructor initializes the components
        //!        and event connections.
        //!
        //! \param series The series whose slices will be visualized.
        //! \param orien The orientation of the slices.
        //!
        SliceSubInterface(SeriesData* series, SliceOrientation orien);

        //!
        //! \brief The SliceSubInterface constructor II initializes the
        //!        components and event connections.
        //!
        //! \param viewer A pointer to the merged series slice viewer which will be
        //!               used to visualize merged series slices.
        //!
        SliceSubInterface(MergedSeriesSliceViewer* viewer);

        //!
        //! \brief The SliceSubInterface destructor.
        //!
        ~SliceSubInterface();

        //!
        //! \brief The updateSliceRange method updates the dedicated slice range
        //!        (on which the slice slider integer values are mapped)
        //!        according to the slice range the viewer indicates to be able
        //!        to visualize.
        //!
        //! \return Nothing
        //!
        void updateSliceRange();

    public slots:
        //!
        //! \brief The resetSlider slot makes the slice slider change so that
        //!        it indicates its mid-value.
        //!
        //! \return Nothing
        //!
        void resetSlider();

    private:
        //!
        //! \brief The initInterface private method initializes the interface
        //!        (widget component) and event connections.
        //!
        //! \return Nothing.
        //!
        void initInterface();

        DoubleSlider* m_sliceSlider; // The slider to control the reslice action
        customwidget::PushButton* m_resetSliderButton;
        SliceOrientation m_orientation;
};

#endif
