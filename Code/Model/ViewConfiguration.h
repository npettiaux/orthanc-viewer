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
//! \file ViewConfiguration.h
//! \brief The ViewConfiguration.h file contains the interface of the
//!        ViewConfiguration class and the definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef VIEWCONFIGURATION_H
#define VIEWCONFIGURATION_H

#include "Model/Colormap.h"
#include "Model/Vector3D.h"

//!
//! \brief The ViewConfiguration class represents the configuration in which a
//!        volume or a slice is visualized.
//!
//! Configuration is composed of hounsfield window, colormap, translation and
//! rotation.
//!
class ViewConfiguration
{
    public:
        //!
        //! \brief The ViewConfiguration constructor.
        //!
        ViewConfiguration();

        //!
        //! \brief The ViewConfiguration destructor.
        //!
        ~ViewConfiguration();

        //!
        //! \brief The ViewParam enum enumerates the parameters that can be
        //!        changed in the visualisation through a Viewer object.
        //!
        enum ViewParam
        {
            ALL, HOUNSFIELD, COLORMAP, TRANSLATION, ROTATION
        };

        //!
        //! \brief The hounsfield method returns the hounsfield range
        //!        of the view configuration.
        //!
        //! The method is inline.
        //!
        //! \return The hounsfield range of the view configuration.
        //!
        inline Range const& hounsfield() const;

        //!
        //! \brief The hounsfieldMaxRange method returns the range which contains
        //!        the boundaries which are imposed on hounsfield values in the
        //!        view configuration.
        //!
        //! The method is inline.
        //!
        //! \return The range which contains the boundaries which are imposed
        //!         on hounsfield values in the view configuration.
        //!
        inline Range const& hounsfieldMaxRange() const;

        //!
        //! \brief The colormap method returns the internal colormap
        //!        of the view configuration.
        //!
        //! The method is inline.
        //!
        //! \return The internal colormap of the view configuration.
        //!
        inline Colormap const& colormap() const;

        //!
        //! \brief The translation method returns the internal translation of
        //!        the view configuration.
        //!
        //! The method is inline.
        //!
        //! \return The internal translation of the view configuration.
        //!
        inline Vector3D const& translation() const;

        //!
        //! \brief The rotation method returns the internal rotation of
        //!        the view configuration.
        //!
        //! The method is inline.
        //!
        //! \return The internal rotation of the view configuration.
        //!
        inline Vector3D const& rotation() const;

        //!
        //! \brief The setHounsfield method changes the hounsfield range of the
        //!        view configuration.
        //!
        //! \param hounsfield The new hounsfield range of the view configuration.
        //! \param hounsfieldMaxRange A range which indicates boundaries on the
        //!                           hounsfield values which can be used.
        //!
        //! \return Nothing.
        //!
        void setHounsfield(Range const& hounsfield, Range const& hounsfieldMaxRange);

        //!
        //! \brief The setColormap method changes the internal colormap of the
        //!        view configuration.
        //!
        //! \param colormap The new colormap of the view configuration.
        //!
        //! \return Nothing.
        //!
        void setColormap(Colormap const& colormap);

        //!
        //! \brief The setTranslation method changes the internal translation of the
        //!        view configuration.
        //!
        //! \param translation The new translation of the view configuration.
        //!
        //! \return Nothing.
        //!
        void setTranslation(Vector3D const& translation);

        //!
        //! \brief The setRotation method changes the internal rotation of the
        //!        view configuration.
        //!
        //! \param rotation The new rotation of the view configuration.
        //!
        //! \return Nothing.
        //!
        void setRotation(Vector3D const& rotation);

    private:
        Range m_hounsfield, m_hounsfieldMaxRange;
        Colormap m_colormap;
        Vector3D m_translation, m_rotation;
};

// The 'hounsfield' method
inline Range const& ViewConfiguration::hounsfield() const { return m_hounsfield; }

// The 'hounsfieldMaxRange' method
inline Range const& ViewConfiguration::hounsfieldMaxRange() const
{ return m_hounsfieldMaxRange; }

// The 'colormap' method
inline Colormap const& ViewConfiguration::colormap() const { return m_colormap; }

// The 'translation' method
inline Vector3D const& ViewConfiguration::translation() const { return m_translation; }

// The 'rotation' method
inline Vector3D const& ViewConfiguration::rotation() const { return m_rotation; }

#endif
