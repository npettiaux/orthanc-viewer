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
//! \file ColormapWidget.h
//! \brief The ColormapWidget.h file contains the interface of the
//!        ColormapWidget class and the definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef COLORMAPWIDGET_H
#define COLORMAPWIDGET_H

#include <iostream>
#include <map>

#include <QBoxLayout>
#include <QButtonGroup>
#include <QFileDialog>
#include <QMessageBox>
#include <QColorDialog>

#include "View/Qt/customwidget/CheckBox.h"
#include "View/Qt/customwidget/Dial.h"
#include "View/Qt/customwidget/DoubleSpinBox.h"
#include "View/Qt/customwidget/GroupBox.h"
#include "View/Qt/customwidget/PushButton.h"
#include "View/Qt/customwidget/RadioButton.h"
#include "View/Qt/customwidget/Slider.h"
#include "View/Qt/customwidget/Widget.h"

#include "View/Qt/ColorbarWidget.h"
#include "View/Qt/DoubleSlider.h"

//!
//! \brief The ColormapWidget class regroups a collection of widgets which can
//!        control the modifications of an internal colormap.
//!
class ColormapWidget : public customwidget::Widget
{
    Q_OBJECT

    public:
        //!
        //! \brief The ColormapWidget constructor initializes its component and
        //!        event connections with the colormap.
        //!
        //! \param barRange The maximum range on which a colormap can be applied.
        //! \param parent The QWidget parent of the ColormapWidget.
        //!
        ColormapWidget(Range const& barRange, QWidget* parent = 0);

        //!
        //! \brief The ColormapWidget destructor.
        //!
        ~ColormapWidget();

        //!
        //! \brief The getColormap method returns the current colormap.
        //!
        //! The method is inline.
        //!
        //! \return A reference to the constant current colormap object.
        //!
        inline Colormap const& getColormap() const;

        //!
        //! \brief The setColormap method changes the internal colormap the
        //!        widget can customize according to the given one.
        //!
        //! All widget components are adapted to this colormap.
        //!
        //! \param colormap The new colormap the widget must represents.
        //!
        //! \return Nothing.
        //!
        void setColormap(Colormap const& colormap);

        //!
        //! \brief The setFocusRange method sets the colorbar focus range (see
        //!        colorbar focus range description).
        //!
        //! \param range The new focus range to set for the colorbar.
        //!
        //! \return Nothing.
        //!
        void setFocusRange(Range const& range);

    public slots:
        //!
        //! \brief The updateColormap slot checks the colormap widget
        //!        components and adapts the current colormap if necessary.
        //!
        //! Notice that the colorbar is not taken into account as it
        //! automatically adapts the colormap.
        //!
        //! \return Nothing.
        //!
        void updateColormap();

        //!
        //! \brief The addNewColor slot checks the widget components to add a
        //!        new color in the colormap (and so in the colorbar).
        //!
        //! \return Nothing.
        //!
        void addNewColor();

        //!
        //! \brief The updateColorViewWidget slot adapts the color of the widget
        //!        which shows the current color.
        //!
        //! \return Nothing.
        //!
        void updateColorViewWidget();

        //!
        //! \brief The loadLut slot opens a file chooser dialog so that the user
        //!        can select a .lut file and load the selected one to be the
        //!        new colormap.
        //!
        //! If the .lut file is invalid, a messagebox appears to signal it to
        //! the user.
        //!
        //! \return Nothing.
        //!
        void loadLut();

        //!
        //! \brief The repaint slot adapts the widget component according to the
        //!        current colormap.
        //!
        //! Notice that the colorbar is not taken into account as it is
        //! automatically adapted with the colormap.
        //!
        //! This is an extension of the repaint slot.
        //! \see void Widget::repaint()
        //!
        //! \return Nothing.
        //!
        void repaint();

    signals:
        //!
        //! \brief The newValues signal, once emitted, indicates the colormap
        //!        has had modifications.
        //!
        void newValues();

    protected:
        //!
        //! \brief The getCurrentColor method returns the color the colormap
        //!        widget is ready to add (according to color configuration
        //!        controls).
        //!
        //! \return A QColor object which contains the color the colormap widget
        //!         is ready to add (according to color configuration controls).
        //!
        QColor getCurrentColor() const;

    private:
        // The colormap
        Colormap m_colormap;

        // Components
        ColorbarWidget* m_colorbar;
        customwidget::Dial* m_hueDial;
        customwidget::Widget* m_colorViewWidget;
        DoubleSlider *m_satSlider, *m_valSlider;
        customwidget::PushButton *m_addColorButton, *m_clearColorsButton, *m_loadLutButton;
        customwidget::DoubleSpinBox* m_startSpinBox;
        QButtonGroup m_modeButtonGroup;
        std::map<Colormap::InterpolationMode, customwidget::RadioButton*> m_interpolationModeButtons;
};

// The 'getColormap' method
inline Colormap const& ColormapWidget::getColormap() const { return m_colormap; }

#endif
