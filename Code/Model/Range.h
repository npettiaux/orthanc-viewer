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
//! \file Range.h
//! \brief The Range.h file contains the interface of the Range class and the
//!        definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef RANGE_H
#define RANGE_H

#include <cmath>
#include <string>
#include <sstream>

//!
//! \brief The Range class represents a range, i.e. a minimum and a maximum
//!        value.
//!
class Range
{
    public:
        //!
        //! \brief The Range constructor initializes a range between zero
        //!        and zero.
        //!
        Range();

        //!
        //! \brief The Range constructor initializes a range between two
        //!        specified values.
        //!
        //! \param min The minimum value of the range.
        //! \param max The maximum value of the range.
        //!
        Range(double min, double max);

        //!
        //! \brief The Range destructor.
        //!
        ~Range();

        //!
        //! \brief The operator== method returns true if range are equal (with
        //!        regards to equality margin).
        //!
        //! \param range The range to compare with.
        //!
        //! \return true if the range are equal (with regards to equality
        //!         margin) and false if not.
        //!
        bool operator==(Range const& range) const;

        //!
        //! \brief The toString method returns the text representation of the
        //!        range.
        //!
        //! The text representation of the range is : [min, max]
        //!
        //! \return A QString object which contains the text representation of
        //!         the range.
        //!
        std::string toString() const;

        //!
        //! \brief The min method (used in a constant way) returns a copy of
        //!        the minimum value of the range.
        //!
        //! The method is inline.
        //!
        //! \return The minimum value of the range.
        //!
        inline double min() const;

        //!
        //! \brief The min method returns a reference to the minimum value of
        //!        the range.
        //!
        //! The method is inline.
        //!
        //! \return A reference to the minimum value of the range.
        //!
        inline double& min();

        //!
        //! \brief The max method (used in a constant way) returns a copy of
        //!        the maximum value of the range.
        //!
        //! The method is inline.
        //!
        //! \return The maximum value of the range.
        //!
        inline double max() const;

        //!
        //! \brief The max method returns a reference to the maximum value of
        //!        the range.
        //!
        //! The method is inline.
        //!
        //! \return A reference to the maximum value of the range.
        //!
        inline double& max();

        //!
        //! \brief The size method returns the length of the range. The length
        //!        is defined as the difference between the maximum and the
        //!        minimum values.
        //!
        //! The method is inline.
        //!
        //! \return The length of the range.
        //!
        inline double size() const;

        //!
        //! \brief The bound method checks if a value is inside the range and
        //!        returns it if yes and returns the overlapped boundary if not.
        //!
        //! \param value The value to check.
        //!
        //! \return The value in parameter if it is inside the range or the
        //!         overlapped value if not.
        //!
        double bound(double value) const;

        //!
        //! \brief The contains method checks if a value is inside the range.
        //!
        //! \param value The value to check.
        //!
        //! \return True if the value is inside the range and false if not.
        //!
        bool contains(double value) const;

        //!
        //! \brief The relative method returns the relative position of a value
        //!        inside the range.
        //!
        //! The relative position is by default bounded by the range [0, 1].
        //!
        //! \param value The value on which the relative position is computed.
        //! \param mustBound Boolean which is true if the relation position must
        //!                  be bounded by the range [0, 1].
        //!
        //! \return The relative position of value in the range.
        //!
        double relative(double value, bool mustBound = true) const;

        //!
        //! \brief The relative method returns the relative position of a Range
        //!        inside the current range.
        //!
        //! The relative positions are by default bounded by the range [0, 1].
        //!
        //! \param range The range on which the relative positions are computed.
        //! \param mustBound Boolean which is true if the relation positions
        //!                  must be bounded by the range [0, 1].
        //!
        //! \return The relative position of the given range in the current
        //          range.
        //!
        Range relative(Range range, bool mustBound = true) const;

        //!
        //! \brief The absolute method returns the absolute position of a
        //!        relative position (between 0 and 1) inside the range.
        //!
        //! The relative position is by default bounded by the range [0, 1].
        //!
        //! \param value The relative position on which the absolute value is
        //!              computed.
        //! \param mustBound Boolean which is true if the relation position must
        //!                  be bounded by the range [0, 1].
        //!
        //! \return The absolute value of the relative position in the range.
        //!
        double absolute(double value, bool mustBound = true) const;

        //!
        //! \brief The absolute method returns the absolute position of a
        //!        relative range (between 0 and 1) inside the range.
        //!
        //! The relative positions are by default bounded by the range [0, 1].
        //!
        //! \param range The relative range on which the absolute range is
        //!              computed.
        //! \param mustBound Boolean which is true if the relation positions
        //!                  must be bounded by the range [0, 1].
        //!
        //! \return The absolute range of the relative range in the current
        //!         range.
        //!
        Range absolute(Range range, bool mustBound = true) const;

    private:
        double m_min, m_max;

        // EQUAL_PRECISION is the maximal distance between two range values
        // which is considered as a zero.
        static double EQUAL_PRECISION;
};

// The 'min' method
inline double Range::min() const { return m_min; }

// The 'min' method
inline double& Range::min() { return m_min; }

// The 'max' method
inline double Range::max() const { return m_max; }

// The 'max' method
inline double& Range::max() { return m_max; }

// The 'size' method
inline double Range::size() const { return m_max - m_min; }

#endif
