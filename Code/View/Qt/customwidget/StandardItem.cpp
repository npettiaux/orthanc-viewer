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


#include "StandardItem.h"
#include "Model/ProgramConfiguration.h"

// Constructor
customwidget::StandardItem::StandardItem(QString const& text, StandardItemType type)
    : QStandardItem(text), m_type(type)
{
    QString imgDir(ProgramConfiguration::instance()->imageDirectory().c_str());

    switch(m_type)
    {
        case customwidget::StandardItem::NONE:
            break;

        case customwidget::StandardItem::PATIENT:
            setBackground(QColor(192, 192, 255, 64));
            setSizeHint(QSize(0, 50));
            setIcon(QIcon(imgDir + "/patient_icon.png"));
            break;

        case customwidget::StandardItem::STUDY:
            setBackground(QColor(192, 255, 192, 64));
            setSizeHint(QSize(0, 50));
            setIcon(QIcon(imgDir + "/study_icon.png"));
            break;

        case customwidget::StandardItem::SERIES:
            setBackground(QColor(255, 192, 192, 64));
            setSizeHint(QSize(0, 50));
            break;
    }
}
