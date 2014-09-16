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
//! \file MergedSeriesVolumeViewer.h
//! \brief The MergedSeriesVolumeViewer.h file contains the interface of the
//!        MergedSeriesVolumeViewer class and the definitions of its inline
//!        methods.
//!
//! \author Quentin Smetz
//!

#ifndef MERGEDSERIESVOLUMEVIEWER_H
#define MERGEDSERIESVOLUMEVIEWER_H

#include "MergedSeriesViewer.h"
#include "SeriesVolumeViewer.h"

//!
//! @brief The MergedSeriesVolumeViewer class represents a specific widget to
//!        visualize multiple SeriesData volume objects.
//!
class MergedSeriesVolumeViewer : public MergedSeriesViewer
{
    Q_OBJECT

    public:
        MergedSeriesVolumeViewer();
        ~MergedSeriesVolumeViewer();

    public slots:
        //!
        //! \brief The enableMip slot enable or disable the maximum intensity
        //!        projection mode of the volume(s) rendering.
        //!
        //! \param enable A boolean which is true to enable and false to
        //!               disable the MIP.
        //!
        //! \return Nothing.
        //!
        void enableMip(bool enable);
};

#endif
