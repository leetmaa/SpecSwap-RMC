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


/*! \file  ioutils.cpp
 *  \brief File containing implementations for functions defined in ioutils.h
 */

#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <stdexcept>
#include <sstream>

#include "ioexception.h"
#include "ioutils.h"

int time_0__;

// -------------------------------------------------------------------------- //
//
std::string to_string(const int i)
{
    std::string s;
    std::stringstream out;
    out << i;
    return out.str();
}


// -------------------------------------------------------------------------- //
//
std::string newline_indent(const std::string & message, const int blanks)
{
    // Generate the blank string.
    std::string indent("\n");
    for (int i = 0; i < blanks; ++i)
    {
        indent += ' ';
    }

    // Find and replace the newline characters.
    std::string msg;
    std::string::const_iterator it = message.begin();
    for ( ; it != message.end(); ++it)
    {
        if (*it == '\n')
        {
            msg += indent;
        }
        else
        {
            msg += *it;
        }
    }

    // All done.
    return msg;
}


// -------------------------------------------------------------------------- //
//
void print_startup()
{
    printf("   SpecSwap-RMC version 1.0a0\n");
    printf("   Copyright (c) 2008 - 2012  Mikael Leetmaa\n");
    printf("\n");
    printf("\n");
    printf("       ===============================================================\n");
    printf("         SSS                 SSS                     RRRR  M   M  CCC\n");
    printf("        S                   S                        R   R MM MM C\n");
    printf("         SSS  ppp  eee  ccc  SSS  w   w  aa ppp  --- RRRR  M M M C\n");
    printf("            S p  p e e c        S w w w a a p  p     R R   M   M C\n");
    printf("        SSSS  ppp  ee   ccc SSSS   w w  aaa ppp      R  RR M   M  CCC\n");
    printf("      ======= p =========================== p =========================\n");
    printf("              p                             p\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("   Author:  Mikael Leetmaa   <leetmaa@fysik.su.se>\n");
    printf("\n");
    printf("\n");
    printf("   This program implements the SpecSwap-RMC method as described in:\n");
    printf("\n");
    printf("        \"SpecSwap-RMC: a novel reverse Monte Carlo approach using a discrete\n");
    printf("         set of local configurations and pre-computed properties\"\n");
    printf("         Mikael Leetmaa, Kjartan Thor Wikfeldt and Lars G. M. Pettersson\n");
    printf("         J. Phys.: Cond. Matter, 22, 135001, (2010)\n");
    printf("\n");
    printf("\n");
    printf("    This program is distributed under the terms of the GNU General Public\n");
    printf("    License version 3 as published by the Free Software Foundation,\n");
    printf("    with the Additional Terms as stated in the SpecSwap-RMC LICENSE file.\n");
    printf("\n");
    printf("    This program is distributed in the hope that it will be useful,\n");
    printf("    but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
    printf("    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
    printf("    SpecSwap-RMC LICENSE file for more details.\n");
    printf("\n");
    printf("    You should have received a copy of the GNU General Public License\n");
    printf("    along with this program.  If not, see <http://www.gnu.org/licenses/>.\n");
    printf("\n");
    printf("===============================================================================\n");

    timestamp();
}


// -------------------------------------------------------------------------- //
//
bool eof(std::ifstream & stream)
{
    // NOTE : This works for detecting an empty file, but seems to
    //        give false positives and report eof also for format
    //        errors in the stream, on Mac with gcc version
    //        i686-apple-darwin9-gcc-4.2.1
    return stream.peek() == std::ifstream::traits_type::eof();
}


// -------------------------------------------------------------------------- //
//
void check_path(const std::string& path, const std::string& location)
{
    // Open the file.
    std::ifstream infile(path.c_str(), std::ios::in | std::ios::binary);

    // Check that it was openened.
    if (!infile)
    {
        open_file_error(path, location);
    }

    // Check that it is not empty.
    if (eof(infile))
    {
        empty_file_error(path, location);
    }

    // Close.
    infile.close();
}


// -------------------------------------------------------------------------- //
//
void check_eof(std::ifstream & stream, const std::string & filename, const std::string & location)
{
    if (eof(stream) || !stream.good())
    {
        std::string msg = "Format error or EOF when reading from: " + filename + "\n";
        error(msg, location);
    }
}


// -------------------------------------------------------------------------- //
//
void check_sigma(const double sigma, const std::string& location)
{
    // Check that the sigma value is not too small.
    if (sigma*sigma < 1.0e-10)
    {
        error("The SIGMA value squared must be larget than 1.0e-10\n", location);
    }

    if (sigma < 0.0)
    {
        error("The SIGMA value squared must not be negative.\n", location);
    }
}


// -------------------------------------------------------------------------- //
//
void check_positive_integer(const int value, const std::string & filename, const std::string& location)
{
    if (value < 0)
    {
        std::string msg = "Expected an integer value >= 0, found " + to_string(value) + "\nWhen reading file:" + filename + "\n";
        error(msg, location);
    }
}


// -------------------------------------------------------------------------- //
//
void open_file_error(const std::string & filename, const std::string & location)
{
    std::string msg = "Could not open file: " + filename + "\n";
    error(msg, location);
}


// -------------------------------------------------------------------------- //
//
void empty_file_error(const std::string & filename, const std::string & location)
{
    std::string msg = "File '" + filename + "' is empty.\n";
    error(msg, location);
}


// -------------------------------------------------------------------------- //
//
void unknown_section_error(const std::string & section, const std::string & location)
{
    std::string msg = "Unknown section keyword: " + section + "\n";
    error(msg, location);
}


// -------------------------------------------------------------------------- //
//
void same_section_error(const std::string & section, const std::string & location)
{
    std::string msg = "Section " + section + " can only be defined once.\n";
    error(msg, location);
}


// -------------------------------------------------------------------------- //
//
void missing_keyword_error(const std::string & keyword, const std::string & location)
{
    std::string msg = "Required keyword " + keyword + " not found.\n";
    error(msg, location);
}


// -------------------------------------------------------------------------- //
//
void same_keyword_error(const std::string & keyword, const std::string & location)
{
    std::string msg = "Keyword " + keyword + " can only be definded once within this section.\n";
    error(msg, location);
}


// -------------------------------------------------------------------------- //
//
void unknown_keyword_error(const std::string & keyword, const std::string & location)
{
    std::string msg = "Unknown keyword: " + keyword + "\n";
    error(msg, location);
}


// -------------------------------------------------------------------------- //
//
void read_keyword_error(const std::string & filename,
                        const std::string & expected,
                        const std::string & found,
                        const std::string & location)
{
    std::string msg;
    msg = msg + "Error when reading from file: " + filename + "\n";
    msg = msg + "Expected : " + expected + "\n";
    msg = msg + "Found    : " + found + "\n";
    error(msg, location);
}


// -------------------------------------------------------------------------- //
//
void start_timer()
{
    time_t seconds;
    time(&seconds);
    time_0__ = seconds;
}

// -------------------------------------------------------------------------- //
//
int timer()
{
    time_t seconds;
    time(&seconds);
    return static_cast<int>(seconds-time_0__);
}

// -------------------------------------------------------------------------- //
//
void timestamp()
{
    time_t seconds;
    time(&seconds);
    tm* timeinfo;
    timeinfo = localtime(&seconds);
    std::string time_string(asctime(timeinfo));
    printf("Wall-Time Clock: %s\n", time_string.c_str());
}


// -------------------------------------------------------------------------- //
//
void error(const std::string & message,
           const std::string & location)
{
    throw IOException(message, location);
}

// -------------------------------------------------------------------------- //
//
void error_exit(const std::string & program)
{
    printf("                ...error-exiting %s\n", program.c_str());
    timestamp();
    exit(1);
}

// -------------------------------------------------------------------------- //
// Eats an input line (convenient for comments)
void eatline(std::ifstream& infile)
{
    char tmp_char;
    infile.get(tmp_char);
    while(tmp_char != '\n')
    {
        infile.get(tmp_char);
    }
}

// -------------------------------------------------------------------------- //
//
void print_success()
{
    printf("\n===============================================================================\n");
    // Make a timestamp on the screen.
    timestamp();
    // Exit message.
    printf("Exiting SpecSwap-RMC\n");
}

