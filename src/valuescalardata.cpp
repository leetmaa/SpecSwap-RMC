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


/*! \file  valuescalardata.cpp
 *  \brief File for the ValueScalarData class implementation.
 */

#include <cstdio>
#include <iostream>
#include <fstream>

#include "library.h"
#include "ioutils.h"
#include "valuescalardata.h"


// -------------------------------------------------------------------------- //
//
ValueScalarData::ValueScalarData(const std::string & name,
                                 const std::pair<double,double> & interval,
                                 const double target,
                                 const double sigma,
                                 const int pos) :
    name_(name),
    interval_(interval),
    target_(target),
    sigma_(sigma),
    one_over_sigma2_(1.0/(sigma*sigma)),
    pos_(pos)
{
    // NOTHING HERE
}

// -------------------------------------------------------------------------- //
//
double ValueScalarData::calculate_chi2(const double value) const
{
    const double diff = value - target_;
    return diff * diff * one_over_sigma2_;
}

// -------------------------------------------------------------------------- //
//
void ValueScalarData::init(const std::vector<int> & sampleset,
                           const Library & library)
{
    // Get the number of elements in the sample set.
    nsample_ = static_cast<int>(sampleset.size());

    // Zero the value.
    value_ = 0.0;

    int n_within = 0;

    // Loop through the sample set and extract the scalar value.
    for (size_t i = 0; i < sampleset.size(); ++i)
    {
        const int index    = sampleset[i];
        const double value = library.get_scalar_at(index, pos_);

        // Check if within the interval.
        if (value >= interval_.first && value < interval_.second)
        {
            ++n_within;
        }
    }

    // Calculate the fraction.
    value_ = static_cast<double>(n_within) / nsample_;

    // Calculate chi2.
    chi2_ = calculate_chi2(value_);

}

// -------------------------------------------------------------------------- //
//
void ValueScalarData::notify(const int from_sample,
                             const int from_basis,
                             const Library & library)
{
    // Determine the new value.
    const double subtr_val = library.get_scalar_at(from_sample, pos_);
    const double add_val   = library.get_scalar_at(from_basis, pos_);

    // Calculate the number of elements within the interval.
    double n_within = value_ * nsample_;
    if (subtr_val >= interval_.first && subtr_val < interval_.second)
        n_within -= 1.0e0;
    if (add_val >= interval_.first && add_val < interval_.second)
        n_within += 1.0e0;

    // Calculate the new fraction.
    value_new_ = n_within / nsample_;

    // Determine the new chi2.
    chi2_new_ = calculate_chi2(value_new_);

}

// -------------------------------------------------------------------------- //
//
void ValueScalarData::accept()
{
    value_ = value_new_;
    chi2_ = chi2_new_;
}

// -------------------------------------------------------------------------- //
//
void ValueScalarData::print() const
{
    char str[128];
    sprintf(str,
            "%s [%10.3e,%10.3e]",
            name_.c_str(),
            interval_.first,
            interval_.second);

    printf("%38s%15.7e %12.4e %12.4e\n",
           str,
           chi2_,
           value_,
           target_);
}


// -------------------------------------------------------------------------- //
//
void ValueScalarData::weighted_analysis(const Library & library,
                                        const std::vector<BasisContainer> & weights_table,
                                        const std::string & basename) const
{
    double n_within = 0.0;
    double n_within_weighted = 0.0;

    // Loop through the library and extract the scalar value for each basis.
    for (size_t i = 0; i < weights_table.size(); ++i)
    {
        const int index     = weights_table[i].index;
        const double weight = weights_table[i].weight;
        const double value  = library.get_scalar_at(index, pos_);

        // Check if within the interval.
        if (value >= interval_.first && value < interval_.second)
        {
            n_within += 1.0;
            n_within_weighted += 1.0 * weight;
        }
    }

    // Calculate the fraction.
    const double fraction = static_cast<double>(n_within) / weights_table.size();
    const double weighted_fraction = static_cast<double>(n_within_weighted) / weights_table.size();

    // Print to file, weighted and unweighted.
    std::string filename = basename + ".Value-" + name_;
    std::ofstream outfile(filename.c_str());

    // Check that the file is ok.
    if (outfile.bad())
    {
        open_file_error(filename, LOCATION);
    }

    // Write.
    outfile << "   INTERVAL: " << interval_.first << " " << interval_.second << std::endl;
    outfile << "     TARGET: " << target_ << std::endl;
    outfile << "   FRACTION: " << fraction << std::endl;
    outfile << "   WEIGHTED: " << weighted_fraction << std::endl;
    outfile.close();
}

