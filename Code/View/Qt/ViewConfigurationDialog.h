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
//! \file ViewConfigurationDialog.h
//! \brief The ViewConfigurationDialog.h file contains the interface of the
//!        ViewConfigurationDialog class and the definitions of its inline
//!        methods.
//!
//! \author Quentin Smetz
//!

#ifndef VIEWCONFIGURATIONDIALOG_H
#define VIEWCONFIGURATIONDIALOG_H

#include <iostream>
#include <set>

#include "View/Qt/customwidget/CheckBox.h"

#include "Model/ViewConfiguration.h"

#include "View/Qt/OkCancelDialog.h"

//!
//! \brief The ViewConfigurationDialog class is a base class for all dialog
//!        which can generate a view configuration.
//!
//! A ViewConfiguration object contains an internal view configuration it can
//! customize and/or restore.
//!
class ViewConfigurationDialog : public OkCancelDialog
{
    Q_OBJECT

    public:
        //!
        //! \brief The ViewConfigurationDialog constructor initializes the
        //!        interface and connections.
        //!
        //! A view configuration dialog provides a checkbox to (des)active
        //! real time change in the configuration and a reset button.
        //!
        //! \param parent
        //!
        ViewConfigurationDialog(QWidget* parent = 0);

        //!
        //! \brief The ViewConfigurationDialog destructor.
        //!
        ~ViewConfigurationDialog();

        //!
        //! \brief The layout method returns the layout of the useful part of
        //!        the dialog (without the buttons).
        //!
        //! This is a redefinition of the OkCancelDialog layout method.
        //! \see QLayout* OkCancelDialog::layout() const
        //!
        //! \return A pointer to the layout of the useful part of the dialog.
        //!
        virtual QLayout* layout() const;

        //!
        //! \brief The setLayout method sets the layout of the useful part of
        //!        the dialog (without the buttons).
        //!
        //! This is a redefiniation of the OkCancelDialog setLayout method.
        //! \see void setLayout(QLayout*)
        //!
        //! \param layout A pointer to the layout to apply on the useful part
        //!               of the dialog.
        //!
        //! \return Nothing.
        //!
        virtual void setLayout(QLayout* layout);

    public slots:
        //!
        //! \brief The accept method saves the current configuration (so that
        //!        further changes can be undone to come back to the current one),
        //!        send it and hide the dialog.
        //!
        //! \return Nothing.
        //!
        virtual void accept();

        //!
        //! \brief The reject method resets the dialog and hide it.
        //!
        //! \see void ViewConfigurationDialog::reset();
        //! \return Nothing.
        //!
        virtual void reject();

        //!
        //! \brief The reset slot restores the configuration values to
        //!        their previous saved states and emits them if necessary.
        //!
        //! \return Nothing.
        //!
        virtual void reset();

        //!
        //! \brief The sendCurrentConfiguration slot emits a signal which
        //!        contains the current view configuration the dialog represents.
        //!
        //! \section emit
        //! A newConfiguration(ViewConfiguration const&, ViewConfiguration::ViewParam) is emitted.
        //!
        //! \param ifRealTime A boolean which is true if the method must not
        //!                   send the signal if the real time checkbox is not checked
        //!                   If the boolean is false, the signal is always emitted.
        //! \param updateBefore A boolean which is true if the current view configuration
        //!                     the dialog represents must first be updated according
        //!                     to the dialog components state.
        //!
        //! \return Nothing.
        //!
        void sendCurrentConfiguration(bool ifRealTime = true, bool updateBefore = true);

    signals:
        //!
        //! \brief The newConfiguration signal, once emitted, indicates the new
        //!        view configuration the dialog represents and the useful
        //!        view parameter in it.
        //!
        //! \param config The new view configuration the dialog represents
        //! \param param The useful view parameter to take into account in config
        //!
        void newConfiguration(ViewConfiguration const& config, ViewConfiguration::ViewParam param);

    protected:
        //!
        //! \brief The updateComponentsFromCurrentConfiguration method updates
        //!        the dialog components so that it corresponds to the internal
        //!        current view configuration the dialog permits to customize.
        //!
        //! The method is pure virtual, it must be redefined in subclasses.
        //!
        //! \return Nothing.
        //!
        virtual void updateComponentsFromCurrentConfiguration() = 0;

        //!
        //! \brief The updateCurrentConfigurationFromComponents updates the
        //!        internal current configuration the dialog permits to customize
        //!        according to the state of the dialog components.
        //!
        //! The method is pure virtual, it must be redefined in subclasses.
        //!
        //! \return Nothing.
        //!
        virtual void updateCurrentConfigurationFromComponents() = 0;

        //!
        //! \brief The usefulParams method returns a set of ViewParam which
        //!        indicates the useful view parameters the dialog can
        //!        customize.
        //!
        //! The method is pure virtual, it must be redefined in subclasses.
        //!
        //! \return A set object which contains the ViewParam constants which
        //!         indicates the useful view parameters the dialog can
        //!         customize.
        //!
        virtual std::set<ViewConfiguration::ViewParam> usefulParams() const = 0;

        //! The viewer configuration object the dialog customizes.
        ViewConfiguration m_currentConfig;

        //! The saved previous view configuration to which the dialog can come back.
        ViewConfiguration m_previousConfig;

    private:
        QWidget* m_configArea;

        customwidget::CheckBox* m_realTimeCheckBox;
        customwidget::PushButton* m_resetButton;
};

#endif
