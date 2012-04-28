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


/*! \file  pcfdata.cpp
 *  \brief File for the PCFData class implementation.
 */


#include <fstream>
#include <cmath>

#include "ioutils.h"
#include "mathutils.h"
#include "library.h"
#include "pcfdata.h"

// -------------------------------------------------------------------------- //
//
PCFData::PCFData(const double rmin,
                 const double rmax,
                 const double dr,
                 const double sigma,
                 const double numberdensity,
                 const std::pair<double,double> & fit_interval,
                 const int nbins,
                 const std::pair<int,int> & partial,
                 const std::string & path) :
    rmin_(rmin),
    rmax_(rmax),
    dr_(dr),
    one_over_dr_(1.0/dr),
    sigma_(sigma),
    one_over_sigma2_(1.0/(sigma*sigma)),
    numberdensity_(numberdensity),
    partial_(partial),
    pcf_reference_(nbins,0.0),
    pcf_normalization_factor_(nbins,0.0),
    histogram_(nbins,0.0),
    histogram_new_(nbins,0.0)
{
    // Read in the reference pcf.
    read_reference(path);

    // Calculate the bins for the fit interval.
    fit_interval_.first  = calculate_bin(fit_interval.first);
    fit_interval_.second = calculate_bin(fit_interval.second);

    // Check that the fit interval is within bounds.
    if (fit_interval_.first < 0
        || fit_interval_.first > (nbins-1)
        || fit_interval_.second < 0
        || fit_interval_.second > (nbins-1) )
    {
        char str[200];
        sprintf(str, "Fit interval [%i,%i] not within bounds [%i,%i].", fit_interval_.first, fit_interval_.second, 0, nbins-1);
        error(str, LOCATION);
    }

    // Setup the normalization factor.
    for (size_t i = 0; i < pcf_normalization_factor_.size(); ++i)
    {
        const double r = get_r(i);
        const double factor = 3.0 / (4.0 * PI__ * r * r * numberdensity_ * dr_);
        pcf_normalization_factor_[i] = factor;
    }
}


// -------------------------------------------------------------------------- //
//
void PCFData::read_reference(const std::string & path)
{
    // Open the file.
    std::ifstream infile(path.c_str());
    check_eof(infile, path, LOCATION);

    // Read all data.
    for (size_t i = 0; i < pcf_reference_.size(); ++i)
    {
        double bin_value;
        infile >> bin_value >> pcf_reference_[i];
        check_eof(infile, path, LOCATION);

        // Make sure the bin value is correct.
        const double ref_bin_value = get_r(i);
        double diff = ref_bin_value - bin_value;
        diff = std::sqrt(diff*diff);

        if (diff > 1.0e-6)
        {
            std::string msg = "RMIN, RMAX and DR must match values in file: \"" + path + "\"";
            error(msg, LOCATION);
        }
    }

    // Close the file after reading.
    infile.close();

}


// -------------------------------------------------------------------------- //
//
const double PCFData::get_r(const int index) const
{
    return rmin_ + (dr_/2.0) + (dr_*index);
}


// -------------------------------------------------------------------------- //
//
const int PCFData::calculate_bin(const double distance) const
{
    return static_cast<int>( fastfloor((distance - rmin_) * one_over_dr_) );
}


// -------------------------------------------------------------------------- //
//
const std::vector<double> PCFData::calculate_partial_histogram(const Library & library, const int index) const
{
    // NOTE : PERFORMANCE :
    //  49 % of the time of a normal specswap run against two pcf's and one
    //  curve is spent in this routine.

    // Setup the return data.
    size_t nbins = histogram_.size();
    std::vector<double> partial_histogram(nbins, 0);

    // Get the distance distribution from the librarty.
    const std::vector<double> & distances = library.get_partial(index, partial_.first, partial_.second);

    // Skip all distance values smaller than rmin.
    size_t i = 0;
    for ( ; i < distances.size(); ++i)
    {
        if (distances[i] < rmin_)
        {
            continue;
        }
        break;
    }

    // Go through the rest of the distances.
    for ( ; i < distances.size(); ++i)
    {
        // Break if we have passed rmax.
        if (rmax_ <= distances[i])
        {
            break;
        }

        // Calculate the bin.
        const int bin = calculate_bin(distances[i]);

        // Add to the bin.
        partial_histogram[bin] += 1.0;
    }

    // Return the partial histogram.
    return partial_histogram;
}


// -------------------------------------------------------------------------- //
//
const double PCFData::calculate_chi2(const std::vector<double> & histogram) const
{
    double chi2 = 0.0;
    for (int i = fit_interval_.first; i <= fit_interval_.second; ++i)
    {
        // Normalize to a pcf in this point.
        const double pcf_value = histogram[i] * pcf_normalization_factor_[i];

        // Take the difference in each point.
        const double pcf_diff = pcf_value - pcf_reference_[i];

        // Calculate the contribution to chi2.
        chi2 += pcf_diff * pcf_diff * one_over_sigma2_;
    }

    // Divide by the number of fitted bins.
    chi2 = chi2 / (fit_interval_.second - fit_interval_.first + 1);

    // All done.
    return chi2;
}


// -------------------------------------------------------------------------- //
//
void PCFData::init(const std::vector<int> & sampleset,
                   const Library & library)
{
    // Save the number of basis elements in the sampleset.
    nsample_ = static_cast<int>(sampleset.size());

    // Loop over all indices in the sampleset.
    for (size_t i = 0; i < sampleset.size(); ++i)
    {
        // For each, get the distances and add.
        const int index = sampleset[i];
        const std::vector<double> partial_histogram = calculate_partial_histogram(library, index);
        vadd(histogram_, partial_histogram);
    }

    // Divide by the number of elements in the sampleset.
    histogram_ = histogram_ / nsample_;

    // Calculate chi2.
    chi2_ = calculate_chi2(histogram_);

    // Init the new histogram.
    histogram_new_ = histogram_;
}


// -------------------------------------------------------------------------- //
//
void PCFData::notify(const int from_sample,
                     const int from_basis,
                     const Library & library)
{
    // Multiply with the number of basis elements.
    histogram_new_ = histogram_ * nsample_;

    // Subtract the histogram to swap out.
    const std::vector<double> partial_histogram1 = calculate_partial_histogram(library, from_sample);
    vsub(histogram_new_, partial_histogram1);

    // Add the histogram to swap in.
    const std::vector<double> partial_histogram2 = calculate_partial_histogram(library, from_basis);
    vadd(histogram_new_, partial_histogram2);

    // Divide with the number of basis elements.
    histogram_new_ = histogram_new_ / nsample_;

    // Get the new chi2.
    chi2_new_ = calculate_chi2(histogram_new_);
}


// -------------------------------------------------------------------------- //
//
void PCFData::accept()
{
    // Save the new chi2 value.
    chi2_ = chi2_new_;
    // Save the new histogram.
    histogram_ = histogram_new_;
}


// -------------------------------------------------------------------------- //
// Functions for IO
// -------------------------------------------------------------------------- //


// -------------------------------------------------------------------------- //
//
void PCFData::print() const
{
    char str[128];
    sprintf(str, "PCF(%i,%i)", partial_.first, partial_.second);

    printf("      %20s%15.7e\n", str, chi2_);
}


// -------------------------------------------------------------------------- //
//
void PCFData::print_to_file(const std::string& basename) const
{
    // Open the file.
    char tmp_str[50];
    sprintf(tmp_str, "_partial_%i_%i.pcf", partial_.first, partial_.second);
    std::string filename = basename + tmp_str;
    std::ofstream outfile(filename.c_str());

    // Check that the file is ok.
    if (outfile.bad())
    {
        open_file_error(filename, LOCATION);
    }

    outfile << "R                HST               PCF             REFERENCE          NORMALIZATION" << std::endl;
    for (size_t i = 0; i < histogram_.size(); ++i)
    {
        const double bin_value = get_r(i);
        const double hst_value = histogram_[i];
        const double pcf_value = hst_value*pcf_normalization_factor_[i];

        char line[300];
        sprintf(line, "%20.10f %20.10f %20.10f %20.10f %20.10f\n", bin_value, hst_value, pcf_value, pcf_reference_[i], 1.0/pcf_normalization_factor_[i]);
        outfile << std::string(line);
    }
    outfile.close();
}


// -------------------------------------------------------------------------- //
//
void PCFData::weighted_analysis(const Library & library,
                                const std::vector<BasisContainer> & weights_table,
                                const std::string & basename) const
{
    // Setup unweighted and weighted data.
    std::vector<double> histogram(histogram_.size(), 0.0);
    std::vector<double> weighted_histogram(histogram_.size(), 0.0);

    // Loop over all indices.
    for (size_t i = 0; i < weights_table.size(); ++i)
    {
        // For each, get the distances and add.
        const int index = weights_table[i].index;
        std::vector<double> partial_histogram = calculate_partial_histogram(library, index);
        vadd(histogram, partial_histogram);
        partial_histogram = partial_histogram * weights_table[i].weight;
        vadd(weighted_histogram, partial_histogram);
    }

    // Divide by the number of elements.
    const int nbase = static_cast<int>(weights_table.size());
    histogram = histogram / nbase;
    weighted_histogram = weighted_histogram / nbase;


    // Open the file.
    char tmp_str[50];
    sprintf(tmp_str, "_partial_%i_%i.pcf", partial_.first, partial_.second);
    std::string filename = basename + tmp_str;
    std::ofstream outfile(filename.c_str());

    // Check that the file is ok.
    if (outfile.bad())
    {
        open_file_error(filename, LOCATION);
    }

    // Print.
    outfile << "        R                    HST                  PCF                  WEIGHTED             REFERENCE            NORMALIZATION" << std::endl;
    for (size_t i = 0; i < histogram_.size(); ++i)
    {
        const double bin_value = get_r(i);
        const double hst_value = histogram[i];
        const double pcf_value = hst_value*pcf_normalization_factor_[i];
        const double weighted_hst_value = weighted_histogram[i];
        const double weighted_pcf_value = weighted_hst_value*pcf_normalization_factor_[i];

        char line[300];
        sprintf(line, "%20.10f %20.10f %20.10f %20.10f %20.10f %20.10f\n", bin_value, hst_value, pcf_value, weighted_pcf_value, pcf_reference_[i], 1.0/pcf_normalization_factor_[i]);
        outfile << std::string(line);
    }
    outfile.close();
}

