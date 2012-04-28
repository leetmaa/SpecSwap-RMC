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


/*! \file  mlrmc.h
 *  \brief File for the Mlrmc class definition.
 */

#ifndef __MLRMC__
#define __MLRMC__

#include <vector>
#include <string>
#include <fstream>

#include "specswap.h"
#include "mlrnd.h"

// Forward declarations

#include <iosfwd>


/// The central RMC driver class, handling input setup and program flow.
class Mlrmc {

public:

    /*! \brief The constructor for the RMC object.
     *  \param filename : The basename of the inputfile.
     */
    Mlrmc(std::string filename);

    // -------------------------------------------------------------------------
    // Functions called from the main routine.
    // -------------------------------------------------------------------------

    /*! /brief Function for triggering the rmc simulation to start.
     */
    void run_rmc();

    /*! \brief Performs post processing.
     */
    void post_process();

    // -------------------------------------------------------------------------


    /// the random number generator.
    Mlrnd rand_;

 private:

    // -------------------------------------------------------------------------
    // Functions for setup.
    // -------------------------------------------------------------------------

    /*! \brief Function for reading an input section keyword.
     *  \param section_key : The section keyword read.
     *  \parma infile      : The open input filestream.
     */
    void read_section(const std::string section_key,
                      std::ifstream& infile);

    /*! \brief Reading a RUN input section.
     *  \parma infile      : The open input filestream.
     */
    void read_RUN(std::ifstream& infile);

    /*! \brief Reading a PCF input section.
     *  \parma infile      : The open input filestream.
     */
    void read_PCF(std::ifstream& infile);

    /*! \brief Reading CURVE input.
     *  \parma infile      : The open input filestream.
     */
    void read_CURVE(std::ifstream& infile);

    /*! \brief Reading SCALAR input.
     *  \parma infile      : The open input filestream.
     */
    void read_SCALAR(std::ifstream& infile);

    /*! \brief Reading MEAN input.
     *  \parma infile      : The open input filestream.
     */
    void read_MEAN(std::ifstream& infile, const std::string& scalarname);

    /*! \brief Reading VALUE input.
     *  \parma infile      : The open input filestream.
     */
    void read_VALUE(std::ifstream& infile, const std::string& scalarname);

    /*! \brief Reading DISTRIBUTION input.
     *  \parma infile      : The open input filestream.
     */
    void read_DISTRIBUTION(std::ifstream& infile, const std::string& scalarname);

    /*! \brief Reading an ANALYSIS input section.
     *  \parma infile : The open input filestream.
     */
    void read_ANALYSIS(std::ifstream& infile);


    // -------------------------------------------------------------------------
    // Functcions called from within the RMC loop.
    // -------------------------------------------------------------------------

    /*! \brief The main RMC loop.
     */
    void rmc_loop();

    /*! \brief The collection of Chi2 from datasets.
     */
    void init_chi2();

    /*! \brief Make a move.
     */
    void move();

    /*! \brief Notify the datasets that a move has been made.
     */
    void notify();

    /*! \brief Collect the new Chi2 from the data sets.
     */
    void get_chi2_new();

    /*! \brief Perform the MC test.
     */
    bool montecarlo_test() const;

    /*! \brief Accept the move.
     */
    void accept();


    // -------------------------------------------------------------------------
    // Functcions for IO, some of which are called from within the RMC loop.
    // -------------------------------------------------------------------------

    /*! \brief Handle all IO during run.
     */
    void print_and_save();

    /*! \brief Handle screen and chi2 file IO.
     */
    void print();

    /*! \brief Prints some starting information to screen.
     */
    void first_print();

    /*! \brief Prints some ending information to screen.
     */
    void last_print();

    /*! \brief IO function for saving a configuration to file.
     */
    void save() const;

    /*! \brief Prints some post processing start information to screen.
     */
    void print_post_process_start() const;

    /*! \brief Prints some post processing stop information to screen.
     */
    void print_post_process_stop() const;

    // -------------------------------------------------------------------------
    // Member data for running the simulation.
    // -------------------------------------------------------------------------

    /// The main Specswap object performing specswap moves and interfacing the library.
    Specswap specswap_;

    // -------------------------------------------------------------------------
    // Input flags.
    // -------------------------------------------------------------------------

    /// Flag for indicating if the RUN input section has been read.
    bool RUN_section_;
    /// Flag for indicating if the ANALYSIS input section has been read.
    bool ANALYSIS_section_;

    // -------------------------------------------------------------------------
    // Stearing parameters.
    // -------------------------------------------------------------------------

    /// The seed to the random number generator.
    int seed_;
    /// Parameter for holding how often to print to screen.
    int print_interval_;
    /// Parameter for holding how often to save to file.
    int save_interval_;
    /// Parameter for holding the probe interval.
    int probe_interval_;
    /// Parameter for holding the dump interval.
    int dump_interval_;
    /// Holds number of chuncks in analysis.
    int analysis_chunks_;
    /// Holds the size of the chunks in analysis.
    int analysis_chunk_size_;
    /// Parameter holding the number of moves the simulation should run.
    unsigned long int moves_;

    // -------------------------------------------------------------------------
    // Counters.
    // -------------------------------------------------------------------------

    /// Counter holding the number of times the structure was probed.
    unsigned long int probe_counter_;
    /// The number of accepted moves.
    unsigned long int accepted_;
    /// The number of attempted moves.
    unsigned long int attempted_;
    /// The number of accepted since last print.
    int accepted_recent_print_;
    /// The number of accepted since last probe.
    int accepted_recent_probe_;
    /// The number of accepted moves since last dump.
    int accepted_recent_dump_;
    /// The number of attempted moves since last print.
    int attempted_recent_print_;

    // -------------------------------------------------------------------------
    // Run variables.
    // -------------------------------------------------------------------------

    /// The chi2.
    double chi2_;
    /// The new chi2.
    double chi2_new_;
    /// The difference in chi2.
    double delta_chi2_;

    // -------------------------------------------------------------------------
    // Member data for IO.
    // -------------------------------------------------------------------------

    // The title of the run.
    std::string title_;
    /// The logfile.
    std::ofstream logfile_;

};


#endif


