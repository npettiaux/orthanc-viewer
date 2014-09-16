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
//! \file VolumeSubInterface.h
//! \brief The VolumeSubInterface.h file contains the interface of the
//!        VolumeSubInterface class and the definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef VOLUMESUBINTERFACE_H
#define VOLUMESUBINTERFACE_H

#include "View/Qt/customwidget/PushButton.h"

#include "View/VTK/SeriesVolumeViewer.h"
#include "View/VTK/MergedSeriesVolumeViewer.h"
#include "Controller/SubInterface.h"

//!
//! \brief The VolumeSubInterface class is a SubInterface which is specialized
//!        for the visualization of a 3D Volume.
//!
class VolumeSubInterface : public SubInterface
{
    public:
        //!
        //! \brief The VolumeSubInterface constructor initializes the components
        //!        and event connections.
        //!
        //! \param series The series whose volume will be visualized.
        //!
        VolumeSubInterface(SeriesData* series);

        //!
        //! \brief The VolumeSubInterface constructor II initializes the
        //!        components and event connections.
        //!
        //! \param viewer A pointer to the merged series volume viewer which will
        //!               visualized merged volumes.
        //!
        VolumeSubInterface(MergedSeriesVolumeViewer* viewer);

        //!
        //! \brief The VolumeSubInterface destructor.
        //!
        ~VolumeSubInterface();

    private:
        void initInterface();

        // Components
        customwidget::PushButton* m_mipButton;
};

#endif
