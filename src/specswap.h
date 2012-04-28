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


/*! \file  specswap.h
 *  \brief File for the Specswap class definition.
 */

#ifndef __SPECSWAP__
#define __SPECSWAP__

#include <vector>
#include <string>

// Local includes.
#include "library.h"
#include "sampleset.h"
#include "mlrnd.h"
#include "pcfdata.h"
#include "curvedata.h"
#include "meanscalardata.h"
#include "valuescalardata.h"
#include "scalardistributiondata.h"
#include "basiscontainer.h"

// std io forward declarations.
#include <iosfwd>


/// The central Specswap workhorse object performing moves and controling communication with attached data sets.
class Specswap {

  public:

    /*! \brief Default constructor needed for setup from the rmc main program.
     */
    Specswap() {}

    /*! \brief Normal constructor of the Specswap class.
     *  \param nsample      : The number of basis elements to take in the sample set.
     *  \param rand         : The random number generator to use.
     *  \param library_path : The path to the library file.
     */
    Specswap(int nsample,
             const Mlrnd & rand,
             const std::string & library_path);

    /*! \brief Restart constructor of the Specswap class.
     *  \param nsample         : The number of basis elements to take in the sample set.
     *  \param rand            : The random number generator to use.
     *  \param library_path    : The path to the library file.
     *  \param restart_inpath  : The path to the restart file.
     */
    Specswap(int nsample,
             const Mlrnd & rand,
             const std::string & library_path,
             const std::string & restart_path);

    // -------------------------------------------------------------------------
    // Functions for setup.
    // -------------------------------------------------------------------------

    /*! \brief To add a mean scalar to fit.
     *  \param scalarname : The name of the scalar. Must match a scalar name in
     *                      the added library.
     *  \param target     : The target mean value to fit against.
     *  \param sigma      : The sigma value to use for the fit.
     */
    void add_scalar_mean(const std::string& scalarname,
                         const double target,
                         const double sigma);

    /*! \brief To add a scalar value to fit.
     *  \param scalarname : The name of the scalar. Must match a scalar name in
     *                      the added library.
     *  \param value_low  : The low limit of the fit interval.
     *  \param value_high : The high limit of the fit interval.
     *  \param fraction   : The target fraction of elements with the specified
     *                      scalar within the interval.
     *  \param sigma      : The sigma value to use for the fit.
     */
    void add_scalar_value(const std::string& scalarname,
                          const double value_low,
                          const double value_high,
                          const double fraction,
                          const double sigma);

    /*! \brief To add a scalar distribution to fit.
     *  \param scalarname : The name of the scalar. Must match a scalar name in
     *                      the added library.
     *  \param filename   : The name of the file containing the reference data.
     *  \param sigma      : The sigma value to use for the fit.
     */
    void add_scalar_distribution(const std::string& scalarname,
                                 const std::string& filename,
                                 const double sigma);

    /*! \brief Function for adding a curve data set.
     *  \param sigma       : The sigma value to use.
     *  \param area_renorm : Flag indicating if area renormalization should be used.
     *  \parma curve_name  : The name (ending) of the curve.
     *  \param path        : Full path to the file holding the reference curve information.
     */
    void add_curve(const double sigma,
                   const bool area_renorm,
                   const std::string & curve_name,
                   const std::string & path);

    /*! \brief Function for adding a PCF data set.
     *  \param rmin          : The low r cutoff.
     *  \param rmax          : The high r cutoff.
     *  \param dr            : The r spacing.
     *  \param sigma         : The sigma value to use for the dataset.
     *  \param numberdensity : The numberdensity to use for the normalization.
     *  \param fit_interval  : The interval to use when calculating chi2.
     *  \parma nbins         : The number of bins in the data.
     *  \param partial       : The partial in question, refering to the numbering in the library.
     *  \param path          : Full path to the file holding the reference PCF information.
     */
    void add_pcf(const double rmin,
                 const double rmax,
                 const double dr,
                 const double sigma,
                 const double numberdensity,
                 const std::pair<double,double> & fit_interval,
                 const int nbins,
                 const std::pair<int,int> & partial,
                 const std::string & path);


    /*! \brief The main setup function called when all input is read and the
     *          specswap object is to be prepared for running the simulation.
     */
    void setup();

    // -------------------------------------------------------------------------
    // Functions called from within the MC loop.
    // -------------------------------------------------------------------------

    /*! \brief The move function called from the main RMC loop when a move
     *         should be made.
     *  \note  : Called from within the MC loop.
     */
    void move();

    /*! \brief Query for the chi2 value.
     *  \note  : Called from within the MC loop.
     */
    const double get_chi2() const { return chi2_; }

    /*! \brief Query for the new chi2 value.
     *  \note  : Called from within the MC loop.
     */
    const double get_chi2_new() const { return chi2_new_; }

    /*! \brief The notify function called from the main RMC loop when a move
     *         has been made and it is time to calculate the delta chi2.
     *  \note  : Called from within the MC loop.
     */
    void notify();

    /*! \brief The accept function called from the main RMC loop when a move
     *         should be accepted and the new configuration stored as old.
     *  \note  : Called from within the MC loop.
     */
    void accept();

    /*! \brief Function for collecting weights by probing the sampleset.
     *  \note  : Called from within the MC loop.
     */
    void collect_weights();


    // -------------------------------------------------------------------------
    //  Functions for IO.
    // -------------------------------------------------------------------------

    /*! \brief The print function called form the main RMC loop to printout
     *         run info to standard out.
     *  \note  : Called from within the MC loop.
     */
    void print() const;

    /*! \brief Printing startup information.
     */
    void print_start() const;

    /*! \brief Printing stop information.
     */
    void print_stop() const;

    /*! \brief Print the weights to standard out.
     *  \param output : The ofstream to write to.
     *  \param weight : The weight to divide the incremented probe numbers with
     *                  to produce the weights to print.
     */
    void print_weights(std::ofstream& output, const int weight) const;

    /*! \brief Write restart info to the specified path.
     */
    void write_restart(const std::string & path) const;

    /*! \brief Write curve and pcf information for the present sampleset to file.
     *  \param title : The title of the run to use as part of the filename.
     */
    void write_dump(const std::string& title);

    // -------------------------------------------------------------------------
    // Functions for performing analysis.
    // -------------------------------------------------------------------------

    /*! \brief Write a file with all basis names and their weights listed,
     *         sorted after highest weight.
     *  \param title : The title of the run to use as part of the filename.
     */
    void write_weights_and_names_list(const std::string& title);

    /*! \brief Write all data sets to file, weighted and unweighted.
     *  \param title : The title of the run to use as part of the filename.
     */
    void weighted_analysis(const std::string& title);

    /*! \brief Write all data sets to file unweughted, summed in chunks of decreasing
     *         weights.
     *  \param title      : The title of the run to use as part of the filename.
     *  \param chunks     : The number of chunks to create.
     *  \param chunk_size : The size of the chunks.
     */
    void chunk_analysis(const std::string& title, const int chunks, const int chunk_size);

  private:

    // -------------------------------------------------------------------------
    // Functions for setup.
    // -------------------------------------------------------------------------

    /*! \brief For setting up the sampleset.
     */
    void setup_sampleset();

    /*! \brief For setting up the chi2 values.
     */
    void setup_chi2();


    // -------------------------------------------------------------------------
    // Functions for IO.
    // -------------------------------------------------------------------------

    /*! \brief Function for printing run info to a file.
     *  \param basename : The path of the file to write to, withouth extension.
     */
    void print_to_file(const std::string& basename) const;


    // -------------------------------------------------------------------------
    // Utility functions.
    // -------------------------------------------------------------------------

    /*! \brief Obtain the position in the library for a given scalarname.
     *  \param scalarname : The name of the scalar to check for.
     *  \return           : The position of the scalar in the library.
     */
    const int calc_scalar_pos(const std::string& scalarname) const;

    /*! \brief Generates the index of a random basis from the library.
     *  \return : A random basis index.
     */
    const int random_basis() const;

    /*! \brief A random number generator.
     *  \return : an integer between
     */
    double random() const;

    /*! \brief Calling this function sets up things needed for all analysis.
     */
    void prepare_for_analysis();

    /*! \brief Perform analysis on the chunk.
     *  \param weights_table : The chunk to analyse.
     *  \param basename      : The base name of the file to write to.
     */
    void analyse_chunk(const std::vector<BasisContainer> & weights_table,
                       const std::string& basename);

    // -------------------------------------------------------------------------
    // Member data.
    // -------------------------------------------------------------------------

    /// The random number generator.
    Mlrnd rand_;

    /// The library holding all basis element data.
    Library library_;

    /// The sampleset object.
    Sampleset sampleset_;

    /// The vector with objects holding PCF data.
    std::vector<PCFData> pcf_data_;

    /// The vector with objects holding curve data.
    std::vector<CurveData> curve_data_;

    /// The vector with mean scalar data objects.
    std::vector<MeanScalarData> mean_scalar_data_;

    /// The vector with value scalar data objects.
    std::vector<ValueScalarData> value_scalar_data_;

    /// The vector with scalar distribution data objects.
    std::vector<ScalarDistributionData> scalar_distribution_data_;

    /// The number of basis elements in the sample set.
    int nsample_;

    /// The number of curves in the library.
    int ncurves_;

    /// The total specswap chi2.
    double chi2_;

    /// The total new chi2 from specswap.
    double chi2_new_;

    /// The sampled weights multiplied with the number of times probed.
    std::vector<int> weights_;

    /// Holding names with associated indices and weights for analysis.
    std::vector<BasisContainer> weights_table_;

    /// Used to define a move, holding the index of the basis element to swap out of the sampleset.
    int from_sample_;

    /// Used to define a move, holding the index of the basis element to swap in to the sample set.
    int from_basis_;

    /// The sampleset slot to swap in.
    int slot_;

    /// The number of times weights have been collected.
    int nprobe_;

    /// Counter for indexing filenames when dumping to file.
    int dump_counter_;

    /// Flag indicating if this is a restart run or not.
    bool restart_;

    /// The path to the restart file to read from.
    std::string restart_path_;

    /// Declaring the test class as friend, to facilitate testing.
    friend class Test_Specswap;

};

#endif

