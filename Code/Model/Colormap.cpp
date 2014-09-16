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
//! \file Colormap.cpp
//! \brief The Colormap.cpp file contains the definition of non-inline
//!        methods of the Colormap class.
//!
//! \author Quentin Smetz
//!

#include "Colormap.h"
using namespace std;

// Constructor
Colormap::Colormap() : QObject(), m_interpolationMode(Colormap::LINEAR)
{}

// Copy constructor
Colormap::Colormap(Colormap const& copy) : QObject()
{
    *this = copy;
}

// Destructor
Colormap::~Colormap()
{}

// The 'operator=' method
Colormap& Colormap::operator=(Colormap const& copy)
{
    m_colors = copy.m_colors;
    m_interpolationMode = copy.m_interpolationMode;
    emit modified();

    return *this;
}

// The 'setStartAt' method
void Colormap::setStartAt(unsigned int index, double start)
{
    if(start < 0 || start > 1)
        return;

    m_colors.at(index).first = start;
    emit colorModified(index, start, m_colors.at(index).second);
}

// The 'setColorAt' method
void Colormap::setColorAt(unsigned int index, QColor const& color)
{
    m_colors.at(index).second = color;
    emit colorModified(index, m_colors.at(index).first, color);
}

// The 'setInterpolationMode' method
void Colormap::setInterpolationMode(InterpolationMode const& interpolationMode)
{
    m_interpolationMode = interpolationMode;
    emit interpolationModeModified(interpolationMode);
}

// The 'addColor' method
void Colormap::addColor(double start, QColor const& color)
{
    if(start < 0 || start > 1)
        return;

    // Search where to insert the new color
    unsigned int i = 0;
    for(vector< pair<double, QColor> >::iterator it = m_colors.begin()
         ; it != m_colors.end() ; it++)
    {
        if(start < it->first)
        {
            m_colors.insert(it, pair<double, QColor>(start, color));
            emit colorAdded(i, start, color);
            return;
        }

        i++;
    }

    m_colors.push_back(pair<double, QColor>(start, color));
    emit colorAdded(m_colors.size()-1, start, color);
}

// The 'loadFromLutFile' method
bool Colormap::loadFromLutFile(QString filename)
{
    clear();

    ifstream stream(filename.toStdString().c_str(), ios::in | ios::binary);
    if(!stream.is_open())
        return false;

    unsigned char c;
    vector<unsigned int> cs;

    try{
        while(stream.read((char*)&c, sizeof(unsigned char)) != 0)
            cs.push_back(static_cast<unsigned int>(c));
    }
    catch(...){
        stream.close();
        return false;
    }

    map<unsigned int, QColor> colorList;
    unsigned int r = cs.size()/3;
    for(unsigned int i = 0 ; i < r ; i++)
        colorList[i] = QColor::fromRgb(cs.at(i), cs.at(i+r), cs.at(i+r*2));

    for(map<unsigned int, QColor>::iterator iter = colorList.begin()
        ; iter != colorList.end() ; )
    {
        QColor const& cL = iter->second;
        double sL = iter->first;

        iter++; if(iter == colorList.end()) break;
        QColor const& c = iter->second;
        double s = iter->first;

        iter++; if(iter == colorList.end()) break;
        QColor const& cR = iter->second;
        double sR = iter->first;

        iter--;
        iter--;
        double pos = static_cast<double>(s - sL) / static_cast<double>(sR - sL);

        QColor approx = QColor::fromRgbF(cL.redF()+(cR.redF()-cL.redF())*pos,
                                               cL.greenF()+(cR.greenF()-cL.greenF())*pos,
                                               cL.blueF()+(cR.blueF()-cL.blueF())*pos);

        if(sqrt(pow(approx.redF() - c.redF(), 2)
                + pow(approx.greenF() - c.greenF(), 2)
                + pow(approx.blueF() - c.blueF(), 2)) <= 2.0/255.0)
        {
            colorList.erase(++iter);
            iter = colorList.begin();
        }
        else
            iter++;
    }

    for(map<unsigned int, QColor>::iterator iter = colorList.begin()
        ; iter != colorList.end() ; iter++)
        addColorToEnd(static_cast<double>(iter->first)/(r-1), iter->second);
    emit modified();

    stream.close();
    return true;
}

// The 'getColorFromPosition' method
QColor Colormap::getColorFromPosition(double position) const
{
    // If position if before the first color, return black
    if(count() > 0 && position < startAt(0))
        return QColor::fromRgbF(0, 0, 0);

    // Search the position color
    for(vector< pair<double, QColor> >::const_iterator it = m_colors.begin()
         ; it != m_colors.end() ; it++)
    {
        if(position < it->first)
            return (--it)->second;
    }

    // If position is above the last color (and there is one), return this color
    if(count() > 0)
        return colorAt(count()-1);

    // If no color, return white
    return QColor::fromRgbF(1, 1, 1);
}

// The 'computeVTKColorTransferFunction' method
vtkColorTransferFunction* Colormap::computeVTKColorTransferFunction
    (Range const& onRange, Range const& onRangeWindow) const
{
    double start;
    QColor color;

    // Initialize the color function
    vtkColorTransferFunction* func = vtkColorTransferFunction::New();
    func->SetColorSpaceToRGB();
    func->AllowDuplicateScalarsOn();

    // If no color, all modes shows a grayscale
    if(count() == 0)
    {
        func->AddRGBPoint(onRange.min()-1e-6, 0, 0, 0);
        func->AddRGBPoint(onRangeWindow.min()-1e-6, 0, 0, 0);
        func->AddRGBPoint(onRangeWindow.max()+1e-6, 1, 1, 1);
        func->AddRGBPoint(onRange.max()+1e-6, 1, 1, 1);
        return func;
    }

    switch(m_interpolationMode)
    {
        case Colormap::LINEAR:
            // Fill begin with first color
            color = colorAt(0);
            func->AddRGBPoint(onRange.min()-1e-6, color.redF(), color.greenF(), color.blueF());
            func->AddRGBPoint(onRangeWindow.min()-1e-6, color.redF(), color.greenF(), color.blueF());

            // Add colors one by one
            for(unsigned int i = 0 ; i < count() ; i++)
            {
                start = onRangeWindow.absolute(startAt(i), false);
                color = colorAt(i);

                func->AddRGBPoint(start, color.redF(), color.greenF(), color.blueF());
            }

            // Fill outside above range of colormap with last color
            color = colorAt(count()-1);
            func->AddRGBPoint(onRangeWindow.max()+1e-6, color.redF(), color.greenF(), color.blueF());
            func->AddRGBPoint(onRange.max()+1e-6, color.redF(), color.greenF(), color.blueF());
            break;

        case Colormap::CONSTANT:
            // Start with black until range window
            func->AddRGBPoint(onRange.min()-1e-6, 0, 0, 0);
            func->AddRGBPoint(onRangeWindow.min()-1e-6, 0, 0, 0);

            // Add black until first color
            start = onRange.absolute(startAt(0), false);
            func->AddRGBPoint(start-1e-6, 0, 0, 0);

            // Add colors one by one
            for(unsigned int i = 0 ; i < count() ; i++)
            {
                start = onRange.absolute(startAt(i), false);
                color = changeValue(colorAt(i), onRangeWindow.relative(start));

                if(start >= onRangeWindow.min())
                    func->AddRGBPoint(start, color.redF(), color.greenF(), color.blueF());

                if(i < count()-1)
                {
                    start = onRange.absolute(startAt(i+1), false);
                    color = changeValue(color, onRangeWindow.relative(start));

                    if(start >= onRangeWindow.min())
                        func->AddRGBPoint(start-(1e-6), color.redF(), color.greenF(), color.blueF());
                }
            }

            color = changeValue(getColorFromPosition(onRange.relative(onRangeWindow.max())), 1);
            func->AddRGBPoint(onRangeWindow.max()+1e-6, color.redF(), color.greenF(), color.blueF());

            color = changeValue(getColorFromPosition(1), 1);
            func->AddRGBPoint(onRange.max()+1e-6, color.redF(), color.greenF(), color.blueF());
            break;
    }

    return func;
}

// The 'changeValue' static method
QColor Colormap::changeValue(QColor const& color, double value)
{
    QColor hsv = color.toHsv();
    return QColor::fromHsvF(hsv.hsvHueF(), hsv.hsvSaturationF(), value);
}

// The 'clear' slot
void Colormap::clear()
{
    m_colors.clear();
    emit modified();
}

// The 'addColorToEnd' private method
void Colormap::addColorToEnd(double start, QColor const& color)
{
    if(start < 0 || start > 1)
        return;

    m_colors.push_back(pair<double, QColor>(start, color));
}
