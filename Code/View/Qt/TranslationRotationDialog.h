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
//! \file TranslationRotationDialog.h
//! \brief The TranslationRotationDialog.h file contains the interface of the
//!        TranslationRotationDialog class and the definitions of its inline
//!        methods.
//!
//! \author Quentin Smetz
//!

#ifndef TRANSLATIONROTATIONDIALOG_H
#define TRANSLATIONROTATIONDIALOG_H

#include <QFormLayout>

#include "View/Qt/customwidget/Dial.h"
#include "View/Qt/customwidget/GroupBox.h"
#include "View/Qt/customwidget/Label.h"

#include "Model/ViewConfiguration.h"

#include "View/Qt/ViewConfigurationDialog.h"
#include "View/Qt/DoubleSlider.h"

//!
//! \brief The TranslationRotationDialog class provides a view configuration
//!        dialog to customize translations and rotations.
//!
class TranslationRotationDialog : public ViewConfigurationDialog
{
    Q_OBJECT

    public:
        //!
        //! \brief The TranslationRotationDialog constructor initializes the
        //!        interface and event connection.
        //!
        //! \param parent A pointer to the parent window of the dialog.
        //!
        TranslationRotationDialog(QWidget* parent = 0);

        //!
        //! \brief The TranslationRotationDialog destructor.
        //!
        ~TranslationRotationDialog();

    protected:
        //!
        //! \brief The updateComponentsFromCurrentConfiguration method updates
        //!        the dialog components so that it corresponds to the internal
        //!        current view configuration the dialog permits to customize.
        //!
        //! \return Nothing.
        //!
        void updateComponentsFromCurrentConfiguration();

        //!
        //! \brief The updateCurrentConfigurationFromComponents updates the
        //!        internal current configuration the dialog permits to customize
        //!        according to the state of the dialog components.
        //!
        //! \return Nothing.
        //!
        void updateCurrentConfigurationFromComponents();

        //!
        //! \brief The usefulParams method returns a set of ViewParam which
        //!        indicates the useful view parameters the dialog can
        //!        customize.
        //!
        //! The useful parameters are ViewParam::TRANSLATION and
        //! ViewParam::ROTATION.
        //!
        //! \return A set object which contains the ViewParam constants which
        //!         indicates the useful view parameters the dialog can
        //!         customize.
        //!
        std::set<ViewConfiguration::ViewParam> usefulParams() const;

    private:
        DoubleSlider *m_xSlider, *m_ySlider, *m_zSlider;
        customwidget::Dial *m_aDial, *m_bDial, *m_cDial;
};

#endif
