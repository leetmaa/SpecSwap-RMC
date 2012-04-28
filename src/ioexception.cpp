/*
    Part of the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    This program is distributed under the terms of the GNU General Public
    License version 3 as published by the Free Software Foundation,
    with the Additional Terms as stated in the SpecSwap-RMC LICENSE file.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    See the SpecSwap-RMC LICENSE file for full details.

    Author: Mikael Leetmaa    <leetmaa@fysik.su.se>
*/


/*! \file  ioexception.cpp
 *  \brief File for the IOException class implementation.
 */

#include <cstdio>

#include "ioexception.h"
#include "ioutils.h"

// -------------------------------------------------------------------------- //
//
IOException::IOException(const std::string & message,
                         const std::string & location) :
    std::runtime_error(message),
    message_(message),
    location_(location)
{
    // NOTHING HERE
}


// -------------------------------------------------------------------------- //
//
const char* IOException::what() const throw()
{
    return info().c_str();
}


// -------------------------------------------------------------------------- //
//
void IOException::print() const
{
    printf("%s", info().c_str());
}


// -------------------------------------------------------------------------- //
//
std::string IOException::info() const
{
    char str[1024];
    sprintf(str, "\n   IOException: %s\n       Message: %s\n",
            location_.c_str(),
            newline_indent(message_,16).c_str());
    return std::string(str);
}
