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
//! \file OkCancelDialog.h
//! \brief The OkCancelDialog.h file contains the interface of the
//!        OkCancelDialog class and the definitions of its inline
//!        methods.
//!
//! \author Quentin Smetz
//!

#ifndef OKCANCELDIALOG_H
#define OKCANCELDIALOG_H

#include <QBoxLayout>

#include "View/Qt/customwidget/Dialog.h"
#include "View/Qt/customwidget/PushButton.h"

//!
//! \brief The OkCancelDialog class represents a dialog window which already
//!        contains an acceptation button and a rejection button.
//!
//! The validation and rejection buttons call specific actions which can be
//! redefined in subclasses.
//!
class OkCancelDialog : public customwidget::Dialog
{
    public:
        //!
        //! \brief The OkCancelDialog constructor initializes the dialog window
        //!        with the two buttons and connect them on the validation and
        //!        rejection actions.
        //!
        //! \param parent The QWidget parent of the dialog.
        //!
        OkCancelDialog(QWidget* parent = 0);

        //!
        //! \brief The OkCancelDialog destructor.
        //!
        ~OkCancelDialog();

        //!
        //! \brief The layout method returns the layout of the useful part of
        //!        the dialog (without the buttons).
        //!
        //! This is a redefinition of the Dialog layout method.
        //! \see QLayout* Dialog::layout() const
        //!
        //! \return A pointer to the layout of the useful part of the dialog.
        //!
        virtual QLayout* layout() const;

        //!
        //! \brief The setLayout method sets the layout of the useful part of
        //!        the dialog (without the buttons).
        //!
        //! This is a redefiniation of the Dialog setLayout method.
        //! \see void setLayout(QLayout*)
        //!
        //! \param layout A pointer to the layout to apply on the useful part
        //!               of the dialog.
        //!
        //! \return Nothing.
        //!
        virtual void setLayout(QLayout* layout);

    private:
        QWidget* m_dialogArea; // The dialog area which does not contain the buttons
        customwidget::PushButton *m_okButton, *m_cancelButton; // The validation buttons
};

#endif
