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
//! \file HounsfieldColormapDialog.h
//! \brief The HounsfieldColormapDialog.h file contains the interface of the
//!        HounsfieldColormapDialog class and the definitions of its inline
//!        methods.
//!
//! \author Quentin Smetz
//!

#ifndef HOUNSFIELDCOLORMAPDIALOG_H
#define HOUNSFIELDCOLORMAPDIALOG_H

#include "View/Qt/customwidget/GroupBox.h"

#include "View/Qt/ColormapWidget.h"
#include "View/Qt/HounsfieldWidget.h"
#include "View/Qt/ViewConfigurationDialog.h"

//!
//! \brief The HounsfieldColormapDialog class regroups a ColormapWidget and a
//!        HounsfieldWidget in an OkCancelDialog form.
//!
class HounsfieldColormapDialog : public ViewConfigurationDialog
{
    Q_OBJECT

    public:
        //!
        //! \brief The HounsfieldColormapDialog constructor initializes the
        //!        widgets components and event connections.
        //!
        //! \param ranges The default ranges for hounsfield widget
        //! \param range The current range for hounsfield widget
        //! \param parent The QWidget parent of the dialog
        //!
        HounsfieldColormapDialog(std::vector<Range> const& ranges, Range const& range, QWidget* parent = 0);

        //!
        //! \brief The HounsfieldColormapDialog destructor.
        //!
        ~HounsfieldColormapDialog();

        //!
        //! \brief The getMaximumRange method returns the boundaries of the
        //!        hounsfield values the dialog can accept.
        //!
        //! \return A Range object which contains the minimum and maximum values
        //!         a hounsfield value can take in the dialog.
        //!
        Range getMaximumRange();

    protected:
        //!
        //! \brief The updateComponentsFromCurrentConfiguration method updates
        //!        the dialog components so that it corresponds to the internal
        //!        current view configuration the dialog permits to customize.
        //!
        //! \return Nothing.
        //!
        void updateComponentsFromCurrentConfiguration();

        //!
        //! \brief The updateCurrentConfigurationFromComponents updates the
        //!        internal current configuration the dialog permits to customize
        //!        according to the state of the dialog components.
        //!
        //! \return Nothing.
        //!
        void updateCurrentConfigurationFromComponents();

        //!
        //! \brief The usefulParams method returns a set of ViewParam which
        //!        indicates the useful view parameters the dialog can
        //!        customize.
        //!
        //! The useful parameters are ViewParam::HOUNSFIELD and
        //! ViewParam::COLORMAP.
        //!
        //! \return A set object which contains the ViewParam constants which
        //!         indicates the useful view parameters the dialog can
        //!         customize.
        //!
        std::set<ViewConfiguration::ViewParam> usefulParams() const;

    private:
        HounsfieldWidget* m_hounsfieldWidget; // Hounsfield widget part
        ColormapWidget* m_colormapWidget;     // Colormap widget part
};

#endif
