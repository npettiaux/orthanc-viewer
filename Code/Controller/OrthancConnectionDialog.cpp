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
//! \file OrthancConnectionDialog.cpp
//! \brief The OrthancConnectionDialog.cpp file contains the definition of
//!        non-inline methods of the OrthancConnectionDialog class.
//!
//! \author Quentin Smetz
//!

#include "OrthancConnectionDialog.h"
#include "Model/ProgramConfiguration.h"
using namespace std;
using namespace customwidget;

// Constructor
OrthancConnectionDialog::OrthancConnectionDialog(QWidget* parent)
    : OkCancelDialog(parent), m_orthanc(0)
{
    setModal(true);
    setWindowTitle("Connexion");

    // Create the interface
    QFormLayout* formLayout = new QFormLayout();

    m_hostLineEdit = new LineEdit(QString(ProgramConfiguration::instance()->defaultHost().c_str()));
    formLayout->addRow("Hôte", m_hostLineEdit);

    m_portSpinBox = new SpinBox();
    m_portSpinBox->setRange(1, 65535);
    m_portSpinBox->setValue(ProgramConfiguration::instance()->defaultPort());
    formLayout->addRow("Port", m_portSpinBox);

    m_userLineEdit = new LineEdit("");
    formLayout->addRow("Utilisateur", m_userLineEdit);

    m_passLineEdit = new LineEdit("");
    formLayout->addRow("Mot de passe", m_passLineEdit);

    setLayout(formLayout);
}

// Destructor
OrthancConnectionDialog::~OrthancConnectionDialog()
{
    if(m_orthanc != 0)
        delete m_orthanc;
}

// The 'accept' slot
void OrthancConnectionDialog::accept()
{
    if(m_orthanc != 0)
        delete m_orthanc;

    setEnabled(false);

    try
    {
        m_orthanc = new OrthancClient::OrthancConnection(computeUrl().toStdString(),
                                                         m_userLineEdit->text().toStdString(),
                                                         m_passLineEdit->text().toStdString());
    }
    catch(OrthancClient::OrthancClientException& e)
    {
        cout << e.What() << endl;
    }

    OkCancelDialog::accept();
}

// The 'computeUrl' method
QString OrthancConnectionDialog::computeUrl() const
{
    QString url("");
    url += m_hostLineEdit->text();
    url += ":";
    url += QString::number(m_portSpinBox->value());

    return url;
}
