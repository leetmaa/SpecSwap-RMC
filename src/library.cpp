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


/*! \file  library.cpp
 *  \brief File for the Library class definition.
 */

// ----------------------------------------------------------- //
// Naming convension for setup functions:                      //
// 'set' functions sets values and should only be called once. //
// 'add' functions makes push_back operations on member data.  //
// ----------------------------------------------------------- //

#include <algorithm>
#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdio>
#include <cstdlib>

#include "ioutils.h"
#include "mathutils.h"
#include "library.h"

// Define the version string.
// NOTE : The version string must be 11 characters long for the binary IO to
//        work. This should be changed/fixed.
const std::string Library::version_ = "version-3.0";


// -------------------------------------------------------------------------- //
//
Library::Library() :
    nadded_scale_(0),
    nadded_(0),
    nbase_(0),
    ntypes_(0),
    npairs_(0),
    ncurves_(0),
    nscalars_(0),
    use_atoms_info_(false),
    setup_done_(false)
{
    // NOTHING HERE
}


// -------------------------------------------------------------------------- //
//
Library::Library(const std::string & filename) :
    nadded_scale_(0),
    nadded_(0),
    npairs_(0),
    ncurves_(0),
    nscalars_(0),
    use_atoms_info_(false),
    setup_done_(false)
{
    // Setup from the binary library file.
    from_binary(filename);
    setup_done_ = true;
}


// -------------------------------------------------------------------------
// Functions for setup from Mklib
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
//
void Library::set_name(const std::string& name)
{
    // Only to be used for setup.
    check_setup(FUNCTION, LOCATION);
    // Set the name of the library.
    name_ = name;
}

// -------------------------------------------------------------------------
//
void Library::set_ncurves(const int ncurves)
{
    // Only to be used for setup.
    check_setup(FUNCTION, LOCATION);
    // Set the number of curves on the class.
    ncurves_ = ncurves;
}

// -------------------------------------------------------------------------
//
void Library::add_convolute(const double no1,
                            const double no2,
                            const double no3,
                            const double no4)
{
    // Only to be used for setup.
    check_setup(FUNCTION, LOCATION);
    // Add the parameters to the no1, no2, no3 and no4 vectors.
    no1_.push_back(no1);
    no2_.push_back(no2);
    no3_.push_back(no3);
    no4_.push_back(no4);
}

// -------------------------------------------------------------------------
//
void Library::add_format(const double start,
                         const double stop,
                         const double step)
{
    // Only to be used for setup.
    check_setup(FUNCTION, LOCATION);
    // Add the parameters.
    start_.push_back(start);
    stop_.push_back(stop);
    step_.push_back(step);
    dimension_.push_back(static_cast<int>(step));
}

// -------------------------------------------------------------------------
//
void Library::add_scale(const std::string& s)
{
    // Only to be used for setup.
    check_setup(FUNCTION, LOCATION);
    read_scale(name_+"/"+s);
    nadded_scale_ += 1;
}

// -------------------------------------------------------------------------
//
void Library::add_ending(const std::string& ending)
{
    // Only to be used for setup.
    check_setup(FUNCTION, LOCATION);
    // Add curve ending.
    endings_.push_back(ending);
}

// -------------------------------------------------------------------------
//
void Library::set_nscalars(const int nscalars)
{
    // Only to be used for setup.
    check_setup(FUNCTION, LOCATION);
    // Set the number pf scalars on the class.
    nscalars_ = nscalars;
}

// -------------------------------------------------------------------------
//
void Library::add_scalar_name(const std::string& name)
{
    // Only to be used for setup.
    check_setup(FUNCTION, LOCATION);
    // Add this scalar name to the end of the vector holding the scalar names.
    scalar_names_.push_back(name);
}

// -------------------------------------------------------------------------
//
void Library::set_atomtypes(const std::vector<std::string>& atomtypes)
{
    // Only to be used for setup.
    check_setup(FUNCTION, LOCATION);
    // Set the atomtypes on the class.
    atomtypes_ = atomtypes;
    ntypes_ = atomtypes_.size();
    npairs_ = (((ntypes_ * ntypes_) - ntypes_) / 2 ) + ntypes_;
    // The index matrix can be setup as soon as we know the number of atom types.
    index_matrix_ = setup_index_matrix(ntypes_);
}

// -------------------------------------------------------------------------
//
void Library::set_atoms_info(const std::vector<int>& atoms_per_type,
                             const std::vector<int>& central_molecule)
{
    // Only to be used for setup.
    check_setup(FUNCTION, LOCATION);
    // Set the flag for knowing that atoms info were set.
    use_atoms_info_ = true;
    // Set the atoms per type on the class.
    atoms_per_type_ = atoms_per_type;
    central_molecule_ = central_molecule;
}

// -------------------------------------------------------------------------
//
void Library::set_nbase(const int nbase)
{
    // Only to be used for setup.
    check_setup(FUNCTION, LOCATION);
    // Set the number of basis functions.
    nbase_ = nbase;
    base_.resize(nbase_);
}

// -------------------------------------------------------------------------
//
void Library::add_base(const std::string& basename)
{
    // Only to be used for setup.
    check_setup(FUNCTION, LOCATION);

    // Save the name.
    names_.push_back(basename);

    // Read curves.
    for (int i = 0; i < ncurves_; ++i)
    {
        // Setup the name and read.
        const std::string filename = name_ + "/" + basename + "." + endings_[i];
        add_curve(filename, i);
    }

    // Read scalars.
    if (nscalars_ > 0)
    {
        // Setup the name and read.
        const std::string filename = name_ + "/" + basename + "." + "sclr";
        add_scalars(filename);
    }

    // Read the geometry.
    {
        // Setup the name and read.
        const std::string filename = name_ + "/" + basename + ".xyz";
        add_geometry(filename);
        // Calculate partials and internals.
        calculate_partials();
        calculate_internals();
    }

    // Additional setup here if needed.

    // Increment the counter.
    nadded_ += 1;

}

// -------------------------------------------------------------------------
//
void Library::complete_setup()
{
    // Only to be used for setup.
    check_setup(FUNCTION, LOCATION);
    setup_done_ = true;
}


// -------------------------------------------------------------------------
// Other utility function and functions for binary IO.
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
//
void Library::add_curve(const std::string& filename,
                        const int pos)
{
    // Open the file.
    check_path(filename, LOCATION);
    std::ifstream infile(filename.c_str());

    // For temporary storage.
    std::vector<double> tmp_data(dimension_[pos]);

    // Read the first (scale) value.
    double first_value;
    infile >> first_value;
    check_eof(infile, filename, LOCATION);

    // Check against expected start value.
    if (first_value != start_[pos])
    {
        const std::string msg = "Mismatch expected START value and the\nfirst value in file " + filename + "\n";
        error(msg, LOCATION);
    }

    // Read the first curve value.
    infile >> tmp_data[0];
    check_eof(infile, filename, LOCATION);

    // Read all other values.
    for (int i = 1; i < dimension_[pos]-1; ++i)
    {
        // Throw away the scale 'dummy'
        double dummy;
        infile >> dummy;
        check_eof(infile, filename, LOCATION);
        // Save the curve value.
        infile >> tmp_data[i];
        check_eof(infile, filename, LOCATION);
    }

    // Read and check the last value.
    double last_value;
    infile >> last_value;
    check_eof(infile, filename, LOCATION);

    if (last_value != stop_[pos])
    {
        const std::string msg = "Mismatch expected STOP value and the\nlast value in file " + filename + "\n";
        error(msg, LOCATION);
    }

    // Read the last value.
    infile >> tmp_data[dimension_[pos]-1];
    check_eof(infile, filename, LOCATION);

    // Save the curve.
    base_[nadded_].push_back(tmp_data);

    // Close the file.
    infile.close();
}


// -------------------------------------------------------------------------
//
void Library::add_scalars(const std::string& filename)
{
    // Open the file.
    check_path(filename, LOCATION);
    std::ifstream infile(filename.c_str());

    // Data to read in.
    std::vector<double> tmp_data;

    // For each scalar.
    for (int i = 0; i < nscalars_; ++i)
    {
        double value;
        infile >> value;
        check_eof(infile, filename, LOCATION);
        tmp_data.push_back(value);
    }
    scalars_.push_back(tmp_data);

    // Close.
    infile.close();
}


// -------------------------------------------------------------------------
//
void Library::add_geometry(const std::string& filename)
{
    // Open the file.
    check_path(filename, LOCATION);
    std::ifstream infile(filename.c_str());

    // Read the number of atoms.
    int tmp_natoms;
    infile >> tmp_natoms;
    check_eof(infile, filename, LOCATION);

    // Note that the first element in 'atoms_per_type'
    // holds the total number of atoms in this basis element.
    std::vector<int> atoms_per_type;
    std::vector<int> mol_vec;

    // If we have read the atoms per type in the setup file.
    if (use_atoms_info_)
    {
        atoms_per_type = atoms_per_type_;
        mol_vec = central_molecule_;
    }

    // If we read atoms per type for each file.
    else
    {
        // Save natoms as the first element in the atoms per type vector.
        atoms_per_type.push_back(tmp_natoms);

        // Read the number of atoms for each type.
        int sum = 0;
        for (size_t i = 0; i < atomtypes_.size(); ++i)
        {
            int tmp_apt;
            infile >> tmp_apt;
            check_eof(infile, filename, LOCATION);
            atoms_per_type.push_back(tmp_apt);
            sum += tmp_apt;
        }

        // Check that the sum is correct.
        if (atoms_per_type[0] != sum)
        {
            const std::string msg = "Wrong number of atoms per type\nwhen reading file: " + filename + "\n";
            error(msg, LOCATION);
        }

        // Read the definition of the central molecule.
        std::string tmp_str;
        std::string MOL_key("MOL");
        infile >> tmp_str;
        check_eof(infile, filename, LOCATION);

        // Check that it says MOL in the file.
        if (tmp_str != MOL_key)
        {
            read_keyword_error(filename, MOL_key, tmp_str, LOCATION);
        }

        // Read the number of atoms in the central molecule.
        int tmp_nmol;
        infile >> tmp_nmol;
        check_eof(infile, filename, LOCATION);
        // Check that this is a positive interger.

        // Read the indices of the central molecule.
        for (int i = 0; i < tmp_nmol; ++i)
        {
            int index;
            infile >> index;
            check_eof(infile, filename, LOCATION);
            mol_vec.push_back(index);
        }
    }

    // Add the atoms per typ and central molecule.
    natoms_.push_back(atoms_per_type);
    central_mol_.push_back(mol_vec);

    // Read in the geometry.
    Matrix GEO(atoms_per_type[0], 3);
    int atom_index = -1;

    // For each atom type.
    for (int i = 0; i < (int) atoms_per_type.size() - 1; ++i)
    {
        // for each atom of type 'i':
        for (int j = 0; j < (int) atoms_per_type[i+1]; ++j)
        {
            atom_index += 1;

            // Read the atom type.
            std::string tmp_str;
            infile >> tmp_str;
            check_eof(infile, filename, LOCATION);

            // Check the atom type.
            if (tmp_str != atomtypes_[i])
            {
                const std::string msg = "Wrong atom type encountered.\nCheck number of atoms per type in file: " + filename + "\n";
                error(msg, LOCATION);
            }

            // Read the coordinates.
            for (int k = 0; k < 3; ++k)
            {
                infile >> GEO(atom_index,k);
                check_eof(infile, filename, LOCATION);
            }
        }
    }

    // Save the geometry.
    geo_.push_back(GEO);

    // Close the file.
    infile.close();

}


// -------------------------------------------------------------------------
//
void Library::calculate_partials()
{
    // Calculate and store intermolecular distance dirstributions
    // for the latest added geometry.

    // NOTE: This function can probably be done much more efficiently,
    //       but it is in no way performance sensitive.

    // This is a help indexing vector, storing the index of the first
    // atom of each type.
    std::vector<int> first_index;
    first_index.push_back(0);
    int tmp_sum = 0;
    for (size_t i = 1; i < natoms_[nadded_].size(); ++i)
    {
        tmp_sum += natoms_[nadded_][i];
        first_index.push_back(tmp_sum);
    }

    // Setup a vector of partial pair correlation functions for this geometry.
    std::vector< std::vector<double> > this_partials(npairs_);

    // For all atoms in the central molecule.
    for (size_t i = 0; i < central_mol_[nadded_].size(); ++i)
    {
        const int index1 = central_mol_[nadded_][i];
        int type1 = -1;

        // Determine the type.
        for (int j = 0; j < (int)atomtypes_.size(); ++j)
        {
            if (index1 < first_index[j+1])
            {
                type1 = j;
                break;
            }
        }

        // For all types.
        for (size_t j = 0; j < atomtypes_.size(); ++j)
        {
            const int type2 = j;

            // For all atoms of this type.
            for (int k = 0; k < natoms_[nadded_][type2 + 1]; ++k)
            {
                const int index2 = first_index[type2] + k;

                // Now we have index1, type1, index2, type2.

                // Make sure index2 is not part of the central molecule.
                bool pass_flag = true;
                for (size_t l = 0; l < central_mol_[nadded_].size(); ++l)
                {
                    if (central_mol_[nadded_][l] == index2)
                    {
                        pass_flag = false;
                        break;
                    }
                }

                if (pass_flag)
                {
                    // Calculate and store the distance in the correct temporary partial.
                    double atom1[3];
                    double atom2[3];

                    for (int l = 0; l < 3; ++l)
                    {
                        atom1[l] = geo_[nadded_](index1,l);
                        atom2[l] = geo_[nadded_](index2,l);
                    }

                    const double distance = calculate_distance(atom1, atom2);
                    const int pos = index_matrix_(type1,type2);
                    this_partials[pos].push_back(distance);
                }
            }
        }
    }

    // Sort the partials.
    for (int i = 0; i < npairs_; ++i)
    {
        std::sort(this_partials[i].begin(), this_partials[i].end());
    }

    partials_.push_back(this_partials);
}


// -------------------------------------------------------------------------
//
void Library::calculate_internals()
{
    // Calculate and store intramolecular distance distributions.
    std::vector< std::vector<double> > this_internals(npairs_);

    // This is a help indexing vector, storing the index of the first
    // atom of each type.

    // FIXME: Should go as a separate function, since also calculate_partials()
    // uses it.
    std::vector<int> first_index;
    first_index.push_back(0);
    int tmp_sum = 0;
    for (size_t i = 1; i < natoms_[nadded_].size(); ++i)
    {
        tmp_sum += natoms_[nadded_][i];
        first_index.push_back(tmp_sum);
    }

    // Loop over all atoms in the central molecule except the last.
    for (size_t i = 0; i < central_mol_[nadded_].size() - 1; ++i)
    {
        const int index1 = central_mol_[nadded_][i];
        int type1 = -1;

        // Determine the type.
        for (size_t j = 0; j < atomtypes_.size(); ++j)
        {
            if (index1 < first_index[j+1])
            {
                type1 = j;
                break;
            }
        }

        // Loop over the rest of the atoms in the central molecule.
        for (size_t j = i+1; j < central_mol_[nadded_].size(); ++j)
        {
            const int index2 = central_mol_[nadded_][j];
            int type2 = -1;

            // Determine the type.
            for (size_t k = 0; k < atomtypes_.size(); ++k)
            {
                if (index2 < first_index[k+1])
                {
                    type2 = k;
                    break;
                }
            }

            // Calculate and store the distance in the correct temp internal.
            double atom1[3];
            double atom2[3];

            for (int k = 0; k < 3; ++k)
            {
                atom1[k] = geo_[nadded_](index1,k);
                atom2[k] = geo_[nadded_](index2,k);
            }

            const double distance = calculate_distance(atom1, atom2);
            const int pos = index_matrix_(type1,type2);
            this_internals[pos].push_back(distance);
        }
    }

    // Sort the internals.
    for (int i = 0; i < npairs_; ++i)
    {
        std::sort(this_internals[i].begin(), this_internals[i].end());
    }

    // Store.
    internals_.push_back(this_internals);
}


// -------------------------------------------------------------------------
//
//
void Library::read_scale(const std::string& filename)
{
    // Open the file.
    check_path(filename, LOCATION);
    std::ifstream infile(filename.c_str());

    // Shorthand for the position.
    const int pos = nadded_scale_;

    // Read number of steps info.
    int steps;
    infile >> steps;
    check_eof(infile, filename, LOCATION);
    std::vector<double> tmp_data(dimension_[pos]);

    if (steps != step_[pos])
    {
        const std::string msg = "Mismatch between defined STEP value and\nnumber of steps as read in from file: \n" + filename;
        error(msg, LOCATION);
    }

    // Start reading the values.
    double first_value;
    infile >> first_value;
    check_eof(infile, filename, LOCATION);
    tmp_data[0] = first_value;

    if (first_value != start_[pos])
    {
        const std::string msg = "Mismatch between defined START value and\nthe first scale value in file: " + filename + "\n";
        error(msg, LOCATION);
    }

    // Read all other values.
    for (int i = 1; i < dimension_[pos]-1; ++i)
    {
        double dummy;
        infile >> dummy;
        check_eof(infile, filename, LOCATION);
        infile >> tmp_data[i];
        check_eof(infile, filename, LOCATION);
    }

    // Read and check the last value.
    double dummy;
    infile >> dummy;
    double last_value;
    infile >> last_value;
    tmp_data[dimension_[pos]-1] = last_value;

    if (last_value != stop_[pos])
    {
        const std::string msg = "Mismatch between defined STOP value\nand the last value in file: " + filename + "\n";
        error(msg, LOCATION);
    }

    scale_.push_back(tmp_data);
    infile.close();
}


// -------------------------------------------------------------------------- //
// Functions for binary IO
// -------------------------------------------------------------------------- //
//
 void Library::to_binary(const std::string & filename) const
{
    if (nbase_ != nadded_)
    {
        const std::string msg("Can not write binary file. Some basis functions are yet to be added.\n");
        error(msg, LOCATION);
    }

    // We are ready to write to file.
    const std::string full_filename = filename + ".library";
    std::ofstream outfile(full_filename.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);

    if (outfile.bad())
    {
        std::string msg("Unable to write to file: ");
        msg += full_filename;
        error(msg, LOCATION);
    }

    printf("succesfully opened file %s for writing\n", full_filename.c_str());

    int size;
    int* memblock_i;
    double* memblock_d;

    // write version ------------------------------

    outfile.write((char*)version_.c_str(), 12*sizeof(char));

    // write 'ncurves' and 'nscalars' --------------------------

    outfile.write((char*)&ncurves_, sizeof(int));
    outfile.write((char*)&nscalars_, sizeof(int));


    // write 'endings' ------------------------

    for (int i = 0; i < (int)endings_.size(); ++i)
    {
        size = endings_[i].size() + 1;
        outfile.write((char*)&size, sizeof(int));
        outfile.write((char*)endings_[i].c_str(), size*sizeof(char));
    }

    // write 'scalar_names' ---------------------------

    for (int i = 0; i < (int)scalar_names_.size(); ++i)
    {
        size = scalar_names_[i].size() + 1;
        outfile.write((char*)&size, sizeof(int));
        outfile.write((char*)scalar_names_[i].c_str(), size*sizeof(char));
    }

    // write CONVOLUTE parameters and 'start', 'stop', 'step' and 'dimension'
    for (int i = 0; i < ncurves_; ++i)
    {
        size = 8;
        double row[8];

        row[0] = no1_[i];
        row[1] = no2_[i];
        row[2] = no3_[i];
        row[3] = no4_[i];
        row[4] = start_[i];
        row[5] = stop_[i];
        row[6] = step_[i];

        // Watch out! this is an implicit type cast.
        row[7] = dimension_[i];
        outfile.write((char*)row, size*sizeof(double));
        size = scale_[i].size();
        outfile.write((char*)&size, sizeof(int));
        outfile.write((char*)(&scale_[i][0]), size*sizeof(double));
    }


    // write 'name' -----------------------------

    size = name_.size() + 1;
    outfile.write((char*)&size, sizeof(int));
    outfile.write((char*)name_.c_str(), size*sizeof(char));
    outfile.write((char*)&nbase_, sizeof(int));

    // write 'natoms' ------------------------

    int rows = natoms_.size();
    int cols = natoms_[0].size();

    size = rows * cols;

    memblock_i = new int[size];

    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            memblock_i[i*cols+j] = natoms_[i][j];
        }
    }

    outfile.write((char*)&rows, sizeof(int));
    outfile.write((char*)&cols, sizeof(int));
    outfile.write((char*)memblock_i, size*sizeof(int));

    delete [] memblock_i;


    // write 'names' ----------------------------------

    for (int i = 0; i < (int)names_.size(); ++i)
    {
        size = names_[i].size() + 1;
        outfile.write((char*)&size, sizeof(int));
        outfile.write((char*)names_[i].c_str(), size*sizeof(char));
    }

    // write 'base' i.e. the "curves" -----------------

    // The dimensions here are known,
    //  base_.size() == nbase_
    //  base_[i].size() == ncurves_
    //  base_[i][j].size() = dimension_[j]

    for (int i = 0; i < nbase_; ++i)
    {
        for (int j = 0; j < ncurves_; ++j)
        {
            size = dimension_[j];
            outfile.write((char*)(&base_[i][j][0]), size*sizeof(double));
        }
    }

    // write 'atomtypes' -----------------------------

    outfile.write((char*)&ntypes_, sizeof(int));

    for (int i = 0; i < ntypes_; ++i)
    {
        size = atomtypes_[i].size() + 1;
        outfile.write((char*)&size, sizeof(int));
        outfile.write((char*)atomtypes_[i].c_str(), size*sizeof(char));
    }

    // write 'partials' ------------------------------

    for (int i = 0; i < nbase_; ++i)
    {
        for (int j = 0; j < npairs_; ++j)
        {
            size = partials_[i][j].size();
            outfile.write((char*)&size, sizeof(int));
            outfile.write((char*)(&partials_[i][j][0]), size*sizeof(double));
        }
    }

    // write 'internals' ----------------------------

    for (int i = 0; i < nbase_; ++i)
    {
        for (int j = 0; j < npairs_; ++j)
        {
            size = internals_[i][j].size();
            outfile.write((char*)&size, sizeof(int));
            outfile.write((char*)(&internals_[i][j][0]), size*sizeof(double));
        }
    }

    // write 'scalars' ------------------------------------

    size = nscalars_;

    memblock_d = new double[size];

    for (int i = 0; i < nbase_; ++i)
    {
        for (int j = 0; j < nscalars_; ++j)
        {
            memblock_d[j] = scalars_[i][j];
        }
        outfile.write((char*)memblock_d, size*sizeof(double));
    }

    delete [] memblock_d;


    // write 'geo' ----------------------------------------

    for (int i = 0; i < nbase_; ++i)
    {
        const int rows = natoms_[i][0];
        const int cols = 3;
        const int size = rows*cols;
        std::vector<double> data(size);

        for (int j = 0; j < rows; ++j)
        {
            for (int k = 0; k < cols; ++k)
            {
                data[j*cols+k] = geo_[i](j,k);
            }
        }

        outfile.write((char*)(&data[0]), size*sizeof(double));
    }

    // write 'central_mol' --------------------------------

    for (int i = 0; i < nbase_; ++i)
    {
        size = central_mol_[i].size();
        memblock_i = new int[size];

        for (int j = 0; j < size; ++j)
        {
            memblock_i[j] = central_mol_[i][j];
        }
        outfile.write((char*)&size, sizeof(int));
        outfile.write((char*)memblock_i, size*sizeof(int));

        delete [] memblock_i;
    }

    outfile.close();

}


// -------------------------------------------------------------------------- //
//
void Library::from_binary(const std::string & filename)
{
    std::string filename_lib = filename + ".library";

    // Flag for debug output.
    bool trace = false;

    // Open the file.
    std::ifstream in(filename_lib.c_str(), std::ios::in | std::ios::binary);

    // Check that the file was sucessfully opened.
    check_eof(in, filename_lib, LOCATION);

    int size;

    // Read version --------------
    const int toread = 12;
    std::string version;
    char* memblock_c = new char[toread];
    in.read((char*)memblock_c, toread*sizeof(char));
    check_eof(in, filename_lib, LOCATION);
    version = memblock_c;
    delete [] memblock_c;

    if (trace)
    {
        printf("read version: %s\n", version.c_str());
    }

    if (version != version_)
    {
        error(std::string("Error when reading file: " + filename_lib  + \
                          "\nNot a compatible library file.\n"),
              LOCATION);
    }

    printf("Reading %s binary library file: %s\n", version_.c_str(), filename_lib.c_str());

    // Read 'ncurves' and 'nscalars' ------------------

    in.read((char*)&ncurves_, sizeof(int));
    in.read((char*)&nscalars_, sizeof(int));

    if (trace)
    {
        printf("read ncurves: %i\n", ncurves_);
        printf("read nscalars: %i\n", nscalars_);
    }

    // Read 'endings' -------------------

    for (int i = 0; i < ncurves_; ++i)
    {
        in.read((char*)&size, sizeof(int));
        memblock_c = new char[size];
        in.read((char*)memblock_c, size*sizeof(char));
        std::string tmp_str(memblock_c);
        endings_.push_back(tmp_str);
        delete [] memblock_c;
    }

    if (trace)
    {
        printf("read endings\n");
        for (int i = 0; i < (int)endings_.size(); ++i)
        {
            printf("%s\n", endings_[i].c_str());
        }
    }

    // Read 'scalar_names' ------------------

    if (trace)
    {
        printf("reading 'scalar_names'\n");
    }

    std::vector< std::string > tmp_vec2;

    for (int i = 0; i < nscalars_; ++i)
    {
        in.read((char*)&size, sizeof(int));
        memblock_c = new char[size];
        in.read((char*)memblock_c, size*sizeof(char));
        std::string tmp_str(memblock_c);
        scalar_names_.push_back(tmp_str);
        delete [] memblock_c;
    }

    if (trace)
    {
        for (int i = 0; i < (int)scalar_names_.size(); ++i)
        {
            printf("%s\n", scalar_names_[i].c_str());
        }
    }


    // Read CONVOLUTE parameters, and 'start', 'stop', 'step' and 'dimension' ---
    for (int i = 0; i < ncurves_; ++i)
    {
        size = 8;
        double row[8];
        in.read((char*)row, size*sizeof(double));

        if (trace)
        {
            for (int j = 0; j < 8; ++j)
                printf("%f ", row[j]);
            printf("\n");
        }

        no1_.push_back(row[0]);
        no2_.push_back(row[1]);
        no3_.push_back(row[2]);
        no4_.push_back(row[3]);
        start_.push_back(row[4]);
        stop_.push_back(row[5]);
        step_.push_back(row[6]);
        dimension_.push_back((int)row[7]);

        in.read((char*)&size, sizeof(int));

        if (trace)
        {
            printf("scale dimension as read in: %i\n", size);
        }
        std::vector<double> tmp_scale(size);
        in.read((char*)(&tmp_scale[0]), size*sizeof(double));
        scale_.push_back(tmp_scale);
    }

    // Read 'name' --------------------------------------

    in.read((char*)&size, sizeof(int));
    memblock_c = new char[size];
    in.read((char*)memblock_c, size*sizeof(char));
    name_ = memblock_c;
    delete [] memblock_c;

    // Read 'nbase' ------------------------------

    in.read((char*)&nbase_, sizeof(int));

    if (trace)
    {
        printf("%i\n", nbase_);
    }

    // Read 'natoms' -------------------------------

    int rows;
    int cols;

    in.read((char*)&rows, sizeof(int));
    in.read((char*)&cols, sizeof(int));

    size = rows * cols;

    int* memblock_i = new int[size];

    in.read((char*)memblock_i, size*sizeof(int));

    for (int i = 0; i < rows; ++i)
    {
        std::vector<int> tmp_vec;

        for (int j = 0; j < cols; ++j)
        {
            tmp_vec.push_back(memblock_i[i*cols+j]);
        }
        natoms_.push_back(tmp_vec);
    }

    if (trace)
    {
        for (int i = 0; i < (int)natoms_.size(); ++i)
        {
            for (int j = 0; j < (int)natoms_[0].size(); ++j)
            {
                printf("%i ", natoms_[i][j]);
            }
        }
        printf("\n");
    }

    // Read 'names' ----------------------------------------

    for (int i = 0; i < nbase_; ++i)
    {
        in.read((char*)&size, sizeof(int));
        memblock_c = new char[size];
        in.read((char*)memblock_c, size*sizeof(char));
        std::string tmp_str(memblock_c);
        delete [] memblock_c;
        names_.push_back(tmp_str);
    }

    // Read 'base' i.e. the "curves" -----------------

    // the dimensions here are known,
    //  base.size() == nbase_
    //  base[i].size() == ncurves_
    //  base[i][j].size() = dimension_[j]

    for (int i = 0; i < nbase_; ++i)
    {
        std::vector< std::vector< double> > tmp_vec;

        // make sure 'dimension' is really read before we try to read this
        for (int j = 0; j < ncurves_; ++j)
        {
            size = dimension_[j];
            std::vector<double> tmp_curve(size);
            in.read((char*)(&tmp_curve[0]), size*sizeof(double));
            tmp_vec.push_back(tmp_curve);
        }

        base_.push_back(tmp_vec);
    }


    // Read 'atomtypes' -----------------------------

    in.read((char*)&ntypes_, sizeof(int));

    index_matrix_ = setup_index_matrix(ntypes_);
    npairs_ = (((ntypes_ * ntypes_) - ntypes_) / 2 ) + ntypes_;

    if (trace)
    {
        printf("%i\n", ntypes_);
    }

    for (int i = 0; i < ntypes_; ++i)
    {
        in.read((char*)&size, sizeof(int));
        memblock_c = new char[size];
        in.read((char*)memblock_c, size*sizeof(char));
        std::string tmp_str(memblock_c);
        atomtypes_.push_back(tmp_str);
    }

    // Read 'partials' ----------------------------

    for (int i = 0; i < nbase_; ++i)
    {
        std::vector< std::vector<double> > tmp_vec;

        for (int j = 0; j < npairs_; ++j)
        {
            in.read((char*)&size, sizeof(int));

            std::vector<double> tmp_partial(size);
            in.read((char*)(&tmp_partial[0]), size*sizeof(double));
            tmp_vec.push_back(tmp_partial);
        }

        partials_.push_back(tmp_vec);
    }

    // Read 'internals' ----------------------------

    for (int i = 0; i < nbase_; ++i)
    {
        std::vector< std::vector<double> > tmp_vec;

        for (int j = 0; j < npairs_; ++j)
        {
            in.read((char*)&size, sizeof(int));

            std::vector<double> tmp_internal(size);
            in.read((char*)(&tmp_internal[0]), size*sizeof(double));
            tmp_vec.push_back(tmp_internal);
        }

        internals_.push_back(tmp_vec);
    }

    // Read 'scalars' ------------------------------

    size = nscalars_;

    for (int i = 0; i < nbase_; ++i)
    {
        double* memblock_d = new double[size];
        in.read((char*)memblock_d, size*sizeof(double));

        std::vector<double> tmp_vec;
        for (int j = 0; j < size; ++j)
            tmp_vec.push_back(memblock_d[j]);

        scalars_.push_back(tmp_vec);

        delete [] memblock_d;
    }

    if (trace)
    {
        for (int i = 0; i < (int)scalars_.size(); ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                printf("%f ", scalars_[i][j]);
            }
            printf("\n");
        }
    }

    // Read 'geo' -------------------------------------

    for (int i = 0; i < nbase_; ++i)
    {
        int rows = natoms_[i][0];
        int cols = 3;

        size = rows * cols;
        std::vector<double> data(size);
        in.read((char*)(&data[0]), size*sizeof(double));

        Matrix tmp_geo(rows, cols);

        for (int j = 0; j < rows; ++j)
        {
            for (int k = 0; k < cols; ++k)
            {
                tmp_geo(j,k) = data[j*cols+k];
            }
        }

        geo_.push_back(tmp_geo);

    }

    // Read 'central_mol' --------------------------------

    for (int i = 0; i < nbase_; ++i)
    {
        std::vector<int> tmp_vec;

        int size;
        in.read((char*)&size, sizeof(int));

        memblock_i = new int[size];

        in.read((char*)memblock_i, size*sizeof(int));

        for (int j = 0; j < size; ++j)
            tmp_vec.push_back(memblock_i[j]);

        delete [] memblock_i;

        central_mol_.push_back(tmp_vec);
    }

    if (trace)
    {
        for (int i = 0; i < (int)central_mol_.size(); ++i)
        {
            printf("central_mol %i: ", i);
            for (int j = 0; j < (int)central_mol_[i].size(); ++j)
            {
                printf(" %i ", central_mol_[i][j]);
            }
            printf("\n");
        }
    }

    in.close();

    nadded_ = nbase_;

    printf("Successfully read information from file: %s\n", filename_lib.c_str());
}


// -------------------------------------------------------------------------- //
// Access functions
// -------------------------------------------------------------------------- //
//
const std::vector<double> & Library::get_partial(const int index, const int type1, const int type2) const
{
    const int pos = index_matrix_(type1,type2);
    return partials_[index][pos];
}


// -------------------------------------------------------------------------- //
//
const std::vector<double> & Library::get_internal(const int index,
                                                  const int type1,
                                                  const int type2) const
{
    const int pos = index_matrix_(type1,type2);
    return internals_[index][pos];
}


// -------------------------------------------------------------------------- //
// Other utility functions.
// -------------------------------------------------------------------------- //
//

// -------------------------------------------------------------------------- //
//
void Library::check_setup(const std::string & function, const std::string & location) const
{
    if (setup_done_)
    {
        std::string msg("Function '");
        msg += function + "(...)' can not be called after setup is completed.\n";
        error(msg, location);
    }
}
