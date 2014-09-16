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
//! \file Vector3D.h
//! \brief The Vector3D.h file contains the interface of the
//!        Vector3D class and the definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <cmath>

//!
//! \brief The Vector3D class is convenient to contains information about a 3D
//!        vector.
//!
class Vector3D
{
    public:
        //!
        //! \brief The Vector3D contructor initializes its three components from
        //!        parameters.
        //!
        //! \param x The first component of the 3D vector.
        //! \param y The second component of the 3D vector.
        //! \param z The third component of the 3D vector.
        //!
        Vector3D(double x = 0, double y = 0, double z = 0);

        //!
        //! \brief The Vector3D destructor.
        //!
        ~Vector3D();

        //!
        //! \brief The x method returns the first component of the 3D vector
        //!        (constant method).
        //!
        //! The method is inline.
        //!
        //! \return The first component of the 3D vector.
        //!
        inline double x() const;

        //!
        //! \brief The x method returns the first component of the 3D vector
        //!
        //! The method is inline.
        //!
        //! \return The first component of the 3D vector.
        //!
        inline double& x();

        //!
        //! \brief The y method returns the second component of the 3D vector
        //!        (constant method).
        //!
        //! The method is inline.
        //!
        //! \return The second component of the 3D vector.
        //!
        inline double y() const;

        //!
        //! \brief The y method returns the second component of the 3D vector
        //!
        //! The method is inline.
        //!
        //! \return The second component of the 3D vector.
        //!
        inline double& y();

        //!
        //! \brief The z method returns the third component of the 3D vector
        //!        (constant method).
        //!
        //! The method is inline.
        //!
        //! \return The third component of the 3D vector.
        //!
        inline double z() const;

        //!
        //! \brief The z method returns the third component of the 3D vector
        //!
        //! The method is inline.
        //!
        //! \return The third component of the 3D vector.
        //!
        inline double& z();

        //!
        //! \brief The dotProduct method returns the dot product between the
        //!        current vector and another given vector.
        //!
        //! \param vector The vector with which to compute the dot product.
        //!
        //! \return The dot product between the current vector and the parameter
        //!         vector.
        //!
        double dotProduct(Vector3D const& vector) const;

        //!
        //! \brief The crossProduct method returns the cross product between the
        //!        current vector and another given vector.
        //!
        //! \param vector The vector with which to compute the cross product.
        //!
        //! \return The cross product between the current vector and the parameter
        //!         vector (a new Vector3D object).
        //!
        Vector3D crossProduct(Vector3D const& vector) const;

        //!
        //! \brief The norm method returns the norm of the 3D vector.
        //!
        //! \return The norm of the 3D vector.
        //!
        double norm() const;

        //!
        //! \brief The normalize method transform the vector so that its norm
        //!        becomes equal to one.
        //!
        //! \return Nothing.
        //!
        void normalize();

        //!
        //! \brief The operator+= method adds another vector to the current one.
        //!
        //! \param vector The vector to add to the current vector.
        //!
        //! \return A reference to the current vector.
        //!
        Vector3D& operator+=(Vector3D const& vector);

        //!
        //! \brief The operator-= method substracts another vector from the
        //!        current one.
        //!
        //! \param vector The vector to substract from the current vector.
        //!
        //! \return A reference to the current vector.
        //!
        Vector3D& operator-=(Vector3D const& vector);

    private:
        double m_x, m_y, m_z;
};

// The 'x' method
inline double Vector3D::x() const { return m_x; }

// The 'x' method
inline double& Vector3D::x() { return m_x; }

// The 'y' method
inline double Vector3D::y() const { return m_y; }

// The 'y' method
inline double& Vector3D::y() { return m_y; }

// The 'z' method
inline double Vector3D::z() const { return m_z; }

// The 'z' method
inline double& Vector3D::z() { return m_z; }

//!
//! \brief The operator+ method returns the addition between two vectors.
//!
//! \param v1 The first vector to add.
//! \param v2 The second vector to add.
//!
//! \return A 3D vector which is the addition of the two ones in parameter.
//!
Vector3D operator+(Vector3D const& v1, Vector3D const& v2);

//!
//! \brief The operator- method returns the difference between two vectors.
//!
//! \param v1 The first vector to differentiate.
//! \param v2 The second vector to differentiate.
//!
//! \return A 3D vector which is the difference of the two ones in parameter.
//!
Vector3D operator-(Vector3D const& v1, Vector3D const& v2);

#endif
