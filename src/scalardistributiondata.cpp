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


/*! \file  scalardistributiondata.cpp
 *  \brief File for the ScalarDistributionData class implementation.
 */


#include <fstream>
#include <cstdlib>

#include "library.h"
#include "ioutils.h"
#include "mathutils.h"
#include "scalardistributiondata.h"


// -------------------------------------------------------------------------- //
//
ScalarDistributionData::ScalarDistributionData(const std::string & name,
                                               const std::string & path,
                                               const double sigma,
                                               const int pos) :
    name_(name),
    sigma_(sigma),
    one_over_sigma2_(1.0/(sigma*sigma)),
    pos_(pos)
{
    // Read in the reference distribution from file.
    read_reference(path);
}


// -------------------------------------------------------------------------- //
//
void ScalarDistributionData::read_reference(const std::string & path)
{
    // Open the file.
    std::ifstream infile(path.c_str());
    check_eof(infile, path, LOCATION);
    // Read the number of bins.
    infile >> nbins_;
    check_eof(infile, path, LOCATION);

    // Throw an error if the number of bins is too small.
    if (nbins_ <= 1)
    {
        std::string msg = "Number of bins as read from the file \"" + path + "\" is too small.\n";
        error(msg, LOCATION);
    }

    // Resize member data according to the number of bins.
    target_.resize(nbins_, 0.0);
    scale_.resize(nbins_, 0.0);
    factor_.resize(nbins_, 0.0);
    distribution_.resize(nbins_, 0.0);
    distribution_new_.resize(nbins_, 0.0);

    // Read all data.
    for (int i = 0; i < nbins_; ++i)
    {
        // Read the three columns, scale, target distrubution and local sigma.
        infile >> scale_[i];
        check_eof(infile, path, LOCATION);
        infile >> target_[i];
        check_eof(infile, path, LOCATION);
        infile >> factor_[i];
        check_eof(infile, path, LOCATION);
    }

    // Close the file after reading.
    infile.close();

    // Normalize the target to 1.
    const double norm = vsum(target_);
    target_ = target_ / norm;

    // Calculate the bin related data.
    const double halfbinsize = (scale_[1] - scale_[0]) / 2;
    lowest_  = scale_[0] - halfbinsize;
    highest_ = scale_[nbins_-1] - halfbinsize;
    one_over_binsize_ = 1.0 / (2.0 * halfbinsize);
}

// ----------------------------------------------------------------------------
//
int ScalarDistributionData::get_bin(const double value) const {

    if (value < lowest_)
    {
        return 0;
    }

    if (highest_ < value)
    {
        return nbins_ - 1;
    }

    return static_cast<int>(fastfloor(((value-lowest_)*one_over_binsize_)));
}

// -------------------------------------------------------------------------- //
//
double ScalarDistributionData::calculate_chi2(const std::vector<double> & distribution) const
{
    // Subtract the target from the scalar distribution.
    std::vector<double> tmp_distribution = distribution - target_;

    // Square the difference.
    vsquare(tmp_distribution);

    // Multiply with the elementwize factors.
    tmp_distribution = tmp_distribution * factor_;

    // Sum.
    double chi2 = vsum(tmp_distribution);

    // Divide by sigma2.
    chi2 *= one_over_sigma2_;

    // All done.
    return chi2;
}

// -------------------------------------------------------------------------- //
//
void ScalarDistributionData::init(const std::vector<int> & sampleset,
                                  const Library & library)
{
    // Get the number of elements in the sample set.
    nsample_ = static_cast<int>(sampleset.size());

    // Loop through the sample set and extract the scalar value.
    for (size_t i = 0; i < sampleset.size(); ++i)
    {
        const int index    = sampleset[i];
        const double value = library.get_scalar_at(index, pos_);

        // Calculate which bin this value belongs to.
        const int bin = get_bin(value);

        // Increment the distribution at this bin.
        distribution_[bin] += 1.0;

    }

    // Normalize the distribution to 1.
    const double norm = vsum(distribution_);
    distribution_ = distribution_ / norm;

    // Calculate chi2.
    chi2_ = calculate_chi2(distribution_);
}

// -------------------------------------------------------------------------- //
//
void ScalarDistributionData::notify(const int from_sample,
                                    const int from_basis,
                                    const Library & library)
{
    // Determine the new value.
    const double subtr_val = library.get_scalar_at(from_sample, pos_);
    const double add_val   = library.get_scalar_at(from_basis, pos_);

    // Calculate which bin these values belong to.
    const int subtr_bin = get_bin(subtr_val);
    const int add_bin   = get_bin(add_val);

    // Copy the distribution over.
    distribution_new_ = distribution_;

    // Add and subtract. (Division by nsample here to get correct normalization)
    distribution_new_[add_bin]   += (1.0 / nsample_);
    distribution_new_[subtr_bin] -= (1.0 / nsample_);

    // Determine the new chi2.
    chi2_new_ = calculate_chi2(distribution_new_);
}

// -------------------------------------------------------------------------- //
//
void ScalarDistributionData::accept()
{
    distribution_ = distribution_new_;
    chi2_ = chi2_new_;
}

// -------------------------------------------------------------------------- //
//
void ScalarDistributionData::print() const
{
    printf("                  %20s%15.7e\n",
           name_.c_str(),
           chi2_);
}


// -------------------------------------------------------------------------- //
//
void ScalarDistributionData::weighted_analysis(const Library & library,
                                               const std::vector<BasisContainer> & weights_table,
                                               const std::string & basename) const
{
    // Setup distributions.
    std::vector<double> distribution(distribution_.size(), 0.0);
    std::vector<double> weighted_distribution(distribution_.size(), 0.0);

    // Loop through the library set and extract the scalar value.
    for (size_t i = 0; i < weights_table.size(); ++i)
    {
        const int index     = weights_table[i].index;
        const double weight = weights_table[i].weight;
        const double value  = library.get_scalar_at(index, pos_);

        // Calculate which bin this value belongs to.
        const int bin = get_bin(value);

        // Increment the distribution at this bin.
        distribution[bin] += 1.0;
        weighted_distribution[bin] += 1.0 * weight;
    }

    // Normalize the distributions to 1.
    const double norm = vsum(distribution);
    distribution = distribution / norm;
    weighted_distribution = weighted_distribution / norm;

    // Print scale unweighted and weighted to file.
    std::string filename = basename + "." + "Distribution-" + name_;
    std::ofstream outfile(filename.c_str());

    // Check that the file is ok.
    if (outfile.bad())
    {
        open_file_error(filename, LOCATION);
    }

    outfile << "        SCALE                DISTRIBUTION          WEIGHTED             REFERENCE" << std::endl;

    // Loop over bins.
    for (size_t i = 0; i < distribution.size(); ++i)
    {
        // Pull out the values.
        const double scale  = scale_[i];
        const double target = target_[i];
        const double value  = distribution[i];
        const double weighted_value = weighted_distribution[i];

        char line[300];
        sprintf(line, "%20.10f %20.10f  %20.10f %20.10f\n", scale, value, weighted_value, target);
        outfile << std::string(line);
    }
    outfile.close();
}

