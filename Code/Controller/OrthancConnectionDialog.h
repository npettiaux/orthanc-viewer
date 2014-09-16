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
//! \file OrthancConnectionDialog.h
//! \brief The OrthancConnectionDialog.h file contains the interface of the
//!        OrthancConnectionDialog class and the definitions of its inline
//!        methods.
//!
//! \author Quentin Smetz
//!

#ifndef ORTHANCCONNECTIONDIALOG_H
#define ORTHANCCONNECTIONDIALOG_H

#include <iostream>

#include <QFormLayout>
#include <QMovie>

#include "orthanc/OrthancCppClient.h"

#include "View/Qt/customwidget/Label.h"
#include "View/Qt/customwidget/LineEdit.h"
#include "View/Qt/customwidget/SpinBox.h"

#include "View/Qt/OkCancelDialog.h"

//!
//! \brief The OrthancConnectionDialog class is a dedicated dialog to enter
//!        and use orthanc connection information to initialize internal
//!        orthanc communication.
//!
class OrthancConnectionDialog : public OkCancelDialog
{
    Q_OBJECT

    public:
        //!
        //! \brief The OrthancConnectionDialog constructor constructs the
        //!        dialog interface.
        //!
        //! \param parent A pointer to the parent of the dialog.
        //!
        OrthancConnectionDialog(QWidget* parent = 0);

        //!
        //! \brief The OrthancConnectionDialog destructor.
        //!
        //! The OrthancConnection is closed.
        //!
        ~OrthancConnectionDialog();

        //!
        //! \brief The orthanc method returns the internal OrthancConnection
        //!        object.
        //!
        //! The method is inline.
        //!
        //! \return A pointer to the internal OrthancConnection (or 0 if not
        //!         initialized).
        //!
        inline OrthancClient::OrthancConnection* orthanc() const;

    public slots:
        //!
        //! \brief The accept slot close the dialog after trying to initialize
        //!        an Orthanc connection with the entered connection information
        //!        and signal it if successful.
        //!
        //! \return Nothing.
        //!
        void accept();

    protected:
        //!
        //! \brief The computeUrl protected method returns a string object which
        //!        contains the Orthanc server url, computed from entered host
        //!        and port.
        //!
        //! \return A string object which contains the Orthanc server url,
        //!         computed from entered host and port.
        //!
        QString computeUrl() const;

    private:
        customwidget::LineEdit* m_hostLineEdit;
        customwidget::SpinBox* m_portSpinBox;
        customwidget::LineEdit *m_userLineEdit, *m_passLineEdit;

        OrthancClient::OrthancConnection* m_orthanc; // The Orthanc server connexion
};

// The 'orthanc' method
inline OrthancClient::OrthancConnection* OrthancConnectionDialog::orthanc() const
{ return m_orthanc; }

#endif
