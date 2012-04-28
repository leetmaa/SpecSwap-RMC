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


/*! \file  library.h
 *  \brief File for the Library class definition.
 */


#ifndef __LIBRARY__
#define __LIBRARY__

// ----------------------------------------------------------- //
// Naming convension for setup functions:                      //
// 'set' functions sets values and should only be called once. //
// 'add' functions makes push_back operations on member data.  //
// ----------------------------------------------------------- //

#include <string>
#include <vector>

#include "matrix.h"

/// Struct for encapsulating the convolute parameters.
struct Convolute {
    int no1;
    int no2;
    int no3;
    int no4;
};

/// Struct for encapsulating the format parameters.
struct Format {
    int start;
    int stop;
    int step;
};


/// The class defining the library to use in SpecSwap-RMC.
class Library {

public:

    // -------------------------------------------------------------------------
    // Constructors and functions for setup.
    // -------------------------------------------------------------------------

    /*!\brief Default constructor.
     */
    Library();

    /*!\brief Costructor for construction from file.
     * \param filename : The name of the binary library file to read from.
     */
    Library(const std::string & filename);

    // -------------------------------------------------------------------------
    // Functions for setup from Mklib.
    // -------------------------------------------------------------------------

    /*!\brief For setting the name of the library.
     * \param name : The name.
     */
    void set_name(const std::string& name);

    /*!\brief To set the number of curves.
     * \param ncurves : The number of curves.
     */
    void set_ncurves(const int ncurves);

    /*!\brief For adding convolute parameters.
     * \param no1 : The no1 parameter.
     * \param no2 : The no2 parameter.
     * \param no3 : The no3 parameter.
     * \param no4 : The no4 parameter.
     */
    void add_convolute(const double no1, const double no2, const double no3, const double no4);

    /*!\brief For adding format parameters.
     * \param start : The start value of the curve.
     * \param stop  : The stop value of the curve.
     * \param setp  : The step value of the curve.
     */
    void add_format(const double start, const double stop, const double step);

    /*!\brief For adding a scale.
      * \param filemame The file name to read the scale data from.
      */
    void add_scale(const std::string& filename);

    /*!\brief For adding an ending.
      * \param ending : The curve ending (name) to add.
      */
    void add_ending(const std::string& ending);

    /*!\brief For setting the number of scalars.
     * \param nscalars The number of scalars.
     */
    void set_nscalars(const int nscalars);

    /*!\brief For addding a scalar name.
     * \param name : The name of the sacalar to add.
     */
    void add_scalar_name(const std::string& name);

    /*!\brief To set the atom types of the system. Must be
     *        the same for all basis elements.
     * \param atomtypes The vector of atomtypes.
     */
    void set_atomtypes(const std::vector<std::string>& atomtypes);

    /*!\brief To set the atoms information for all basis elements.
     *        If this is called it overrides any information about
     *        atoms per type and central molecule stored in the xyz files when
     *        generating a basis using Mklib.
     * \param atoms_per_type   : The vector of atoms per type, with the first
     *                           element being the total number of atoms.
     * \param central_molecule : The vector holding the indices of the central molecule.
     */
    void set_atoms_info(const std::vector<int>& atoms_per_type,
                        const std::vector<int>& central_molecule);

    /*!\brief To set the number of base elements to fill the library with.
     * \param nbase : The number of base elements.
     */
    void set_nbase(const int nbase);

    /*!\brief Adding a base, given its filename, reading curves, scalars
     *        and geometry from the file with the given base name.
     *        and calculating the correlation functions.
     * \param basename : The base-name of the base to add.
     */
    void add_base(const std::string& basename);

    /*!\brief Function for finalizing setup.
     */
    void complete_setup();

    // -------------------------------------------------------------------------
    // -------------------------------------------------------------------------


    // -------------------------------------------------------------------------
    // Access functions
    // -------------------------------------------------------------------------

    /*!\brief Query function for the number of basis elements.
     * \return The number of added basis functions.
     */
    const int get_nbase() const {
        return nbase_;
    }

    /*!\brief Query function for the number of curves for each basis element.
     * \return The number of curves.
     */
    const int get_ncurves() const {
        return ncurves_;
    }

    /*!\brief Query function for the number of scalars for each basis element.
     * \return The number of scalars.
     */
    const int get_nscalars() const {
        return nscalars_;
    }

    /*!\brief Query function for the ending of a curve.
     * \param pos : The possition number of the curve.
     * \return The name ending pf the curve.
     */
    const std::string& get_ending(const int pos) const {
        return endings_[pos];
    }

    /*!\brief Query function for the name of a scalar.
     * \param pos : The possition number of the scalar.
     * \return The name of the scalar.
     */
    const std::string& get_scalar_name(const int pos) const {
        return scalar_names_[pos];
    }

    /*!\brief Query function for the convolute parameters of a curve.
     * \param pos : The possition number of the curve.
     * \return The name convolute parametes of the curve.
     */
    Convolute get_convolute(const int pos) const {
        Convolute c;
        c.no1 = no1_[pos];
        c.no2 = no2_[pos];
        c.no3 = no3_[pos];
        c.no4 = no4_[pos];
        return c;
    }

    /*!\brief Query function for the format parameters of a curve.
     * \param pos : The possition number of the curve.
     * \return The format parameters of the curve.
     */
    Format get_format(const int pos) const {
        Format f;
        f.start = start_[pos];
        f.stop  = stop_[pos];
        f.step  = step_[pos];
        return f;
    }

    /*!\brief Query function for the library name.
     * \return The name.
     */
    const std::string get_name() const {
        return name_;
    }

    /*!\brief Query function for the name of a basis element.
     * \param index : The index of the basis element.
     * \return The name.
     */
    const std::string& get_basis_name(const int index) const {
        return names_[index];
    }

    /*!\brief Query function for a partial distance distribution of a basis.
     * \param index : The index of the basis function.
     * \param type1 : The firt atom type.
     * \param type2 : The second atom type.
     * \return The list of distances between atoms of type1 and type2 in basis index.
     */
    const std::vector<double> & get_partial(const int index, const int type1, const int type2) const;

    /*!\brief Query function for the central molecule internal distance distribution of a basis.
     * \param index : The index of the basis function.
     * \param type1 : The firt atom type.
     * \param type2 : The second atom type.
     * \return The list of intra molecular distances between atoms of type1 and type2 in basis index.
     */
    const std::vector<double> & get_internal(const int index, const int type1, const int type2) const;

    /*!\brief Query function for a central molecule of a basis element.
     * \param index : The index of the basis element.
     * \return The list of indices for the central molecule.
     */
    const std::vector<int> & get_central(const int index) const {
        return central_mol_[index];
    }

    /*!\brief Query function for the geometry matrix of a basis.
     * \param index : The index of the basis element.
     * \return The coordinate matrix of the basis element.
     */
    const Matrix & get_geo(int index) const {
        return geo_[index];
    }

    /*!\brief Query function for the values of a basis curve.
     * \param index The index number of the basis element.
     * \param pos The curve position number.
     * \return The curve data.
     */
    const std::vector<double> & get_at(int index, int pos ) const {
        return base_[index][pos];
    }

    /*! \brief Query function for the scale of a curve.
     * \param pos : The possition number of the curve.
     * \return The scale (x-values) for the desiered curve.
     */
    std::vector<double> get_scale(const int pos) const {
        return scale_[pos];
    }

    /*!\brief Query function for the value of a scalar for a certain basis function.
     * \param index The index of the basis function.
     * \param pos The possition number of the scalar.
     * \return The value of the scalar.
     */
    const double get_scalar_at(const int index, const int pos) const {
        return scalars_[index][pos];
    }

    // -------------------------------------------------------------------------
    // Functions for binary IO
    // -------------------------------------------------------------------------

    /*!\brief Utility function to write a file to binary format.
     * \param filename The name of the file to write to.
     */
    void to_binary(const std::string & filename) const;

    // -------------------------------------------------------------------------
    // -------------------------------------------------------------------------

    /// The version string of the library.
    static const std::string version_;

private:

    // -------------------------------------------------------------------------
    // Private utility functions.
    // -------------------------------------------------------------------------

    /*!\brief Raises an error with the message saying the function_name is not
     *        callable after setup has been completed.
     * \param function : The name of the function to appear in the error message.
     * \param location : The location of the error.
     */
    void check_setup(const std::string & function, const std::string & location="") const;

    /*!\brief Read a scale from file.
     * \param filename The full name of the file to read.
     */
    void read_scale(const std::string& filename);

    /*!\brief Add a curve from file.
     * \param filename The full name of the file to read.
     * \param pos      The position index to which the part should be added.
     *                 (Needed for knowing the dimensions.)
     */
    void add_curve(const std::string& filename, const int pos);

    /*!\brief Add a set of scalars to a bais.
     * \param filename The full name of the scalrs file to read.
     */
    void add_scalars(const std::string& filename);

    /*!\brief Add the geometry of a basis element.
     * \param filename The full name of the geometry (xyz) file to read.
     */
    void add_geometry(const std::string& filename);

    /*!\brief Calculate and save the partial distance distributions
     *        for the last added geometry.
     */
    void calculate_partials();

    /*!\brief Calculate and save the internal (molecular) distance distributions
     *        for the last added geometry.
     */
    void calculate_internals();

    // -------------------------------------------------------------------------
    // Functions for binary IO
    // -------------------------------------------------------------------------
    /*!\brief Utility function to setup from a binary file.
     * \param filename The full path of the file to read from, without the .library ending.
     */
    void from_binary(const std::string & filename);

    // -------------------------------------------------------------------------
    // Data fields with leading dimension 'nbase'.
    // -------------------------------------------------------------------------
    /// The vector of basis functions.
    std::vector< std::vector < std::vector<double> > > base_;
    /// The vector of partials.
    std::vector< std::vector< std::vector<double> > > partials_;
    /// The vector of internal geometries.
    std::vector< std::vector< std::vector<double> > > internals_;
    /// The vector of geometries.
    std::vector<Matrix> geo_;
    /// The indices vector of the central molecule.
    std::vector< std::vector<int> > central_mol_;
    /// The number of atoms per basis. This is needed for normalizing the partials.
    std::vector< std::vector<int> > natoms_;
    /// The scalars data.
    std::vector< std::vector<double> > scalars_;
    /// The vector of base-names of the basis functions.
    std::vector< std::string > names_;

    // -------------------------------------------------------------------------
    // Info fields with leading dimension 'ncurves'
    // -------------------------------------------------------------------------
    /// The name ending of the curves.
    std::vector< std::string > endings_;
    /// The scales of the curves.
    std::vector< std::vector<double> > scale_;
    // FIXME: Put these in a struct instead. {no1, no2, no3, no4}
    /// The no1 parameter.
    std::vector<double> no1_;
    /// The no2 parameter.
    std::vector<double> no2_;
    /// The no3 parameter.
    std::vector<double> no3_;
    /// The no4 parameter.
    std::vector<double> no4_;
    /// The start parameter.
    std::vector<double> start_;
    /// The stop parameter.
    std::vector<double> stop_;
    /// The step parameter.
    std::vector<double> step_;
    /// The dimension parameter.
    std::vector<int> dimension_;

    // -------------------------------------------------------------------------
    // Info field with leading dimension 'nscalars'
    // -------------------------------------------------------------------------
    /// The names of the scalars.
    std::vector< std::string > scalar_names_;

    // -------------------------------------------------------------------------
    // Info fields with leading dimension 'ntypes'
    // -------------------------------------------------------------------------
    /// The names of the atom types.
    std::vector<std::string> atomtypes_;

    // -------------------------------------------------------------------------
    // Data fields.
    // -------------------------------------------------------------------------
    /// counter to hold the current possition when adding scales.
    int nadded_scale_;
    /// Holds the number of added basis.
    int nadded_;
    /// The total number of bases.
    int nbase_;

    // -------------------------------------------------------------------------
    // Info fields
    // -------------------------------------------------------------------------
    /// The number of atom types.
    int ntypes_;
    /// The number of pairs.
    int npairs_;
    /// The number of curves.
    int ncurves_;
    /// The number of scalars.
    int nscalars_;

    // -------------------------------------------------------------------------
    // Various flags and constants.
    // -------------------------------------------------------------------------
    /// Flag for indicating that we have read general atoms per type and central molecule info.
    bool use_atoms_info_;
    /// Flag indicating if the setup is finnished.
    bool setup_done_;
    /// Storage of general atoms per type info to use for all basis elements.
    std::vector<int> atoms_per_type_;
    /// Storage of general central molecule info to use for all basis elements.
    std::vector<int> central_molecule_;
    /// The name of the library.
    std::string name_;
    /// Data structure for easy conversion between indices an partials.
    Matrix index_matrix_;

    /// Declare the test class as friend to facilitate testing.
    friend class Test_Library;
};

#endif

