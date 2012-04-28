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


/*! \file  mklib.cpp
 *  \brief File for the Mklib class implementation.
 */

#include <fstream>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>

#include "mklib.h"
#include "ioutils.h"
#include "ioexception.h"

// ----------------------------------------------------------------------------
//
Mklib::Mklib(const bool debug) :
    debug_(debug)
{
    // NOTHING HERE
}
// ----------------------------------------------------------------------------
//
void Mklib::compile_library(const std::string & basename)
{
    try
    {
        // Fill it with information by reading from the library setup file.
        Library library = library_from_info_file(basename);

        // Print to library to a file, named based on the library setup file.
        library.to_binary(basename.c_str());
    }
    catch (IOException & e)
    {
        e.print();
        error_exit("MKLIB");
    }

    // All done.
}

// ----------------------------------------------------------------------------
//
Library Mklib::library_from_info_file(const std::string & basename,
                                      const std::string & debugname)
{
    // Open the file to read.
    std::string filename(basename + "/" + basename + ".info");
    if(debug_)
    {
        filename = debugname;
    }

    check_path(filename, LOCATION);
    std::ifstream infile(filename.c_str());

    // Setup return data.
    Library library;

    // Set the name of the library.
    library.set_name(basename);

    // ------------------------------------------------------------------------
    // Read the fantastic fixed format input.

    // Setup keyword keys.
    std::string NCURVES_key("NCURVES");
    std::string CONVOLUTE_key("CONVOLUTE");
    std::string FORMAT_key("FORMAT");
    std::string START_key("START");
    std::string END_key("END");
    std::string SCALE_key("SCALE");
    std::string ENDING_key("ENDING");
    std::string NSCALARS_key("NSCALARS");
    std::string NAMES_key("NAMES");
    std::string NATOMTYPES_key("NATOMTYPES");
    std::string ATOMS_key("ATOMS");
    std::string MOL_key("MOL");

    // Temporary variables.
    std::string tmp, tmp1;
    int ncurves;

    // Read the NCURVES key.
    infile >> tmp;
    check_eof(infile, filename, LOCATION);
    check_keyword(NCURVES_key, tmp, filename, LOCATION);

    // Get the NCURVES value.
    infile >> ncurves;
    check_eof(infile, filename, LOCATION);
    check_positive_integer(ncurves, filename, LOCATION);
    printf("%s %i\n", NCURVES_key.c_str(), ncurves);

    // Store ncurves on the library.
    library.set_ncurves(ncurves);

    // Loop over all curves.
    for (int i = 0; i < ncurves; ++i)
    {
        // Temporary storage for the 'no' parameters.
        double no1, no2, no3, no4;

        // Read the CONVOLUTE key.
        infile >> tmp;
        check_eof(infile, filename, LOCATION);
        check_keyword(CONVOLUTE_key, tmp, filename, LOCATION);

        infile >> no1;
        check_eof(infile, filename, LOCATION);
        infile >> no2;
        check_eof(infile, filename, LOCATION);
        infile >> no3;
        check_eof(infile, filename, LOCATION);
        infile >> no4;
        check_eof(infile, filename, LOCATION);
        printf("%s %f %f %f %f\n", CONVOLUTE_key.c_str(), no1, no2, no3, no4);
        // Store on the library.
        library.add_convolute(no1, no2, no3, no4);

        // Read the FORMAT keyword.
        infile >> tmp;
        check_eof(infile, filename, LOCATION);
        check_keyword(FORMAT_key, tmp, filename, LOCATION);

        double start, stop, step;
        infile >> start;
        check_eof(infile, filename, LOCATION);
        infile >> stop;
        check_eof(infile, filename, LOCATION);
        infile >> step;
        check_eof(infile, filename, LOCATION);
        printf("%s %f %f %f\n", FORMAT_key.c_str(), start, stop, step);
        // Store on the library.
        library.add_format(start, stop, step);

        // Read the SCALE keyword.
        infile >> tmp;
        check_eof(infile, filename, LOCATION);
        check_keyword(SCALE_key, tmp, filename, LOCATION);

        std::string scale;
        infile >> scale;
        check_eof(infile, filename, LOCATION);
        printf("%s %s\n", SCALE_key.c_str(), scale.c_str());

        // Skip the reading if we are in debug mode.
        if (debug_)
        {
            printf("library.add_scale(%s)\n", scale.c_str());
        }
        else
        {
            library.add_scale(scale);
        }

        // Read the ENDING keyword.
        infile >> tmp;
        check_eof(infile, filename, LOCATION);
        check_keyword(ENDING_key, tmp, filename, LOCATION);

        infile >> tmp;
        check_eof(infile, filename, LOCATION);
        printf("%s %s\n",ENDING_key.c_str(), tmp.c_str());
        library.add_ending(tmp);
    }

    // Read the NSCALARS keyword.
    infile >> tmp;
    check_eof(infile, filename, LOCATION);
    check_keyword(NSCALARS_key, tmp, filename, LOCATION);

    int nscalars;
    infile >> nscalars;
    check_eof(infile, filename, LOCATION);
    check_positive_integer(nscalars, filename, LOCATION);

    printf("%s %i\n", NSCALARS_key.c_str(), nscalars);
    library.set_nscalars(nscalars);

    if (nscalars > 0)
    {
        // Read the NAMES keyword.
        infile >> tmp;
        check_eof(infile, filename, LOCATION);
        check_keyword(NAMES_key, tmp, filename, LOCATION);
        printf("%s", NAMES_key.c_str());

        for (int i = 0; i < nscalars; ++i)
        {
            infile >> tmp;
            check_eof(infile, filename, LOCATION);
            library.add_scalar_name(tmp);
            printf(" %s", tmp.c_str());
        }
        printf("\n");
    }

    // Read the NATOMTYPES keyword.
    infile >> tmp;
    check_eof(infile, filename, LOCATION);
    check_keyword(NATOMTYPES_key, tmp, filename, LOCATION);
    int natomtypes;
    infile >> natomtypes;
    check_eof(infile, filename, LOCATION);
    check_positive_integer(natomtypes, filename, LOCATION);
    printf("%s %i", NATOMTYPES_key.c_str(), natomtypes);

    // Read the atom types.
    std::vector<std::string> atomtypes;
    for (int i = 0; i < natomtypes; ++i)
    {
        infile >> tmp;
        check_eof(infile, filename, LOCATION);
        atomtypes.push_back(tmp);
        printf(" %s", tmp.c_str());
    }
    printf("\n");

    // Store on the library.
    library.set_atomtypes(atomtypes);

    // Read the ATOMS or the START key.
    infile >> tmp;
    check_eof(infile, filename, LOCATION);
    if (tmp != ATOMS_key && tmp != START_key )
    {
        std::string expected = ATOMS_key + " or " + START_key;
        read_keyword_error(filename, expected, tmp, LOCATION);
    }

    // Read the START or the ATOMS keyword.
    if (tmp == ATOMS_key)
    {
        printf("%s ", ATOMS_key.c_str());

        // Temporary storage.
        std::vector<int> atoms_per_type;
        // Read the general atoms info.
        for (int i = 0; i < natomtypes+1; ++i)
        {
            // NOTE: The first element to read is the total number of atoms.
            int tmp_int;
            infile >> tmp_int;
            check_eof(infile, filename, LOCATION);
            check_positive_integer(tmp_int, filename, LOCATION);
            printf(" %i", tmp_int);
            atoms_per_type.push_back(tmp_int);
        }
        printf("\n");

        // Read the central molecule.
        infile >> tmp;
        check_eof(infile, filename, LOCATION);
        check_keyword(MOL_key, tmp, filename, LOCATION);
        printf ("%s ", MOL_key.c_str());

        int natoms_in_molecule;
        infile >> natoms_in_molecule;
        check_eof(infile, filename, LOCATION);
        check_positive_integer(natoms_in_molecule, filename, LOCATION);
        printf (" %i", natoms_in_molecule);

        std::vector<int> central_molecule;
        for (int i = 0; i < natoms_in_molecule; ++i)
        {
            int tmp_int;
            infile >> tmp_int;
            check_eof(infile, filename, LOCATION);
            check_positive_integer(tmp_int, filename, LOCATION);
            central_molecule.push_back(tmp_int);
            printf (" %i", tmp_int);
        }
        printf("\n");

        // Set the data on the library.
        library.set_atoms_info(atoms_per_type,
                               central_molecule);

        // Read the START keyword.
        infile >> tmp;
        check_eof(infile, filename, LOCATION);
        check_keyword(START_key, tmp, filename, LOCATION);
    }

    // Read the number of basis functions (START keyword).
    int nbase;
    infile >> nbase;
    check_eof(infile, filename, LOCATION);
    check_positive_integer(nbase, filename, LOCATION);
    printf("%s %i\n", START_key.c_str(), nbase);
    library.set_nbase(nbase);

    for (int i = 0; i < nbase; ++i)
    {
        infile >> tmp;
        check_eof(infile, filename, LOCATION);
        printf("%s\n", tmp.c_str());
        // Make sure we flush after every filename, for finding errors easier.
        std::cout.flush();
        if (tmp == END_key)
        {
            std::string expected = "A basis name.";
            read_keyword_error(filename, expected, tmp, LOCATION);
        }

        // Read in the base.
        if (debug_)
        {
            // If in debug mode, print out the function call here.
            printf("library.add_base(%s)\n", tmp.c_str());
        }
        else
        {
            // Not in debug mode, make the actual call.
            library.add_base(tmp);
        }
    }

    // Read the END keyword.
    infile >> tmp;
    check_eof(infile, filename, LOCATION);
    check_keyword(END_key, tmp, filename, LOCATION);
    printf("%s\n", END_key.c_str());

    // Close the file. All done.
    infile.close();
    printf("All input for library %s read.\n", filename.c_str());

    // All done, return the library.
    library.complete_setup();
    return library;
}


// ----------------------------------------------------------------------------
//
void Mklib::check_keyword(const std::string & key,
                          const std::string & found,
                          const std::string & filename,
                          const std::string & location)
{
    if (found != key)
    {
        read_keyword_error(filename, key, found, location);
    }
}
