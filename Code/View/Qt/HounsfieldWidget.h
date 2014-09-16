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
//! \file HounsfieldWidget.h
//! \brief The HounsfieldWidget.h file contains the interface of the
//!        HounsfieldWidget class and the definitions of its inline
//!        methods.
//!
//! \author Quentin Smetz
//!

#ifndef HOUNSFIELDWIDGET_H
#define HOUNSFIELDWIDGET_H

#include <iostream>

#include <QBoxLayout>

#include "View/Qt/customwidget/CheckBox.h"
#include "View/Qt/customwidget/ComboBox.h"
#include "View/Qt/customwidget/PushButton.h"
#include "View/Qt/customwidget/DoubleSpinBox.h"
#include "View/Qt/customwidget/Widget.h"

#include "Model/Range.h"

#include "View/Qt/DoubleSlider.h"

//!
//! \brief The HounsfieldWidget class regroups a collection of widgets which can
//!        control the modifications of the hounsfield window.
//!
class HounsfieldWidget : public customwidget::Widget
{
    Q_OBJECT

    public:
        //!
        //! \brief The HounsfieldWidget constructor initializes the widget
        //!        components and event connections.
        //!
        //! \param ranges The DICOM custom ranges for hounsfield values.
        //! \param range The initial hounsfield values.
        //! \param parent The QWidget parent of this widget.
        //!
        HounsfieldWidget(std::vector<Range> const& ranges, Range const& range, QWidget* parent = 0);

        //!
        //! \brief The HounsfieldWidget destructor.
        //!
        ~HounsfieldWidget();

        //!
        //! \brief The getHounsfield method returns the current hounsfield
        //!        values.
        //!
        //! The method is inline.
        //!
        //! \return The current hounsfield value (Range object).
        //!
        inline Range const& getHounsfield() const;

        //!
        //! \brief The setHounsfield method change the internal hounsfield
        //!        window according to the given one.
        //!
        //! Dialog components are adapted to the new hounsfield range.
        //!
        //! \param hounsfield The new hounsfield range.
        //!
        //! \return Nothing.
        //!
        void setHounsfield(Range const& hounsfield);

        //!
        //! \brief The getMaximumRange method returns the boundaries of the
        //!        hounsfield values the dialog can accept.
        //!
        //! \return A Range object which contains the minimum and maximum values
        //!         a hounsfield value can take in the dialog.
        //!
        Range getMaximumRange();

    public slots:
        //!
        //! \brief The updateHounsfieldMin slot checks the state of the widget
        //!        components to load the hounsfield minimum value and update
        //!        other components if necessary.
        //!
        //! After the minimum value has been loaded, the widgets which care for
        //! the maximum values are changed to respect the current constraints
        //! (min < max) and constant window size if asked.
        //!
        //! \section emit
        //! The newValues() signal is emitted.
        //!
        //! \return Nothing.
        //!
        void updateHounsfieldMin();

        //!
        //! \brief The updateHounsfieldMax slot checks the state of the widget
        //!        components to load the hounsfield maximum value and update
        //!        other components if necessary.
        //!
        //! After the maximum value has been loaded, the widgets which care for
        //! the minimum values are changed to respect the current constraints
        //! (min < max) and constant window size if asked.
        //!
        //! \section emit
        //! The newValues() signal is emitted.
        //!
        //! \return Nothing.
        //!
        void updateHounsfieldMax();

        //!
        //! \brief The loadPreset slot loads the asked presets by updating the
        //!        widget components (corresponding signals will be emitted).
        //!
        //! \section emit
        //! The newValues() signal is indirectly emitted.
        //!
        //! \param index The index of the preset to load (if 0, Nothing
        //!              happens).
        //!
        //! \return Nothing.
        //!
        void loadPreset(int index);

    signals:
        //!
        //! \brief The newValues signal, once emitted, indicates that new
        //!        hounsfield values have been set.
        //!
        void newValues();

    protected:
        //!
        //! \brief The setMinimum method changes the minimum values a hounsfield
        //!        value can take in the dialog.
        //!
        //! \param min The new minimum value for a hounsfield value in the dialog.
        //!
        //! \return Nothing.
        //!
        void setMinimum(double min);

        //!
        //! \brief The setMaximum method changes the maximum values a hounsfield
        //!        value can take in the dialog.
        //!
        //! \param max The new maximum value for a hounsfield value in the dialog.
        //!
        //! \return Nothing.
        //!
        void setMaximum(double max);

    private:
        // Current and previous state
        Range m_hounsfield;
        unsigned int m_presetNo;

        // Components
        customwidget::DoubleSpinBox *m_minSpinBox, *m_maxSpinBox;
        DoubleSlider *m_minSlider, *m_maxSlider;
        customwidget::CheckBox* m_keepWindowWidthCheckBox;
        bool m_keepWindowIfAsked;
        customwidget::ComboBox* m_presetsComboBox;
        std::vector<Range> m_presets;
};

// The 'getHounsfield' method
inline Range const& HounsfieldWidget::getHounsfield() const { return m_hounsfield; }

#endif
