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


/*! \file  ioutils.h
 *  \brief File containing definitions of macros and utility functions for IO and error handling.
 */

#ifndef __IOUTILS__
#define __IOUTILS__

#include <fstream>
#include <string>

/// Defines a macro for giving an error with specified location.
#define LOCATION std::string(std::string(__FILE__) + ": " + to_string(__LINE__))

/// Defines a macro for giving an error with specified function.
#define FUNCTION std::string(__func__)


/*! \brief Convert an integer to string.
 *  \param i : The integer to convert.
 *  \return  : A string representation of the integer.
 */
std::string to_string(const int i);


/*! \brief Insert a number of blank spaces after each newline.
 *  \param message : The string to add blanks to.
 *  \param blanks  : The number of blanks to add after each newline.
 *  \return        : The processed string.
 */
std::string newline_indent(const std::string & message, const int blanks);


/*! \brief Checks if the stream is at the end.
 *  \param stream : The stream to check.
 *  \return       : If it is at end of file or not.
 */
bool eof(std::ifstream & stream);


/*! \brief Checks if the stream is at the end and rais an error if it is.
 *  \param stream   : The stream to check.
 *  \param filename : The associated filename.
 */
void check_eof(std::ifstream & stream,
               const std::string & filename,
               const std::string & location="");


/*! \brief Checks that a file exists and can be opened for reading.
 *  \param path     : The path to check.
 *  \param location : The location of the check.
 */
void check_path(const std::string & path, const std::string & location="");


/*! \brief Checks that a given sigma value is not too close to zero.
 *  \param sigma    : The value to check.
 *  \param location : The location of the check.
 */
void check_sigma(const double sigma, const std::string & location="");


/*! \brief Checks that a given integer value is above or equal to zero.
 *  \param value    : The value to check.
 *  \param filename : The name of the file that was read.
 *  \param location : The location of the check.
 */
void check_positive_integer(const int value, const std::string & filename, const std::string& location="");


/*! \brief Raise an error for problems with opening a file.
 *  \param filename : The file name.
 *  \param location : The location of the error.
 */
void open_file_error(const std::string & filename, const std::string & location="");


/*! \brief Raise an error for an empty file.
 *  \param filename : The file name.
 *  \param location : The location of the error.
 */
void empty_file_error(const std::string & filename, const std::string & location="");


/*! \brief Raise an error for an unknown section.
 *  \param message  : The unknown section keyword.
 *  \param location : The location of the error.
 */
void unknown_section_error(const std::string & section, const std::string & location="");


/*! \brief Raise an error for encountering the same section twize.
 *  \param message  : The section keyword.
 *  \param location : The location of the error.
 */
void same_section_error(const std::string & section, const std::string & location="");


/*! \brief Raise an error for missing keyword.
 *  \param message  : The missing keyword.
 *  \param location : The location of the error.
 */
void missing_keyword_error(const std::string & keyword, const std::string & location="");


/*! \brief Raise an error for finding the same keyword twice.
 *  \param message  : The keyword that was found twice.
 *  \param location : The location of the error.
 */
void same_keyword_error(const std::string & keyword, const std::string & location="");


/*! \brief Raise an error for unknown keyword.
 *  \param message  : The keyword that was found.
 *  \param location : The location of the error.
 */
void unknown_keyword_error(const std::string & keyword, const std::string & location="");


/*! \brief Raise an error for problems with reading a keyword.
 *  \param filename : The file that was read from.
 *  \param expected : The expected keyword.
 *  \param found    : The the encountered keyword.
 *  \param location : The location of the error.
 */
void read_keyword_error(const std::string & filename,
                        const std::string & expected,
                        const std::string & found,
                        const std::string & location="");


/*! \brief Sets the global time0. (Starts the timer.)
 */
void start_timer();


/*! \brief Function used for timing.
 *  \return : Seconds since the timer was started.
 */
int timer();


/*! \brief Prints wall clock time and date info to stdout.
 */
void timestamp();


/*! \brief Raises an error.
 *  \param message  : The error message.
 *  \param location : The location of the error.
 */
void error(const std::string & message,
           const std::string & location=std::string("No location provided."));


/*! \brief Function for exiting the program with specified exit code and a
 *         short error message.
 *  \param program : The name of the program to exit.
 */
void error_exit(const std::string & program="SpecSwap-RMC");


/*! \brief Prints the startup message to stdout.
 */
void print_startup();


/*! \brief Function for eating (skipping) a line in an ifstream.
 */
void eatline(std::ifstream& infile);


/*! \brief Prints the exit message to stdout.
 */
void print_success();


#endif

