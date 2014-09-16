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
//! \file MergedSeriesInterface.h
//! \brief The MergedSeriesInterface.h file contains the interface of the
//!        MergedSeriesInterface class and the definitions of its inline
//!        methods.
//!
//! \author Quentin Smetz
//!

#ifndef MERGEDSERIESINTERFACE_H
#define MERGEDSERIESINTERFACE_H

#include <iostream>
#include <vector>

#include "View/Qt/customwidget/ComboBox.h"

#include "View/Qt/DoubleSlider.h"
#include "View/VTK/MergedSeriesSliceViewer.h"
#include "View/VTK/MergedSeriesVolumeViewer.h"
#include "Controller/SeriesInterface.h"

//!
//! \brief The MergedSeriesInterface class is a widget which contains all the
//!        components to ensure and custom the visualization of merged DICOM
//!        series.
//!
//! To be valid, a MergedSeriesInterface object must use valid SeriesInterface
//! objects.
//!
class MergedSeriesInterface : public DisplayInterface
{
    Q_OBJECT

    public:
        //!
        //! \brief The MergedSeriesInterface constructor initializes the toolbar
        //!        and viewers (interface).
        //!
        //! \param title A QString object which contains the interface title.
        //! \param parent A pointer to the QWidget parent of the intereface.
        //!
        MergedSeriesInterface(QString const& title, QWidget* parent = 0);

        //!
        //! \brief The MergedSeriesInterface destructor.
        //!
        ~MergedSeriesInterface();

        //!
        //! \brief The addSeriesInterface method associates a SeriesInterface
        //!        to the fusion (current interface) and combines its viewers
        //!        accordingly.
        //!
        //! \param interface A pointer to the SeriesInterface to add to the fusion.
        //!
        //! \return Nothing.
        //!
        void addSeriesInterface(SeriesInterface* interface);

        //!
        //! \brief The removeSeriesInterface method extract a SeriesInterface
        //!        from the fusion (current interface) (and its viewers).
        //!
        //! \param interface A pointer to the SeriesInterface to remove from
        //!                  the fusion.
        //! \param checkIfContains A boolean to indicate if the method must check
        //!                        if the fusion really contains what is to remove.
        //!
        //! \return Nothing.
        //!
        void removeSeriesInterface(SeriesInterface* interface, bool checkIfContains = true);

        //!
        //! \brief The removeAllSeriesInterface method extract all SeriesInterface
        //!        from the fusion (current interface) (and its viewers).
        //!
        //! \return Nothing.
        //!
        void removeAllSeriesInterface();

        //!
        //! \brief The containsSeriesInterface method indicates if the merged
        //!        series interface contains the given series interface.
        //!
        //! \param interface A pointer to the series interface to search for.
        //!
        //! \return A boolean which is true if the interface is already part of
        //!         the fusion and false if not.
        //!
        bool containsSeriesInterface(SeriesInterface const* interface) const;

    public slots:
        //!
        //! \brief The updateToolBarForSeries method adds the custom actions
        //!        of the given Series Interface to the toolbar, after having
        //!        removed the previously added ones (if any).
        //!
        //! \param index The index of the SeriesInterface from which to take the
        //!              custom actions.
        //!
        //! \return Nothing.
        //!
        void updateToolBarForSeries(int index);

        //!
        //! \brief The changeCurrentSeriesOpacity method sets the opacity of the
        //!        current selected series (within the fusion).
        //!
        //! \param opacity The new opacity for the current series.
        //!
        //! \return Nothing.
        //!
        void changeCurrentSeriesOpacity(double opacity);

    private:
        std::vector<SeriesInterface*> m_seriesInterfaces;
        customwidget::ComboBox* m_seriesSelector;
        DoubleSlider* m_opacitySlider;

        int m_lastSelectedSeriesIndex;
};

#endif
