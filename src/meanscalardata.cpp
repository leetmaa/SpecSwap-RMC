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


/*! \file  meanscalardata.cpp
 *  \brief File for the MeanScalarData class definition.
 */

#include <cstdio>
#include <iostream>
#include <fstream>

#include "ioutils.h"
#include "library.h"
#include "meanscalardata.h"


// -------------------------------------------------------------------------- //
//
MeanScalarData::MeanScalarData(const std::string & name,
                               const double target,
                               const double sigma,
                               const int pos) :
    name_(name),
    target_(target),
    sigma_(sigma),
    one_over_sigma2_(1.0/(sigma*sigma)),
    pos_(pos)
{
    // NOTHING HERE
}

// -------------------------------------------------------------------------- //
//
double MeanScalarData::calculate_chi2(const double value) const
{
    const double diff = value - target_;
    return diff * diff * one_over_sigma2_;
}

// -------------------------------------------------------------------------- //
//
void MeanScalarData::init(const std::vector<int> & sampleset,
                          const Library & library)
{
    // Get the number of elements in the sample set.
    nsample_ = static_cast<int>(sampleset.size());

    // Zero the value.
    value_ = 0.0;

    // Loop through the sample set and extract the scalar value.
    for (size_t i = 0; i < sampleset.size(); ++i)
    {
        const int index    = sampleset[i];
        const double value = library.get_scalar_at(index, pos_);
        value_ += value;
    }

    // Take the mean value.
    value_ /= nsample_;

    // Calculate chi2.
    chi2_ = calculate_chi2(value_);
}

// -------------------------------------------------------------------------- //
//
void MeanScalarData::notify(const int from_sample,
                            const int from_basis,
                            const Library & library)
{
    // Determine the new value.
    const double subtr_val = library.get_scalar_at(from_sample, pos_);
    const double add_val   = library.get_scalar_at(from_basis, pos_);
    value_new_ = value_ * nsample_;
    value_new_ -= subtr_val;
    value_new_ += add_val;
    value_new_ /= nsample_;

    // Determine the new chi2.
    chi2_new_ = calculate_chi2(value_new_);
}

// -------------------------------------------------------------------------- //
//
void MeanScalarData::accept()
{
    value_ = value_new_;
    chi2_ = chi2_new_;
}

// -------------------------------------------------------------------------- //
//
void MeanScalarData::print() const
{
    printf("                  %20s%15.7e %12.4e %12.4e\n",
           name_.c_str(),
           chi2_,
           value_,
           target_);
}

// -------------------------------------------------------------------------- //
//
void MeanScalarData::weighted_analysis(const Library & library,
                                       const std::vector<BasisContainer> & weights_table,
                                       const std::string & basename) const
{
    // Zero the value.
    double value = 0.0;
    double weighted_value = 0.0;

    // Loop through the library and extract the scalar values.
    for (size_t i = 0; i < weights_table.size(); ++i)
    {
        const int index     = weights_table[i].index;
        const double weight = weights_table[i].weight;
        const double val    = library.get_scalar_at(index, pos_);
        value += val;
        weighted_value += val * weight;
    }

    // Take the mean value.
    value /= weights_table.size();
    weighted_value /= weights_table.size();

    // Print to file, weighted and unweighted.
    std::string filename = basename + ".Mean-" + name_;
    std::ofstream outfile(filename.c_str());

    // Check that the file is ok.
    if (outfile.bad())
    {
        open_file_error(filename, LOCATION);
    }

    // Write.
    outfile << "     TARGET: " << target_ << std::endl;
    outfile << "      VALUE: " << value << std::endl;
    outfile << "   WEIGHTED: " << weighted_value << std::endl;
    outfile.close();
}

