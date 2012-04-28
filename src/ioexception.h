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


/*! \file  ioexception.h
 *  \brief File for the IOException class definition.
 */

#include <string>
#include <stdexcept>


/// Class for representing an IO exception.
class IOException : public std::runtime_error {

public:

    /*! \brief Constructor.
     *  \param message  : The error message.
     *  \param location : The error location.
     */
    IOException(const std::string & message, const std::string & location);

    /*! \brief Implementing the destructor.
     */
    virtual ~IOException() throw() {;}

    /*! \brief Overriding the base class what() function.
     */
    virtual const char* what() const throw();

    /*! \brief Function for printing an error message.
     */
    void print() const;

    /*! \brief Query function for the message.
     */
    std::string message() const { return message_; }

    /*! \brief Query function for the location.
     */
    std::string location() const { return location_; }

protected:

private:

    /*! \brief Function for generating the crash info.
     */
    std::string info() const;
    /// The error message.
    std::string message_;
    /// The error location.
    std::string location_;
};

