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


/*! \file  pcfdata.h
 *  \brief File for the PCFData class definition.
 */


#ifndef __PCFDATA__
#define __PCFDATA__

#include <vector>
#include <string>

#include "basiscontainer.h"

// Forward declarations.
class Library;

/// Class representing a PCF data set.
class PCFData {

public:

    /*! \brief Default constructor for the PCFData class needed for
     *         initializing from the Specswap class.
     */
    PCFData() {}

    /*! \brief Constructor for the PCFData class.
     *  \param rmin          : The minimum r value.
     *  \param rmax          : The maximum r value.
     *  \param dr            : The bin size.
     *  \param sigma         : The sigma value for calculating chi2.
     *  \param numberdensity : The numberdensity to use for the normalization.
     *  \param fit_interval  : The interval to perfrorm the fit within.
     *  \param nbins         : The number of bins in the reference.
     *  \param partial       : Indicating which partial in the library to fit.
     *  \param path          : The full path to the file holding the reference.
     */
    PCFData(const double rmin,
            const double rmax,
            const double dr,
            const double sigma,
            const double numberdensity,
            const std::pair<double,double> & fit_interval,
            const int nbins,
            const std::pair<int,int> & partial,
            const std::string & path);

    /*! \brief Function for setting up the pcf and calculating initial chi2.
     *  \param sampleset : Vector holding the sample set indices.
     *  \param library   : The library to take the data from.
     */
    void init(const std::vector<int> & sampleset,
                const Library & library);

    /*! \brief Function to notify the pcfdata object of an attempted move.
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

    /*! \brief Function for printing the histogram, pcf, reference and normalization to file.
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


    /*! \brief Helper routine for calculating the r value center of a bin index.
     *  \param index : The index to get the r value for.
     *  \return : The center r value for a given bin index.
     */
    const double get_r(const int index) const;


    /*! \brief Helper routine for calculating the bin for a given distance.
     *  \param  distance : The distance to calculate the bin for.
     *  \return : The bin.
     */
    const int calculate_bin(const double distance) const;


    /*! \brief Helper routine for calculating the partial histogram for a basis element.
     *  \param  library : The library to use.
     *  \return : The calculated partial histogram.
     */
    const std::vector<double> calculate_partial_histogram(const Library & library,
                                                          const int index) const;

    /*! \brief Helper routine for calculating the chi2 based on a histogram.
     *  \param  histogram : The histogram to calculate chi2 for.
     *  \return : The calculated chi2.
     */
    const double calculate_chi2(const std::vector<double> & histogram) const;

    /// The number of elements in the sample set. Needed for normalization.
    int nsample_;

    /// The minimum r value.
    double rmin_;

    /// The maximum r value.
    double rmax_;

    /// The r spacing (binwidth).
    double dr_;

    /// One over the r spacing (binwidth), for convenience.
    double one_over_dr_;

    /// The sigma value to use for calculating chi2.
    double sigma_;

    /// One over sigma squared, for convenience.
    double one_over_sigma2_;

    /// The numberdensity for normalizing histograms to pcfs.
    double numberdensity_;

    /// The partial to fit againts.
    std::pair<int,int> partial_;

    /// The chi2 value.
    double chi2_;

    /// The chi2 value for the attempted move.
    double chi2_new_;

    /// Holding the reference pcf from the data file.
    std::vector<double> pcf_reference_;

    /// Holding the factors to multiply each bin in the histograms with to get a pcf.
    std::vector<double> pcf_normalization_factor_;

    /// Holding the current distance histogram.
    std::vector<double> histogram_;

    /// Holding the histogram for the attempted move.
    std::vector<double> histogram_new_;

    /// The interval (in bins) to use for calculating chi2.
    std::pair<int,int> fit_interval_;

    /// Declaring the test class as friend, to facilitate testing.
    friend class Test_PCFData;

};

#endif
