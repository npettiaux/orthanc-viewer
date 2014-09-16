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
//! \file main.cpp
//! \brief The main.cpp file contains the definition of the main method.
//!
//! \author Quentin Smetz
//!

#include <iostream>
#include <QApplication>
#include <QTextCodec>

#include "orthanc/OrthancCppClient.h"

#include "Model/ProgramConfiguration.h"

#include "Controller/ViewerWindow.h"

using namespace std;

//!
//! \brief The main function is the principal function of the program.
//!
//! \param argc The number of program's arguments.
//! \param argv The table of program's arguments (char* format).
//!
//! \return zero if the program exited successfully.
//!
int main(int argc, char* argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

	// Initialize Orthanc client library
    try {
#if 0
        string const curFile(argv[0]);
        string const libFile = curFile.substr(0, curFile.rfind("/")+1) + _ORTHANC_LIB_FILENAME_;
        std::cout << libFile<< std::endl;
        OrthancClient::Initialize(libFile);
#else
        OrthancClient::Initialize();
#endif
	}
    catch(OrthancClient::OrthancClientException& e) {
        cerr << e.What() << endl;
		return 0;
	}

    // Create the Qt application
    QApplication app(argc, argv);

    // Create program configuration from config file
    ProgramConfiguration::instance(_CONFIG_FILENAME_);

    // Create main window
    ViewerWindow::instance();

    // Launch and exit application
    int appResult = app.exec();

    // Free
    ViewerWindow::free();
    ProgramConfiguration::free();

    return appResult;
}
