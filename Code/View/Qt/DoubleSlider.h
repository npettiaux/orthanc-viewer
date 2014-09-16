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
//! \file DoubleSlider.h
//! \brief The DoubleSlider.h file contains the interface of the
//!        DoubleSlider class and the definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef DOUBLESLIDER_H
#define DOUBLESLIDER_H

#include "View/Qt/customwidget/Slider.h"

#include "Model/Range.h"

//!
//! \brief The DoubleSlider class provides an improved Slider which can indicates
//!        and signal double values instead of integer values.
//!
class DoubleSlider : public customwidget::Slider
{
    Q_OBJECT

    public:
        //!
        //! \brief The DoubleSlider constructor initializes the Slider according
        //!        to the given orientation.
        //!
        //! The default double range is set as [0, 1] with 0.1 precision.
        //!
        //! \param orientation The Slider orientation.
        //! \param parent A pointer to the QWidget parent of the slider.
        //!
        DoubleSlider(Qt::Orientation orientation, QWidget* parent = 0);

        //!
        //! \brief The DoubleSlider destructor.
        //!
        ~DoubleSlider();

        //!
        //! \brief The doubleRange method returns the current double range the
        //!        slider represents.
        //!
        //! The method is inline.
        //!
        //! \return The double range the slider represents.
        //!
        inline Range const& doubleRange() const;

        //!
        //! \brief The setDoubleRange method sets the range of double values the
        //!        slider controls with the specified precision.
        //!
        //! \param doubleRange The new range of double values.
        //! \param precision The minimum double increment which can be done by
        //!                  moving the slider cursor. Notice that the effective
        //!                  precision may be a little smaller (better) to match
        //!                  the asked double range.
        //!
        //! \return Nothing.
        //!
        void setDoubleRange(Range const& doubleRange, double precision = 1.0);

        //!
        //! \brief The doubleValue method returns the current double value the
        //!        slider indicates.
        //!
        //! \return The double value the slider indicates.
        //!
        double doubleValue() const;

    public slots:
        //!
        //! \brief The setDoubleValue slot changes the slider so that it
        //!        indicates the given double value.
        //!
        //! \param value The new double value the slider must indicates.
        //!
        //! \return Nothing.
        //!
        void setDoubleValue(double value);

        //!
        //! \brief The sendDoubleValue slot emits a signal which indicates
        //!        the current double value the slider indicates.
        //!
        //! \return Nothing.
        //!
        void sendDoubleValue();

    signals:
        //!
        //! \brief The doubleValueChanged signal, once emitted, indicates the
        //!        the new double value of the slider.
        //!
        //! \param value The double value the slider indicates.
        //!
        void doubleValueChanged(double value);

    private:
        Range m_doubleRange;
        double m_precision;
};

// The 'doubleRange' method
inline Range const& DoubleSlider::doubleRange() const { return m_doubleRange; }

#endif
