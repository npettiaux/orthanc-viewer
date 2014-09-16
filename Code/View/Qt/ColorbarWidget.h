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
//! \file ColorbarWidget.h
//! \brief The ColorbarWidget.h file contains the interface of the
//!        ColorbarWidget class and the definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef COLORBARWIDGET_H
#define COLORBARWIDGET_H

#include <iostream>

#include <QBoxLayout>

#include <vtkSmartPointer.h>

#include "View/Qt/customwidget/Label.h"
#include "View/Qt/customwidget/Splitter.h"
#include "View/Qt/customwidget/Widget.h"

#include "Model/Colormap.h"

//!
//! \brief The ColorbarWidget class is a widget which handles the visualisation
//!        of a colormap.
//!
//! The ColorbarWidget class is shown as a bar on which the colormap is drawn.
//! Each color starting point can then be moved by the user, so that the
//! colormap is adapted (and signal it to the user by its own signals).
//!
class ColorbarWidget : public customwidget::Widget
{
    Q_OBJECT

    public:
        //!
        //! \brief The ColorbarWidget constructor initializes the colorbar with
        //!        the given colormap, on a specific range.
        //!
        //! \param colormap The colormap the colorbar represents.
        //! \param barRange The range on which the colormap has to be applied.
        //! \param parent The parent widget of the colorbar.
        //!
        ColorbarWidget(Colormap* colormap, Range const& barRange, QWidget* parent = 0);

        //!
        //! \brief The ColorbarWidget destructor.
        //!
        ~ColorbarWidget();

        //!
        //! \brief The setFocusRange method adapt the range which is focused by
        //!        the colorbar.
        //!
        //! The signification of the focused range depends on the internal
        //! interpolation mode of the colormap. In case of a linear
        //! interpolation, the focus range represents the range on which the
        //! colormap is applied. In case of a constant interpolation, the focus
        //! range represents the range on which the intensity of the color
        //! increases.
        //!
        //! \param range The new focus range.
        //!
        void setFocusRange(Range const& range);

        //!
        //! \brief The getBoundaries method returns the range on which the
        //!        colormap is applied.
        //!
        //! The range on which the colormap is applied depends on the internal
        //! interpolation mode of the colormap. When it is linear, it is the
        //! focus range. When it is constant, it is the colorbar whole range.
        //!
        //! \return The range on which the colormap is applied.
        //!
        Range getBoundaries() const;

    public slots:
        //!
        //! \brief The addColor slot adds a widget to the colorbar whose size
        //!        can be adapted by the user to adapt the colormap.
        //!
        //! \param mustRepaint A boolean which indicates if the colorbar must be
        //!                    repainted. Needed in most cases but can be
        //!                    desactivated when several colors have to be added
        //!                    successively.
        //!
        //! \return Nothing.
        //!
        void addColor(bool mustRepaint = true);

        //!
        //! \brief The updateColorStart slot change the starting point of a
        //!        specific color (and its neighbors if needed).
        //!
        //! \param pos The new starting point of the color.
        //! \param index The index of the color's widget.
        //!
        //! \return Nothing.
        //!
        void updateColorStart(int pos, int index);

        //!
        //! \brief The repaint slot adapts the number of color widgets, applies
        //!        the colormap on them and adapts their sizes.
        //!
        //! This is an extension of the repaint slot.
        //! \see void Widget::repaint()
        //!
        //! \return Nothing.
        //!
        void repaint();

    protected:
        //!
        //! \brief The paint protected method colors a specific widget according
        //!        to a table of pixel colors.
        //!
        //! \param w A pointer to the QWidget to color.
        //! \param range The range of pixel colors to take into account.
        //! \param table A table of pixel colors.
        //!
        //! \return Nothing.
        //!
        void paint(QWidget* w, Range const& range, double table[]);

        //!
        //! \brief The resizeEvent protected method is called when to colorbar
        //!        is resized. It fully repaints the colorbar.
        //!
        //! \param event A pointer to the QResizeEvent.
        //!
        //! \return Nothing.
        //!
        virtual void resizeEvent(QResizeEvent* event);

    private:
        Colormap* m_colormap; // The colormap the colorbar represents at any time

        Range const m_barRange; // The whole range of the colorbar
        Range m_focusRange;     // The focus range (see setFocusRange method)

        // Components
        customwidget::Splitter* m_splitter;
        customwidget::Label *m_labelMin, *m_labelMax;
};

#endif
