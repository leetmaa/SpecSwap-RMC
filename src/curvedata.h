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


/*! \file  curvedata.h
 *  \brief File for the CurveData class definition.
 */

#ifndef __CURVEDATA__
#define __CURVEDATA__

#include <vector>
#include <string>

#include "basiscontainer.h"

// Forward declarations.
class Library;

/// Class representing a curve data set.
class CurveData {

public:

    /*! \brief Default constructor for the CurveData class needed for
     *         initializing from the Specswap class.
     */
    CurveData() {}

    /*! \brief Constructor for the CurveData class.
     *  \param  sigma      : The sigma value to use for the fit.
     *  \param  curve_name : The name (ending) of the curve.
     *  \param  path       : The path to the file with reference data.
     *  \param  library    : The library to use in the rmc run.
     */
    CurveData(const double sigma,
              const bool area_renorm,
              const std::string & curve_name,
              const std::string & path,
              const Library & library);

    /*! \brief Function for setting up the curve and calculating initial chi2.
     *  \param sampleset : Vector holding the sample set indices.
     *  \param library   : The library to take the data from.
     */
    void init(const std::vector<int> & sampleset,
                const Library & library);

    /*! \brief Function to notify the curve object of an attempted move.
     *  \param from_samle : The global index of the basis element to swap out of the sampleset.
     *  \param from_basis : The global index of the basis element to swap into the sampleset.
     *  \param library    : The library to take the data from.
     */
    void notify(const int from_sample,
                const int from_basis,
                const Library & library);

    /*! \brief Function for indicating that the move should be accepted,
     *         which means chi2 should be set to chi2 new, and the data should be updated.
     */
    void accept();

    /*! \brief Query for the chi2 value.
     *  \return : chi2 .
     */
    double get_chi2() const { return chi2_; }

    /*! \brief Query for the chi2 value.
     *  \return : chi2 .
     */
    double get_chi2_new() const { return chi2_new_; }

    /*! \brief Function for printing the curve and reference to file.
     *  \param basename : The base name to save under.
     */
    void print_to_file(const std::string & basename) const;

    /*! \brief Print chi2 info to screen.
     */
    void print() const;

    /*! \brief Print weighted and unweighted data.
     *  \param library       : The library to use.
     *  \param weights_table : The list of basis elements with weights and names.
     *  \param basename      : The base file name to write to.
     */
    void weighted_analysis(const Library & library,
                           const std::vector<BasisContainer> & weights_table,
                           const std::string & basename) const;

protected:

private:

    /*! \brief Helper routine for reading the reference data from the input file.
     *  \param  path : The full path to the file to read.
     */
    void read_reference(const std::string & path);

    /*! \brief Helper routine for calculating chi2 for a given curve.
     *  \param  curve : The curve to calculate for.
     *  \return       : The corresponding chi2
     */
    const double calculate_chi2(const std::vector<double> & curve);

    /// The number of elements in the sample set. Needed for normalization.
    int nsample_;

    /// The sigma value to use for calculating chi2.
    double sigma_;

    /// One over sigma squared, for convenience.
    double one_over_sigma2_;

    /// The chi2 value.
    double chi2_;

    /// The chi2 value for the attempted move.
    double chi2_new_;

    /// The position in the library this curve corresponds to.
    int pos_;

    /// Flag indicating if the curve should be area renormalized.
    bool area_renorm_;

    /// The curve name (in the library).
    std::string curve_name_;

    /// Holding the scale for the curve.
    std::vector<double> scale_;

    /// Holding the reference curve data from the data file.
    std::vector<double> curve_reference_;

    /// Holding the current curve.
    std::vector<double> curve_;

    /// Holding the curve for the attempted move.
    std::vector<double> curve_new_;

    /// Declaring the testclass as friend to facilitate testing.
    friend class Test_CurveData;
};



#endif


