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
//! \file ProgramConfiguration.h
//! \brief The ProgramConfiguration.h file contains the interface of the
//!        ProgramConfiguration class and the definitions of its inline methods.
//!
//! \author Quentin Smetz
//!

#ifndef PROGRAMCONFIGURATION_H
#define PROGRAMCONFIGURATION_H

#include <iostream>
#include <string>
#include <fstream>
#include <map>

#include "Model/Range.h"

//!
//! \brief The ProgramConfiguration class provides an access to some basic
//!        informations about the program.
//!
//! The class is a singleton, only one object is created and is reachable by
//! all points of the program.
//!
class ProgramConfiguration
{
    public:
        //!
        //! \brief The instance static method give access to the unique
        //!        ProgramConfiguration object.
        //!
        //! The parameter is ignored if the unique object has already been created.
        //! Otherwise the unique object is created (by the constructor).
        //!
        //! \param configFileName The name of the configuration file to load.
        //!
        //! \return A pointer to the unique ProgramConfiguration object (or 0
        //!         if object failed to be created.
        //!
        static ProgramConfiguration* instance(std::string configFileName = "");

        //!
        //! \brief The free static method frees the memory which is taken by
        //!        the unique program configuration object.
        //!
        //! Need to called if the unique object was created.
        //!
        //! \return Nothing.
        //!
        static void free();

        //!
        //! \brief The imageDirectory method returns the path to the image
        //!        directory (whose images are used by the program).
        //!
        //! The method is inline.
        //!
        //! \return A string which contains the path to the image directory.
        //!
        inline std::string imageDirectory() const;

        //!
        //! \brief The defaultHost method returns the default host to reach
        //!        the Orthanc server.
        //!
        //! The method is inline.
        //!
        //! \return A string which contains the default host to reach the
        //!         Orthanc server.
        //!
        inline std::string defaultHost() const;

        //!
        //! \brief The defaultPort method returns the default port to reach
        //!        the Orthanc server.
        //!
        //! The method is inline.
        //!
        //! \return The default port to reach the Orthanc server.
        //!
        inline unsigned short defaultPort() const;

        //!
        //! \brief The huPresets method returns a collection of name -> range
        //!        pairs which indicates some hounsfield window presets.
        //!
        //! The method is inline.
        //!
        //! \return A map object which maps presets names to hounsfield
        //!         range presets.
        //!
        inline std::map<std::string, Range> const& huPresets() const;

        //!
        //! \brief The lutDirectory method returns the path to a LUT directory
        //!        (whose LUT files can be used to customize medical images).
        //!
        //! The method is inline.
        //!
        //! \return A string object which contains the path to a LUT directory.
        //!
        inline std::string const& lutDirectory() const;

        //!
        //! \brief The ProgramConfiguration destructor.
        //!
        ~ProgramConfiguration();

    private:
        static ProgramConfiguration* s_programConfiguration;

        ProgramConfiguration(std::string configFileName);

        std::string m_imageDirectory;
        std::string m_defaultHost;
        unsigned short m_defaultPort;
        std::map<std::string, Range> m_hounsfieldPresets;
        std::string m_lutDirectory;
};

// The 'imageDirectory' method
inline std::string ProgramConfiguration::imageDirectory() const { return m_imageDirectory; }

// The 'defaultHost' method
inline std::string ProgramConfiguration::defaultHost() const { return m_defaultHost; }

// The 'defaultPort' method
inline unsigned short ProgramConfiguration::defaultPort() const { return m_defaultPort; }

// The 'huPresets' method
inline std::map<std::string, Range> const& ProgramConfiguration::huPresets() const
{ return m_hounsfieldPresets; }

// The 'lutDirectory' method
inline std::string const& ProgramConfiguration::lutDirectory() const { return m_lutDirectory; }

#endif
