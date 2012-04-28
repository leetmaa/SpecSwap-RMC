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


/*! \file  scalardistributiondata.h
 *  \brief File for the ScalarDistributionData class definition.
 */

#ifndef __SCALAR_DISTRIBUTION_DATA__
#define __SCALAR_DISTRIBUTION_DATA__

#include <string>
#include <vector>

#include "basiscontainer.h"

// Forward declarations.
class Library;

/// Class for representing a scalar distribution data set.
class ScalarDistributionData {

public:

    /*! \brief Default constructor needed for initialization from specswap.
     */
    ScalarDistributionData() {};

    /*! \brief Constructor.
     *  \param name   : The name (ending) of the scalar.
     *  \param path   : Path to the file holding the reference distribution.
     *  \param sigma  : The sigma value.
     *  \param pos    : The index of the scalar in the library.
     */
    ScalarDistributionData(const std::string & name,
                           const std::string & path,
                           const double sigma,
                           const int pos);

    /*! \brief Function for setting up the initial distribution and initial chi2.
     *  \param sampleset : Vector holding the sample set indices.
     *  \param library   : The library to take the data from.
     */
    void init(const std::vector<int> & sampleset,
                const Library & library);

    /*! \brief Function to notify of an attempted move.
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

    /*! \brief Helper routine to read in the reference.
     *  \param path : The full path to the file holding the reference distribution.
     */
    void read_reference(const std::string & path);

    /*! \brief Helper routine to calculate chi2.
     *  \param value : The value to calculate for.
     *  \return : chi2
     */
    double calculate_chi2(const std::vector<double> & distribution) const;

    /*! \brief Helper routine to calculate the bin for a given value.
     *  \param value : The value to get the bin for.
     *  \return : The bin.
     */
    int get_bin(const double value) const;


    /// The name (ending) of the scalar.
    std::string name_;

    /// The target distribution.
    std::vector<double> target_;

    /// The scale (central bin values).
    std::vector<double> scale_;

    /// The factor for each point when calculating chi2.
    std::vector<double> factor_;

    /// The distribution.
    std::vector<double> distribution_;

    /// The distribution for the attempted move.
    std::vector<double> distribution_new_;

    /// One over the binsize.
    double one_over_binsize_;

    /// Every thing lower goes in the first bin.
    double lowest_;

    /// Everty thing higher goes in the last bin.
    double highest_;

    /// The sigma value.
    double sigma_;

    /// The one over sigma2.
    double one_over_sigma2_;

    /// The chi2 value.
    double chi2_;

    /// The chi2 value of the attempted move.
    double chi2_new_;

    /// The index of the scalar in the library.
    int pos_;

    /// The number of elements in the sample set.
    int nsample_;

    /// The number of bins in the distribution.
    int nbins_;

    /// Declaring the test class as friend to facilitate testing.
    friend class Test_ScalarDistributionData;

};

#endif
