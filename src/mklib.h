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


/*! \file  mklib.h
 *  \brief File for the Mklib class definition.
 */

#include <string>
#include <vector>
#include <iosfwd>

#include "library.h"


/// Class for implementing the main functionality of the mklib program.
class Mklib {

public:

    /*! \breief Constructor providing a way to set the debug flag for testing.
     */
    Mklib(const bool debug=false);

    /*! \brief Function for compiling a new binary library file from
               specifications in a .info file.
     *  \param basename : The base name of the the .info file to construct from.
     */
    void compile_library(const std::string & basename);

    /*! \brief Static function to return the library version string.
     */
    static
    const std::string get_lib_version()
    {
        return Library().version_;
    }

private:

    /*! \brief The workhorse function for setting up a library from a .info file.
     *  \param basename  : The folder in which the .info file with the same name to read is to be found.
     *  \param debugname : Name for debugging. Takes a full path.
     */
    Library library_from_info_file(const std::string & basename,
                                   const std::string & debugname="NONAME");


    /*! \brief Helper routine to check keyword errors and throw an error if needed.
     *  \param key      : The keyword to check.
     *  \param found    : The keyword found.
     *  \param filename : The name of the file we were reading from.
     *  \param location : The the location of the check.
     */
    void check_keyword(const std::string & key,
                       const std::string & found,
                       const std::string & filename,
                       const std::string & location);

    /// Flag for indicating verbos printout for debugging.
    static const bool verbos = false;

    /*! \brief Flag indicating if we run in debug/test mode.
     *         With this flag set to true no calls are made to functions
     *         that reads data from file, other than the main input file.
     *         Insead the function call is printed to standard out.
     */
    bool debug_;

    /// Declare the test class a friend to facilitate testing.
    friend class Test_Mklib;
};


