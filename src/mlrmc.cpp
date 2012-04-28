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


/*! \file  mlrmc.cpp
 *  \brief File for the Mlrmc class implementation.
 */


#include <fstream>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "ioutils.h"
#include "mlrnd.h"
#include "mlrmc.h"


// -------------------------------------------------------------------------- //
//
Mlrmc::Mlrmc(std::string filename) :
    RUN_section_(false),
    ANALYSIS_section_(false),
    seed_(13),
    print_interval_(0),
    probe_interval_(0),
    moves_(0),
    accepted_(0),
    attempted_(0),
    accepted_recent_print_(0),
    accepted_recent_probe_(0),
    accepted_recent_dump_(0),
    attempted_recent_print_(0),
    chi2_(0.0)
{
    // Open the input file for reading.
    std::string tmp_filename = filename + ".inp";
    std::ifstream infile;
    infile.open(tmp_filename.c_str());

    // Check if the file was correctly opened.
    if (!infile)
    {
        open_file_error(tmp_filename, LOCATION);
    }

    printf("Reading input from file: %s\n", tmp_filename.c_str());

    // Read the first keyword.
    char tmp[30];
    if (!infile.eof())
    {
        infile >> tmp;
        check_eof(infile, "input file", LOCATION);
    }
    else
    {
        error("The input file is empty.\n");
    }

    // Keep reading, looking for the first section keyword.
    while (!infile.eof())
    {

        // If a section is found.
        if (tmp[0] == '&')
        {
            // Get the rest of the keyword.
            char section_key[30];
            for (int i = 0; i < 29; ++i)
            {
                section_key[i] = tmp[i+1];
            }
            // And read the section.
            std::string key(section_key);
            read_section(section_key, infile);
        }
        // Ignore comments.
        else if (tmp[0] == '#')
        {
            eatline(infile);
        }
        // Raise an error on every thing else but comments or keywords.
        else
        {
            error("Encountered non-comment/non-kewyword when expected new section.");
        }

        // Read the next word.
        infile >> tmp;
        // NOTE:
        // There should be no check_eof statement here, since it is checked above.
    }

    // All input read.
    infile.close();

    // Screen IO.
    printf("Making post-reading input processng...\n");

    // Setup the Specswap object.
    specswap_.setup();

    // Open the log file for writing.
    tmp_filename = filename + ".log";
    logfile_.open(tmp_filename.c_str());

    // Check if the file was correctly opened.
    if (!logfile_)
    {
        open_file_error(tmp_filename);
    }

    // Screen IO.
    printf("Everything is now setup and ready for running.\n");
}


// -------------------------------------------------------------------------- //
//
void Mlrmc::read_section(const std::string section_key, std::ifstream& infile)
{
    // These are the possible input sections.
    std::string RUN_key("RUN");
    std::string PCF_key("PCF");
    std::string GRDATA_key("GRDATA");
    std::string CURVE_key("CURVE");
    std::string SCALAR_key("SCALAR");
    std::string ANALYSIS_key("ANALYSIS");

    if (section_key == RUN_key)
    {
        read_RUN(infile);
    }

    else if (section_key == PCF_key)
    {
        read_PCF(infile);
    }

    else if (section_key == CURVE_key)
    {
        read_CURVE(infile);
    }

    else if (section_key == SCALAR_key)
    {
        read_SCALAR(infile);
    }

    else if (section_key == ANALYSIS_key)
    {
        read_ANALYSIS(infile);
    }

    else
    {
        unknown_section_error(section_key, "Reading input.");
    }

}


// -------------------------------------------------------------------------- //
//
void Mlrmc::read_RUN(std::ifstream& infile)
{
    if (RUN_section_)
    {
        same_section_error("&RUN", "Reading input.");
    }

    // Set the flag to true.
    RUN_section_ = true;

    // Set the location string.
    const std::string location("Reading &RUN input section.");

    // Keywords.
    const std::string TITLE_key("TITLE");
    const std::string LIBPATH_key("LIBPATH");
    const std::string SEED_key("SEED");
    const std::string NSAMPLE_key("NSAMPLE");
    const std::string MOVES_key("MOVES");
    const std::string PRINT_key("PRINT");
    const std::string PROBE_key("PROBE");
    const std::string DUMP_key("DUMP");
    const std::string RESTART_key("RESTART");

    // The END key.
    const std::string END_key("&END");

    // Temporary data.
    int nsample;
    std::string library_path;
    std::string restart_path;

    // Flags for knowing what we have read.
    bool found_title   = false;
    bool found_libpath = false;
    bool found_seed    = false;
    bool found_nsample = false;
    bool found_moves   = false;
    bool found_print   = false;
    bool found_probe   = false;
    bool found_dump    = false;
    bool found_restart = false;

    // Read until the END key is found.
    bool found = false;
    bool keep_reading = true;
    while(keep_reading)
    {
        found = false;

        // Read a keyword.
        std::string word;
        infile >> word;
        check_eof(infile, "input file", location);

        if (word == END_key)
        {
            found = true;
            keep_reading = false;

            if (!found_title)
            {
                missing_keyword_error(TITLE_key);
            }
            if (!found_libpath)
            {
                missing_keyword_error(LIBPATH_key);
            }
            if (!found_seed)
            {
                missing_keyword_error(SEED_key);
            }
            if (!found_nsample)
            {
                missing_keyword_error(NSAMPLE_key);
            }
            if (!found_moves)
            {
                missing_keyword_error(MOVES_key);
            }
            if (!found_print)
            {
                missing_keyword_error(PRINT_key);
            }
            if (!found_probe)
            {
                missing_keyword_error(PROBE_key);
            }
            if (!found_dump)
            {
                // If the optional keyword DUMP is not set,
                // use the moves value for this.
                dump_interval_ = moves_;
            }

            found = true;
            keep_reading = false;
        }

        if (word == TITLE_key)
        {
            // Check.
            if (found_title)
            {
                same_keyword_error(TITLE_key);
            }

            // Read the title, accepting any thing in one word.
            infile >> title_;
            check_eof(infile, "input file", location);

            // Set flags.
            found_title = true;
            found = true;
        }

        if (word == LIBPATH_key)
        {
            // Check.
            if (found_libpath)
            {
                same_keyword_error(LIBPATH_key);
            }

            // Read the library path.
            infile >> library_path;
            check_eof(infile, "input file", location);

            // Check if the library exists and can be opened.
            std::string tmp_path = library_path + ".library";
            check_path(tmp_path, location);

            // Set flags.
            found_libpath = true;
            found = true;
        }

        if (word == SEED_key)
        {
            // Check.
            if (found_seed)
            {
                same_keyword_error(SEED_key, location);
            }
            // Read.
            infile >> seed_;
            check_eof(infile, "input file", location);

            // Check that the seed is positive.
            if (seed_ < 0)
            {
                error("The SEED value must be a positive integer.\n", location);
            }

            // Init the random number generator.
            rand_.set_seed(seed_);

            // Set the flags.
            found_seed = true;
            found = true;
        }

        if (word == NSAMPLE_key)
        {
            // Check.
            if (found_nsample)
            {
                same_keyword_error(NSAMPLE_key, location);
            }
            // Read.
            infile >> nsample;
            check_eof(infile, "input file", location);

            // Check that the nsample is positive.
            if (nsample < 0)
            {
                error("The NSAMPLE value must be a positive integer.", location);
            }

            // Set the flags.
            found_nsample = true;
            found = true;
        }

        if (word == MOVES_key)
        {
            // Check.
            if (found_moves)
            {
                same_keyword_error(MOVES_key, location);
            }
            // Read.
            infile >> moves_;
            check_eof(infile, "input file", location);

            // Check that the moves_ is positive.
            if (moves_ < 0)
            {
                error("The MOVES value must be a positive integer.\n", location);
            }

            // Set the flags.
            found_moves = true;
            found = true;
        }

        if (word == PRINT_key)
        {
            // Check.
            if (found_print)
            {
                same_keyword_error(PRINT_key, location);
            }

            // Read.
            infile >> print_interval_;
            check_eof(infile, "input file", location);

            // Check that the print_interval_ is positive.
            if (print_interval_ < 1)
            {
                error("The PRINT value must be a positive integer.\n", location);
            }

            // Set the flags.
            found_print = true;
            found = true;
        }

        if (word == PROBE_key)
        {
            // Check.
            if (found_probe)
            {
                same_keyword_error(PROBE_key, location);
            }

            // Read.
            infile >> probe_interval_;
            check_eof(infile, "input file", location);

            // Check that the probe_interval_ is positive.
            if (probe_interval_ < 1)
            {
                error("The PROBE value must be a positive integer.\n", location);
            }

            // Set the flags.
            found_probe = true;
            found = true;
        }

        if (word == DUMP_key)
        {
            // Check.
            if (found_dump)
            {
                same_keyword_error(DUMP_key, location);
            }

            // Read.
            infile >> dump_interval_;
            check_eof(infile, "input file", location);

            // Check that the probe_interval_ is positive.
            if (dump_interval_ < 1)
            {
                error("The DUMP value must be a positive integer.\n", location);
            }

            // Set the flags.
            found_dump = true;
            found = true;
        }

        if (word == RESTART_key)
        {
            // Check.
            if (found_restart)
            {
                same_keyword_error(RESTART_key, location);
            }

            // Read.
            infile >> restart_path;
            check_eof(infile, "input file", location);

            // Check if the restart file exists and can be opened.
            check_path(restart_path);

            // Set the flags.
            found_restart = true;
            found = true;
        }

        if (!found)
        {
            unknown_keyword_error(word, location);
        }
    }

    // Every thing read and checked. Time to init the Specswap object.
    if (found_restart)
    {
        specswap_ = Specswap(nsample, rand_, library_path, restart_path);
    }
    else
    {
        specswap_ = Specswap(nsample, rand_, library_path);
    }
}


// -------------------------------------------------------------------------- //
//
void Mlrmc::read_PCF(std::ifstream& infile)
{

    // Register the location.
    const std::string location("Reading PCF input section.");

    // Define possible keyword input.
    const std::string RMIN_key("RMIN");
    const std::string RMAX_key("RMAX");
    const std::string DR_key("DR");
    const std::string FIT_key("FIT");
    const std::string SIGMA_key("SIGMA");
    const std::string NUMBERDENSITY_key("NUMBERDENSITY");
    const std::string PARTIAL_key("PARTIAL");
    const std::string PATH_key("PATH");

    // The end key.
    const std::string END_key("&END");

    // Temporary data storage.
    double rmin;
    double rmax;
    double dr;
    double sigma;
    double numberdensity;
    double fit1, fit2;
    int type1, type2;
    std::string path;

    // Flags for knowing what we have read.
    bool found_rmin          = false;
    bool found_rmax          = false;
    bool found_dr            = false;
    bool found_sigma         = false;
    bool found_numberdensity = false;
    bool found_fit           = false;
    bool found_partial       = false;
    bool found_path          = false;

    // Read the input.
    bool keep_reading = true;
    bool found;
    while(keep_reading)
    {
        // Read a word.
        found = false;
        std::string word;
        infile >> word;
        check_eof(infile, "input file", location);

        if (word == RMIN_key)
        {
            // Check.
            if (found_rmin)
            {
                same_keyword_error(RMIN_key, location);
            }
            infile >> rmin;
            check_eof(infile, "input file", location);
            found = true;
            found_rmin = true;
        }

        if (word == RMAX_key)
        {
            // Check.
            if (found_rmax)
            {
                same_keyword_error(RMAX_key, location);
            }
            infile >> rmax;
            check_eof(infile, "input file", location);
            found = true;
            found_rmax = true;
        }

        if (word == DR_key)
        {
            // Check.
            if (found_dr)
            {
                same_keyword_error(DR_key, location);
            }
            infile >> dr;
            check_eof(infile, "input file", location);
            found = true;
            found_dr = true;
        }

        if (word == SIGMA_key)
        {
            // Check.
            if (found_sigma)
            {
                same_keyword_error(SIGMA_key, location);
            }
            infile >> sigma;
            check_eof(infile, "input file", location);

            check_sigma(sigma);

            found = true;
            found_sigma = true;
        }

        if (word == NUMBERDENSITY_key)
        {
            // Check.
            if (found_numberdensity)
            {
                same_keyword_error(NUMBERDENSITY_key, location);
            }
            infile >> numberdensity;
            check_eof(infile, "input file", location);

            if (numberdensity <= 0.0)
            {
                error("The NUMBERDENSITY value must be larger than zero.\n", location);
            }

            found = true;
            found_numberdensity = true;
        }

        if (word == FIT_key)
        {
            // Check.
            if (found_fit)
            {
                same_keyword_error(FIT_key, location);
            }
            infile >> fit1;
            check_eof(infile, "input file", location);
            infile >> fit2;
            check_eof(infile, "input file", location);

            found = true;
            found_fit = true;
        }

        if (word == PARTIAL_key)
        {
            // Check.
            if (found_partial)
            {
                same_keyword_error(PARTIAL_key, location);
            }
            infile >> type1;
            check_eof(infile, "input file", location);
            infile >> type2;
            check_eof(infile, "input file", location);

            if (type1 < 0 || type2 < 0)
            {
                error("The PARTIAL values must be two positive integers.\n", location);
            }

            found = true;
            found_partial = true;
        }

        if (word == PATH_key)
        {
            // Check.
            if (found_path)
            {
                same_keyword_error(PATH_key, location);
            }

            // Read the path.
            infile >> path;
            check_eof(infile, "input file", location);
            check_path(path);

            found = true;
            found_path = true;
        }

        if (word == END_key)
        {
            if (!found_rmin)
            {
                missing_keyword_error(RMIN_key, location);
            }
            if (!found_rmax)
            {
                missing_keyword_error(RMAX_key, location);
            }
            if (!found_dr)
            {
                missing_keyword_error(DR_key, location);
            }
            if (!found_sigma)
            {
                missing_keyword_error(SIGMA_key, location);
            }
            if (!found_numberdensity)
            {
                missing_keyword_error(NUMBERDENSITY_key, location);
            }
            if (!found_partial)
            {
                missing_keyword_error(PARTIAL_key, location);
            }
            if (!found_path)
            {
                missing_keyword_error(PATH_key, location);
            }
            if (!found_fit)
            {
                fit1 = rmin;
                fit2 = rmax;
            }
            else if (fit1 < rmin)
            {
                error("The lower FIT value must not be smaller than RMIN.\n", location);
            }
            else if (fit2 > rmax)
            {
                error("The upper FIT value must not be large than RMAX.\n", location);
            }

            keep_reading = false;
            found = true;
        }

        if (!found)
        {
            unknown_keyword_error(word, location);
        }

    }

    // Check that the input is reasonable.
    const double tmp_nbins = (rmax - rmin) / dr;
    const int nbins        = static_cast<int>(std::floor(tmp_nbins + 0.5e0));
    const double rest      = sqrt((tmp_nbins - nbins) * (tmp_nbins - nbins));

    if (rest > 1.0e-10)
    {
        error("The combination of rmax, rmin and dr ((rmax-rmin)/dr) must give\nan interger number of bins.", location);
    }

    // Setup the pcf handling on the specswap object.
    specswap_.add_pcf(rmin,
                      rmax,
                      dr,
                      sigma,
                      numberdensity,
                      std::pair<double,double>(fit1, fit2),
                      nbins,
                      std::pair<int,int>(type1,type2),
                      path);
}


// -------------------------------------------------------------------------- //
//
void Mlrmc::read_CURVE(std::ifstream& infile) {

    // Register the location.
    const std::string location("Reading CURVE input section.");

    const std::string EXPPATH_key("EXPPATH");
    const std::string AREARENORM_key("AREARENORM");
    const std::string SIGMA_key("SIGMA");
    const std::string END_key("&END");

    //  determine curve_pos here
    std::string curvename;
    infile >> curvename;
    check_eof(infile, "input file", location);

    // Flags to know what we have read.
    bool found_sigma = false;
    bool found_exp = false;
    bool found_arearenorm = false;

    // Temporary data.
    double sigma;
    std::string path;
    bool area_renorm = false;

    bool found = false;
    bool keep_reading = true;

    while(keep_reading)
    {

        // Read the next word.
        found = false;
        std::string word;
        infile >> word;
        check_eof(infile, "input file", location);

        if (word == END_key)
        {
            if (!found_sigma)
            {
                missing_keyword_error(SIGMA_key, location);
            }
            if (!found_exp)
            {
                missing_keyword_error(EXPPATH_key, location);
            }

            found = true;
            keep_reading = false;
        }

        if (word == SIGMA_key)
        {
            if (found_sigma)
            {
                same_keyword_error(SIGMA_key, location);
            }

            // Read the value.
            infile >> sigma;
            check_eof(infile, "input file", location);
            check_sigma(sigma);

            // Set the flags.
            found = true;
            found_sigma = true;
        }

        if (word == EXPPATH_key)
        {

            if (found_exp)
            {
                same_keyword_error(EXPPATH_key, location);
            }

            // Read the path.
            infile >> path;
            check_eof(infile, "input file", location);
            check_path(path);

            // Set the flags.
            found = true;
            found_exp = true;
        }

        if (word == AREARENORM_key)
        {
            if (found_arearenorm)
            {
                same_keyword_error(AREARENORM_key, location);
            }
            // Set the value.
            area_renorm = true;
            // Set the flags.
            found = true;
            found_arearenorm = true;
        }

        if (!found)
        {
            unknown_keyword_error(word, location);
        }

    }

    // Add the curve.
    specswap_.add_curve(sigma,
                        area_renorm,
                        curvename,
                        path);
}


// -------------------------------------------------------------------------- //
//
void Mlrmc::read_SCALAR(std::ifstream& infile)
{

    // Register the location.
    const std::string location("Reading SCALAR input section.");

    std::string MEAN_key("MEAN");
    std::string VALUE_key("VALUE");
    std::string DISTRIBUTION_key("DISTRIBUTION");
    std::string END_key("&END");

    std::string scalarname;
    infile >> scalarname;
    check_eof(infile, "input file", location);

    std::string word;
    infile >> word;
    check_eof(infile, "input file", location);

    if (word == MEAN_key)
    {
        read_MEAN(infile, scalarname);
    }
    else if (word == VALUE_key)
    {
        read_VALUE(infile, scalarname);
    }
    else if (word == DISTRIBUTION_key)
    {
        read_DISTRIBUTION(infile, scalarname);
    }
    else
    {
        error("The SCALAR section must start with one of the keywords:\nMEAN, VALUE or DISTRIBUTION\n", location);
    }

    // Read the next word. This must be the end key.
    infile >> word;
    check_eof(infile, "input file", location);

    if (word != END_key)
    {
        unknown_keyword_error(word, location);
    }
}


// -------------------------------------------------------------------------- //
//
void Mlrmc::read_MEAN(std::ifstream& infile, const std::string& scalarname)
{

    // Register the location.
    const std::string location("Reading SCALAR MEAN input section.");

    // The only allowed keyword here.
    std::string SIGMA_key("SIGMA");

    // Read the target mean value.
    double target;
    infile >> target;
    check_eof(infile, "input file", location);

    std::string word;
    infile >> word;
    check_eof(infile, "input file", location);

    if (word != SIGMA_key)
    {
        unknown_keyword_error(word, location);
    }

    // Get the sigma value.
    double sigma;
    infile >> sigma;
    check_eof(infile, "input file", location);

    check_sigma(sigma);

    // Add to specswap.
    specswap_.add_scalar_mean(scalarname, target, sigma);

}


// -------------------------------------------------------------------------- //
//
void Mlrmc::read_VALUE(std::ifstream& infile, const std::string& scalarname) {

    // Register the location.
    const std::string location("Reading SCALAR VALUE input section.");

    // The only allowed keyword here.
    std::string SIGMA_key("SIGMA");

    // Temporary input variables.
    double value_low;
    double value_high;
    double fraction;

    // Read.
    infile >> value_low;
    check_eof(infile, "input file", location);
    infile >> value_high;
    check_eof(infile, "input file", location);
    infile >> fraction;
    check_eof(infile, "input file", location);

    // Make sure the values are reasonable.
    if (value_low >= value_high)
    {
        error("The lower bound of the specified VALUE interval must be smaller\nthen the upper bound.\n", location);
    }
    if (fraction > 1.0 || fraction < 0.0)
    {
        error("The fraction target of the specified VALUE interval must be within\the interval [0.0,1.0]\n", location);
    }

    // Read the next word.
    std::string word;
    infile >> word;
    check_eof(infile, "input file", location);

    if (word != SIGMA_key)
    {
        unknown_keyword_error(word, location);
    }

    // Get the sigma value.
    double sigma;
    infile >> sigma;
    check_eof(infile, "input file", location);

    check_sigma(sigma);

    // Add the data.
    specswap_.add_scalar_value(scalarname,
                               value_low,
                               value_high,
                               fraction,
                               sigma);
}


// -------------------------------------------------------------------------- //
//
void Mlrmc::read_DISTRIBUTION(std::ifstream& infile, const std::string& scalarname) {

    // Register the location.
    const std::string location("Reading SCALAR DISTRIBUTION input section.");

    // The only allowed keyword here.
    std::string SIGMA_key("SIGMA");

    // Get the file path.
    std::string tmp_path;
    infile >> tmp_path;
    check_eof(infile, "input file", location);
    check_path(tmp_path);

    std::string word;
    infile >> word;
    check_eof(infile, "input file", location);

    if (word != SIGMA_key)
    {
        unknown_keyword_error(word, location);
    }

    // Get the sigma value.
    double sigma;
    infile >> sigma;
    check_eof(infile, "input file", location);
    check_sigma(sigma);

    // Add.
    specswap_.add_scalar_distribution(scalarname, tmp_path, sigma);
}


// -------------------------------------------------------------------------- //
//
void Mlrmc::read_ANALYSIS(std::ifstream& infile)
{

    if (!RUN_section_)
    {
        error("The RUN section must be read before the ANALYSIS section.\n", "Reading input.");
    }

    if (ANALYSIS_section_)
    {
        same_section_error("&ANALYSIS", "Reading input.");
    }

    // Set the flag to true.
    ANALYSIS_section_ = true;

    // Register the location.
    const std::string location("Reading ANALYSIS input section.");

    std::string CHUNKS_key("CHUNKS");
    std::string END_key("&END");

    std::string word;
    infile >> word;
    check_eof(infile, "input file", location);

    if (word != CHUNKS_key)
    {
        missing_keyword_error(CHUNKS_key, location);
    }

    // Read the number of chunks.
    infile >> analysis_chunks_;
    check_eof(infile, "input file", location);

    // Read the size of the chunks.
    infile >> analysis_chunk_size_;
    check_eof(infile, "input file", location);

    // Read the next word. This must be the end key.
    infile >> word;
    check_eof(infile, "input file", location);

    if (word != END_key)
    {
        unknown_keyword_error(word, location);
    }
}


// -------------------------------------------------------------------------- //
//
void Mlrmc::init_chi2()
{
    // Get chi2 from specswap.
    chi2_ = specswap_.get_chi2();

}


// -------------------------------------------------------------------------- //
//
void Mlrmc::move()
{
    // Let specswap handle the move.
    specswap_.move();
}


// -------------------------------------------------------------------------- //
//
void Mlrmc::notify()
{
    // Notify the specswap "dataset".
    specswap_.notify();
}


// -------------------------------------------------------------------------- //
//
void Mlrmc::get_chi2_new()
{
    // Get the new chi2 from specswap.
    chi2_new_ = specswap_.get_chi2_new();
    delta_chi2_ = chi2_new_ - chi2_;
}


// -------------------------------------------------------------------------- //
//
bool Mlrmc::montecarlo_test() const
{
    // Allways accept if downwards in energy.
    if (delta_chi2_ <= 0)
    {
        return true;
    }

    // Accept with a boltzman probability if upwards in energy.
    const double tmp_ans = rand_.random();

    if (exp(-delta_chi2_) >= tmp_ans)
    {
        return true;
    }

    return false;
}


// -------------------------------------------------------------------------- //
//
void Mlrmc::accept()
{
    // Increment counters.
    ++accepted_;
    ++accepted_recent_print_;
    ++accepted_recent_probe_;
    ++accepted_recent_dump_;

    // Call the accept function on all datasets.
    specswap_.accept();

    // Save the new chi2.
    chi2_ = chi2_new_;
}


// -------------------------------------------------------------------------- //
//
void Mlrmc::print_and_save()
{
    // If it is time to probe the weights.
    if (accepted_recent_probe_ == probe_interval_)
    {
        // Reset the counter.
        accepted_recent_probe_ = 0;
        // Collect the weights.
        specswap_.collect_weights();
    }

    // If it is time to print to screen.
    if (accepted_recent_print_ == print_interval_)
    {
        // Write restart.
        specswap_.write_restart(title_+".restart");

        // Print to screen.
        print();

        // Reset the counter after printing.
        accepted_recent_print_  = 0;
        attempted_recent_print_ = 0;

    }

    // If it is time to print to file.
    if (accepted_recent_dump_ == dump_interval_)
    {
        specswap_.write_dump(title_);
        accepted_recent_dump_ = 0;
    }
}


// -------------------------------------------------------------------------- //
//
void Mlrmc::print()
{
    // Calculate the ratios.
    const double ratio = static_cast<double>(accepted_) / static_cast<double>(attempted_);
    const double ratio_last = static_cast<double>(accepted_recent_print_) / static_cast<double>(attempted_recent_print_);

    // Print logging information to file.
    char tmp_chr[300];
    sprintf(tmp_chr, "%14lu  %14lu  %14.7e  %14.7e  %14.7e\n", attempted_, accepted_, ratio_last, ratio, chi2_);
    logfile_ << std::string(tmp_chr);
    logfile_.flush();


    // Print logging information to screen.
    sprintf(tmp_chr, " Acceptance information ------------------------------------------------------\n");
    printf("%s",tmp_chr);
    sprintf(tmp_chr, "%10s%10s%15s%15s\n", "Attempted", "Accepted", "Ratio(last)", "Ratio(total)");
    printf("%s",tmp_chr);
    sprintf(tmp_chr,"%10lu%10lu%15.8f%15.6f\n\n", attempted_, accepted_, ratio_last, ratio);
    printf("%s",tmp_chr);

    // Call the print function for each dataset.
    specswap_.print();
}


// -------------------------------------------------------------------------- //
//
void Mlrmc::first_print()
{

    printf("\n===============================================================================\n");
    // Make a timestamp on the screen.
    timestamp();

    printf("Starting MLRMC run: %s\n", + title_.c_str());

    // Start the timer.
    start_timer();

    // Setup the first line in the logfile.
    char tmp_chr[300];
    sprintf(tmp_chr, "%s%s%s%s%s\n", "     Attempted", "        Accepted", "  Ratio (recent)", "   Ratio (total)", "    Chi2 (total)");
    logfile_ << std::string(tmp_chr);
    logfile_.flush();
}


// -------------------------------------------------------------------------- //
//
void Mlrmc::last_print()
{
    char tmp_chr[300];
    sprintf(tmp_chr, "SPECSWAP simulation run for %i seconds.\n", timer());
    printf("%s", tmp_chr);
    sprintf(tmp_chr, "Total number of accepted moves: %lu\n", accepted_);
    printf("%s\n", tmp_chr);
    // Close the logfile.
    logfile_.close();
    printf("Successfully ended MLRMC run: %s\n", + title_.c_str());
}


// -------------------------------------------------------------------------- //
//
void Mlrmc::print_post_process_start() const
{
    printf("\n===============================================================================\n");
    // Make a timestamp on the screen.
    timestamp();

    // Tell the world that analysis has started.
    printf("Postprocessing results from RMC run: %s\n", + title_.c_str());
}


// -------------------------------------------------------------------------- //
//
void Mlrmc::print_post_process_stop() const
{
    // Tell the world that analysis has stopped.
    printf("Done post processing.\n");
}


// -------------------------------------------------------------------------- //
//
void Mlrmc::run_rmc()
{
    // Print the timestamp and pretty start info.
    first_print();

    // Print the starting values from everything.
    if (moves_ > 0)
    {
        specswap_.print_start();
    }

    // Call all dataset to get the start value of our global chi2.
    init_chi2();

    // Run RMC.
    for (unsigned long int i = 0; i < moves_; ++i)
    {
        rmc_loop();
    }

    if (moves_ > 0)
    {
        specswap_.print_stop();
    }

    last_print();

    // END OF THE RMC RUN
}


// -------------------------------------------------------------------------- //
//
void Mlrmc::rmc_loop()
{
    // Make a move.
    move();

    // Notify the datasets.
    notify();

    // Get the new chi2.
    get_chi2_new();

    // Increment counters.
    ++attempted_;
    ++attempted_recent_print_;

    // Make the montecarlo test.
    if(montecarlo_test())
    {
        // Accept.
        accept();
    }

    // Handle IO.
    print_and_save();

}


// -------------------------------------------------------------------------- //
// Functions for post processing and analysis
// -------------------------------------------------------------------------- //
//
void Mlrmc::post_process()
{
    // Run analysis.
    if (ANALYSIS_section_)
    {
        // Pretty print to screen.
        print_post_process_start();

        // Print weights and names.
        specswap_.write_weights_and_names_list(title_);

        // Weighted and unweighted analysis.
        specswap_.weighted_analysis(title_);

        // Chunk analysis.
        specswap_.chunk_analysis(title_,
                                 analysis_chunks_,
                                 analysis_chunk_size_);

        // Pretty print to screen.
        print_post_process_stop();
    }
}

