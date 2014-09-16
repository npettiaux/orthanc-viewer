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
//! \file ProgramConfiguration.cpp
//! \brief The ProgramConfiguration.cpp file contains the definition of
//!        non-inline methods of the ProgramConfiguration class.
//!
//! \author Quentin Smetz
//!

#include "ProgramConfiguration.h"
using namespace std;

// Initialize the singleton to null
ProgramConfiguration* ProgramConfiguration::s_programConfiguration = 0;

// The 'instance' static method
ProgramConfiguration* ProgramConfiguration::instance(string configFileName)
{
    if(s_programConfiguration == 0)
        s_programConfiguration = new ProgramConfiguration(configFileName);

    return s_programConfiguration;
}

// The 'free' static method
void ProgramConfiguration::free()
{
    if(s_programConfiguration != 0)
    {
        delete s_programConfiguration;
        s_programConfiguration = 0;
    }
}

// Constructor
ProgramConfiguration::ProgramConfiguration(string configFileName)
    : m_imageDirectory(""), m_defaultHost(""), m_defaultPort(1), m_lutDirectory("")
{
    ifstream file(configFileName.c_str(), ios::in);

    if(file)
    {
        string line;
        while(getline(file, line))
        {
            if(line.size() == 0)
                continue;

            int eqIndex = line.find("=");
            if(eqIndex == static_cast<int>(string::npos))
                continue;

            string paramName = line.substr(0, eqIndex);
            while(paramName.size() > 0 && paramName.at(paramName.size()-1) == ' ')
                paramName.erase(paramName.size()-1);
            while(paramName.size() > 0 && paramName.at(0) == ' ')
                paramName.erase(0, 1);

            string paramContent = line.substr(eqIndex+1);
            while(paramContent.size() > 0 && paramContent.at(paramContent.size()-1) == ' ')
                paramContent.erase(paramContent.size()-1);
            while(paramContent.size() > 0 && paramContent.at(0) == ' ')
                paramContent.erase(0, 1);

            if(paramName == "IMAGE_DIRECTORY")
            {
                m_imageDirectory = paramContent;
            }
            else if(paramName == "ORTHANC_SERVER")
            {
                istringstream iss(paramContent);
                if(!(iss.eof()))
                    iss >> m_defaultHost;
                if(!(iss.eof()))
                    iss >> m_defaultPort;
            }
            else if(paramName == "HOUNSFIELD_PRESETS")
            {
                string huName;
                double tmp;

                istringstream iss(paramContent);
                while(!(iss.eof()))
                {
                    iss >> huName;
                    Range range;
                    iss >> tmp;
                    range.min() = tmp;
                    iss >> tmp;
                    range.max() = tmp;

                    m_hounsfieldPresets[huName] = range;
                }
            }
            else if(paramName == "LUT_DIRECTORY")
            {
                m_lutDirectory = paramContent;
            }
        }

        file.close();
    }
}

// Destructor
ProgramConfiguration::~ProgramConfiguration()
{}
