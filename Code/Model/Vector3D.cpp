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
//! \file Vector3D.cpp
//! \brief The Vector3D.cpp file contains the definition of non-inline
//!        methods of the Vector3D class.
//!
//! \author Quentin Smetz
//!

#include "Vector3D.h"
using namespace std;

// Constructor
Vector3D::Vector3D(double x, double y, double z) : m_x(x), m_y(y), m_z(z)
{}

// Destructor
Vector3D::~Vector3D()
{}

// The 'dotProduct' method
double Vector3D::dotProduct(Vector3D const& vector) const
{
    return m_x*vector.m_x + m_y*vector.m_y + m_z*vector.m_z;
}

// The 'crossProduct' method
Vector3D Vector3D::crossProduct(Vector3D const& vector) const
{
    return Vector3D(m_y*vector.m_z - m_z*vector.m_y,
                    m_z*vector.m_x - m_x*vector.m_z,
                    m_x*vector.m_y - m_y*vector.m_x);
}

// The 'norm' method
double Vector3D::norm() const
{
    return sqrt(m_x*m_x + m_y*m_y + m_z*m_z);
}

// The 'normalize' method
void Vector3D::normalize()
{
    double n = norm();
    m_x /= n;
    m_y /= n;
    m_z /= n;
}

// The 'operator+=' method
Vector3D& Vector3D::operator+=(Vector3D const& vector)
{
    m_x += vector.m_x;
    m_y += vector.m_y;
    m_z += vector.m_z;

    return *this;
}

// The 'operator-=' method
Vector3D& Vector3D::operator-=(Vector3D const& vector)
{
    m_x -= vector.m_x;
    m_y -= vector.m_y;
    m_z -= vector.m_z;

    return *this;
}

// The 'operator+' method
Vector3D operator+(Vector3D const& v1, Vector3D const& v2)
{
    Vector3D result(v1);
    result += v2;
    return result;
}

// The 'operator-' method
Vector3D operator-(Vector3D const& v1, Vector3D const& v2)
{
    Vector3D result(v1);
    result -= v2;
    return result;
}
