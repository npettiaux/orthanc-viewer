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
//! \file Colormap.h
//! \brief The Colormap.h file contains the interface of the
//!        Colormap class and the definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef COLORMAP_H
#define COLORMAP_H

#include <iostream>
#include <fstream>
#include <vector>

#include <QColor>
#include <QFile>
#include <QTextStream>

#include <vtkColorTransferFunction.h>

#include "Range.h"

//!
//! \brief The Colormap class is a collection of colors associated with a range
//!        between 0 and 1.
//!
//! A colormap object can generate a vtkColorTransferFunction according to two
//! different modes of interpolation.
//!
class Colormap : public QObject
{
    Q_OBJECT

    public:
        //!
        //! \brief The Colormap default constructor initialize a colormap
        //!        without any color and a linear interpolation mode.
        //!
        Colormap();

        //!
        //! \brief The Colormap copy constructor constructs a colormap copy of
        //!        a specific one and emits the corresponding signal.
        //!
        //! \section emit
        //! The signal modified() is emitted after the copy.
        //!
        //! \param copy The colormap to copy
        //!
        Colormap(Colormap const& copy);

        //!
        //! \brief The Colormap destructor.
        //!
        ~Colormap();

        //!
        //! \brief The operator= method modifies the colormap so that it becomes
        //!        a full copy of a given one and emits the corresponding
        //!        signal.
        //!
        //! \section emit
        //! The signal modified() is emitted after the copy.
        //!
        //! \param copy The colormap to copy.
        //!
        //! \return A reference to the colormap
        //!
        Colormap& operator=(Colormap const& copy);

        //!
        //! \brief The InterpolationMode enum regroups a serie of constants to
        //!        identify the different interpolation modes.
        //!
        enum InterpolationMode
        {
            LINEAR, CONSTANT
        };

        //!
        //! \brief The count method returns the number of colors the colormap
        //!        contains.
        //!
        //! The method is inline.
        //!
        //! \return The number of colors the colormap contains.
        //!
        inline unsigned int count() const;

        //!
        //! \brief The startAt method returns the starting position of a color.
        //!
        //! The method is inline.
        //!
        //! \param index The index of the color to access.
        //!
        //! \return The starting position of the color.
        //!
        inline double startAt(unsigned int index) const;

        //!
        //! \brief The colorAt methods returns the color of a specific color in
        //!        the colormap.
        //!
        //! The method is inline.
        //!
        //! \param index The index of the color to access.
        //!
        //! \return A QColor object which indicates the color at the given
        //!         index.
        //!
        inline QColor const& colorAt(unsigned int index) const;

        //!
        //! \brief The setStartAt method change the starting point of a specific
        //!        color in the colormap and emits the corresponding signal.
        //!
        //! \section emit
        //! The signal colorModified(uint, double, QColor) is emitted.
        //!
        //! \param index The index of the color to change.
        //! \param start The new starting point for the color.
        //!
        //! \return Nothing.
        //!
        void setStartAt(unsigned int index, double start);

        //!
        //! \brief The setColorAt method change the color of a specific color
        //!        in the colormap and emits the corresping signal.
        //!
        //! \section emit
        //! The signal colorModified(uint, double, QColor) is emitted.
        //!
        //! \param index The index of the color to change.
        //! \param color The new color for the color item of the colormap.
        //!
        //! \return Nothing.
        //!
        void setColorAt(unsigned int index, QColor const& color);

        //!
        //! \brief The interpolationMode method returns the interpolation mode
        //!        constant which identify how the colormap interpolates its
        //!        colors.
        //!
        //! The method is inline.
        //!
        //! \return A InterpolationMode value which indicates how the colormap
        //!         interpolates its colors.
        //!
        inline InterpolationMode interpolationMode() const;

        //!
        //! \brief The setInterpolationMode method change the way the colormap
        //!        interpolates its colors.
        //!
        //! \section emit
        //! The signal interpolationModeModified(InterpolationMode) is emitted.
        //!
        //! \param interpolationMode A InterpolationMode value which indicates
        //!                          how the colormap interpolates its colors.
        //!
        //! \return Nothing.
        //!
        void setInterpolationMode(InterpolationMode const& interpolationMode);

        //!
        //! \brief The addColor method adds a color to the colormap at the given
        //!        starting point.
        //!
        //! \section emit
        //! The signal colorAdded(uint, double, QColor) is emitted.
        //!
        //! \param start The starting point of the new color.
        //! \param color The color to add in the colormap.
        //!
        //! \return Nothing.
        //!
        void addColor(double start, QColor const& color);

        //!
        //! \brief The loadFromLutFile method reinitialize the colormap and then
        //!        add colors according to a LUT file whose name is given in
        //!        parameter.
        //!
        //! \param filename A QString which contains the name of the LUT file to
        //!                 load.
        //!
        //! \return True if the loading was successful and false if not.
        //!
        bool loadFromLutFile(QString filename);

        //!
        //! \brief The getColorFromPosition method returns the color at the
        //!        given position without taking into account the interpolation
        //!        mode.
        //!
        //! The color black is returned if the given position is before every
        //! color starting point in the colormap. The color white is returned
        //! if there is not any color in the colormap.
        //!
        //! \param position The position (normally in the range [0-1]) of the
        //!                 color to search.
        //!
        //! \return A QColor object which contains the color at the given
        //!         position in the colormap.
        //!
        QColor getColorFromPosition(double position) const;

        //!
        //! \brief The computeVTKColorTransferFunction method returns a vtk
        //!        color transfer function which represents the colormap.
        //!
        //! The color transfer function is mapped onto a specific range
        //! depending on the internal interpolation mode.
        //! If the mode is linear, the colormap is mappped on the 'range
        //! window' parameter and black and white are mapped outside in the
        //! 'range' parameter.
        //! If the mode is constant, the colormap is mapped on the 'range'
        //! parameter. On the 'range window', the intensity of the color is
        //! linearly increasing (between 0 and 1). Before it, color is black and
        //! after it colors are pure.
        //!
        //! \param onRange The 'range' parameter (see description).
        //! \param onRangeWindow The 'range window' parameter (see description).
        //!
        //! \return A memory allocated vtkColorTransferFunction which represents
        //!         the colormap (see description).
        //!
        vtkColorTransferFunction* computeVTKColorTransferFunction
                       (Range const& onRange, Range const& onRangeWindow) const;

        //!
        //! \brief The changeValue static method convert the copy color in
        //!        parameter in the HSV colorspace and change its value with
        //!        the given one.
        //!
        //! \param color A QColor object which contains the color to adjust.
        //! \param value The new 'value' (in HSV parameters) for the color.
        //!
        //! \return The adjusted color (with the new value).
        //!
        static QColor changeValue(QColor const& color, double value);

    public slots:
        //!
        //! \brief The clear slot deletes all the colors the colormap
        //!        contains and emits the corresponding signal.
        //!
        //! \section emit
        //! The signal modified() is emitted.
        //!
        //! \return Nothing.
        //!
        void clear();

    signals:
        //!
        //! \brief The colorAdded signal, once emitted, identify the index of
        //!        a new color and its starting point and color.
        //!
        //! \param index The index (in the colormap) of the added color.
        //! \param start The starting point of the added color.
        //! \param color The color of the added color item.
        //!
        void colorAdded(unsigned int index, double start, QColor const& color);

        //!
        //! \brief The colorModified signal, once emitted, identify the index of
        //!        a modified color and its starting point and color.
        //!
        //! \param index The index (in the colormap) of the modified color.
        //! \param start The starting point of the modified color.
        //! \param color The color of the modified color item.
        //!
        void colorModified(uint index, double start, QColor const& color);

        //!
        //! \brief The interpolationModeModified signal, once emitted, indicates
        //!        the new interpolation mode.
        //!
        //! \param interpMode The new colormap interpolation mode.
        //!
        void interpolationModeModified(InterpolationMode const& interpMode);

        //!
        //! \brief The modified signal, once emitted, indicates the colormap has
        //!        been fully reconstructed.
        //!
        void modified();

    private:
        //!
        //! \brief The addColorToEnd method adds a color to the end of the
        //!        colormap (in the internal container) assuming the given
        //!        starting point is at the end.
        //!
        //! \param start The starting point which is associated to the color.
        //! \param color A QColor object which contains the color to add.
        //!
        //! \return Nothing.
        //!
        void addColorToEnd(double start, QColor const& color);

        std::vector< std::pair<double, QColor> > m_colors; // The list of starting
                                                         // points and colors
        InterpolationMode m_interpolationMode; // The way the colormap
                                               // interpolates its colors
};

// The 'count' method
inline unsigned int Colormap::count() const { return m_colors.size(); }

// The 'startAt' method
inline double Colormap::startAt(unsigned int index) const
{ return m_colors.at(index).first; }

// The 'colorAt' method
inline QColor const& Colormap::colorAt(unsigned int index) const
{ return m_colors.at(index).second; }

// The 'interpolationMode' method
inline Colormap::InterpolationMode Colormap::interpolationMode() const
{ return m_interpolationMode; }

#endif
