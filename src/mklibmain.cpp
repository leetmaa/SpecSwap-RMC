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


/*! \file  mklibmain.cpp
 *  \brief File for the implementation of a simple command line interface to the
 *         Mklib library compiler utility.
 */

#include <string>
#include <cstdio>

#include "mklib.h"


// -------------------------------------------------------------------------- //
//
void print_help()
{
    printf("'mklib' is a command line tool for creating libraries for use\n");
    printf("        with the SpecSwap-RMC program.\n");
    printf("        Using Library class version %s\n", Mklib::get_lib_version().c_str());
    printf("Valid input options:\n");
    printf(" -c name\n");
    printf("        create .library file from .info specifications in ./name/name.info\n");
    printf(" -v, --version\n");
    printf("        prints Library class version information\n");
    printf(" -h, --help\n");
    printf("        prints this information\n");
    printf("For additional information contact your local SpecSwap-RMC expert.\n");
    printf("(c) 2012 Mikael Leetmaa");
}


// -------------------------------------------------------------------------- //
//
void print_version()
{
    printf("mklib: Using Library class version %s\n", Mklib::get_lib_version().c_str());
}


// -------------------------------------------------------------------------- //
//
int main(const int argc, const char *argv[])
{
    // Stop if no command-line arguments were given.
    if (argc == 1)
    {
        printf("mklib: Too few arguments.\nTry 'mklib -h' for more information.\n");
        return 1;
    }

    // Parse the option.
    std::string tmp_str(argv[1]);

    if (tmp_str[0] != '-' || tmp_str.size() == 1)
    {
        printf("mklib: Invalid input.\nTry 'mklib -h' for more information.\n");
        return 1;
    }

    // Check if we are parsing a long argument.
    if (tmp_str[1] == '-')
    {
        // now we are sure we have a '--'
        if (tmp_str.size() == 2)
        {
            printf("mklib: Invalid input.\nTry 'mklib -h' for more information.\n");
            return 1;
        }

        // Take the rest of the string as input.
        std::string sub_str = tmp_str.substr(2);

        if (sub_str == std::string("version"))
        {
            print_version();
            return 0;
        }

        else if (sub_str == std::string("help"))
        {
            print_help();
            return 0;
        }

        // If we get here then the input was unknown.
        printf("mklib: Invalid input.\nTry 'mklib -h' for more information.\n");
        return 1;
    }
    else
    {
        // now we are sure we have a '-Xxxx'

        // Stop if the string short option is too long.
        if (tmp_str.size() != 2)
        {
            printf("mklib: Invalid input.\nTry 'mklib -h' for more information.\n");
            return 1;
        }

        // now we are sure we have a '-X'

        if (tmp_str[1] == 'c')
        {
            // now we are sure we have '-c' == create option.

            if (argc != 3 )
            {
                printf("mklib -c: Wrong number of arguments.\nTry 'mklib -h' for more information.\n");
                return 1;
            }

            // Construct the mklib object with the '.info' file path.
            Mklib mklib;
            mklib.compile_library(argv[argc-1]);
            return 0;
        }
        else if (tmp_str[1] == 'h')
        {
            // now we are sure we have '-h'
            print_help();
            return 0;
        }
        else if (tmp_str[1] == 'v')
        {
            // now we are sure we have '-v'
            print_version();
            return 0;
        }

        // If we get here then the input was unknown.
        printf("mklib: Invalid input.\nTry 'mklib -h' for more information.\n");
        return 1;

    }

    return 0;
}



