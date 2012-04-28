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


/*! \file  curvedata.cpp
 *  \brief File for the CurveData class implementation.
 */

#include <fstream>
#include <cmath>

#include "ioutils.h"
#include "mathutils.h"
#include "library.h"
#include "curvedata.h"


// -------------------------------------------------------------------------- //
//
CurveData::CurveData(const double sigma,
                     const bool area_renorm,
                     const std::string & curve_name,
                     const std::string & path,
                     const Library & library) :
    sigma_(sigma),
    one_over_sigma2_(1.0/(sigma*sigma)),
    area_renorm_(area_renorm),
    curve_name_(curve_name)
{
    // Determine which position in the library this curve name corresponds to.
    const int ncurves = library.get_ncurves();
    bool found = false;
    for (int i = 0; i < ncurves; ++i)
    {
        if (curve_name_ == library.get_ending(i))
        {
            // We found the position.
            pos_ = i;
            found = true;
            break;
        }
    }

    // If not found, print an error message.
    if (!found)
    {
        std::string message;
        char tmpstr[150];
        sprintf(tmpstr,
                "Identifier \'%s\' not found in library \'%s\'\n",
                curve_name_.c_str(),
                library.get_name().c_str());
        message = std::string(tmpstr) + std::string("           Curve names in library: \n");
        // Print all names in the library.
        for (int i = 0; i < ncurves; ++i) {
            char tmpstr2[150];
            sprintf(tmpstr2, "           %s\t", library.get_ending(i).c_str());
            message = message + std::string(tmpstr2);
        }//for
        error(message, LOCATION);
    }

    // Get the scale from the library. What we read in later must match.
    scale_ = library.get_scale(pos_);

    // Resize the curve.
    curve_.resize(scale_.size(), 0.0);

    // Resize the reference.
    curve_reference_.resize(scale_.size(), 0.0);

    // Read in the reference curve.
    read_reference(path);

}

// -------------------------------------------------------------------------- //
//
void CurveData::read_reference(const std::string & path)
{
    // Open the file.
    std::ifstream infile(path.c_str());
    check_eof(infile, path, LOCATION);

    // Read all data.
    for (size_t i = 0; i < curve_reference_.size(); ++i)
    {
        double scale_value;
        infile >> scale_value >> curve_reference_[i];
        check_eof(infile, path, LOCATION);

        // Make sure the scale value is correct.
        double diff = scale_[i] - scale_value;
        diff = std::sqrt(diff*diff);

        if (diff > 1.0e-6)
        {
            std::string msg = "Unexpected step size in file: \"" + path + "\"\n";
            error(msg, LOCATION);
        }
    }

    // Close the file after reading.
    infile.close();

}


// -------------------------------------------------------------------------- //
//
const double CurveData::calculate_chi2(const std::vector<double> & curve)
{
    // Take a working copy.
    std::vector<double> data = curve;

    // Area renormalize if required.
    if (area_renorm_)
    {
        vnormalize(data, curve_reference_);
    }

    // Take the elementwize difference and store in the working copy.
    vsub(data, curve_reference_);

    // Square the difference.
    vsquare(data);

    // This is the chi2 value, withouth sigma.
    const double sum = vsum(data);

    // Divide with sigma squared and save.
    const double chi2 = sum * one_over_sigma2_;

    // All done.
    return chi2;
}


// -------------------------------------------------------------------------- //
//
void CurveData::init(const std::vector<int> & sampleset,
                     const Library & library)
{
    // Save the number of basis elements in the sampleset.
    nsample_ = static_cast<int>(sampleset.size());

    // Loop over all indices in the sampleset.
    for (size_t i = 0; i < sampleset.size(); ++i)
    {
        // For each, get the curve and add to the curve data.
        const int index = sampleset[i];
        const std::vector<double> curve = library.get_at(index, pos_);
        vadd(curve_, curve);
    }

    // Divide by the number of elements in the sampleset.
    curve_ = curve_ / nsample_;

    // Calculate chi2.
    chi2_ = calculate_chi2(curve_);
}


// -------------------------------------------------------------------------- //
//
void CurveData::notify(const int from_sample,
                       const int from_basis,
                       const Library & library)
{
    // Multiply by nsample_ for correct normalization in add and subtract.
    curve_new_ = curve_ * nsample_;

    // Subtract the curve from the sample set.
    const std::vector<double> curve_to_subtract = library.get_at(from_sample, pos_);
    vsub(curve_new_, curve_to_subtract);

    // Add the curve from the basis set.
    const std::vector<double> curve_to_add      = library.get_at(from_basis, pos_);
    vadd(curve_new_, curve_to_add);

    // Divide again with nsample_ to get correct normalization.
    curve_new_ = curve_new_ / nsample_;

    // Calculate the new chi2.
    chi2_new_ = calculate_chi2(curve_new_);
}


// -------------------------------------------------------------------------- //
//
void CurveData::accept()
{
    // Save the new data and chi2.
    curve_ = curve_new_;
    chi2_  = chi2_new_;
}

// -------------------------------------------------------------------------- //
// Functions for IO
// -------------------------------------------------------------------------- //


// -------------------------------------------------------------------------- //
//
void CurveData::print() const
{
    printf("      %20s%15.7e\n",
           curve_name_.c_str(),
           chi2_);
}

// -------------------------------------------------------------------------- //
//
void CurveData::print_to_file(const std::string& basename) const
{
    // Open the file.
    std::string filename = basename + "." + curve_name_;
    std::ofstream outfile(filename.c_str());

    // Check that the file is ok.
    if (outfile.bad())
    {
        open_file_error(filename, LOCATION);
    }

    std::vector<double> tmp_curve = curve_;
    if (area_renorm_)
    {
        vnormalize(tmp_curve, curve_reference_);
    }

    outfile << "SCALE              CURVE               REFERENCE" << std::endl;
    for (size_t i = 0; i < tmp_curve.size(); ++i)
    {
        char line[300];
        sprintf(line, "%20.10f %20.10f %20.10f\n", scale_[i], tmp_curve[i], curve_reference_[i]);
        outfile << std::string(line);
    }
    outfile.close();
}


// -------------------------------------------------------------------------- //
//
void CurveData::weighted_analysis(const Library & library,
                                  const std::vector<BasisContainer> & weights_table,
                                  const std::string & basename) const
{
    // Compute the sums.
    std::vector<double> curve_sum(scale_.size(), 0.0);
    std::vector<double> weighted_curve_sum(scale_.size(), 0.0);
    std::vector<double> weighted_component(scale_.size(), 0.0);

    for (size_t i = 0; i < weights_table.size(); ++i)
    {
        // Extract index and weight.
        const int index  = weights_table[i].index;
        const int weight = weights_table[i].weight;

        // Add weighted and unweighted.
        vadd(curve_sum, library.get_at(index,pos_));
        weighted_component = library.get_at(index,pos_) * weight;
        vadd(weighted_curve_sum, weighted_component);
    }

    // Normalize.
    const int nbase = static_cast<int>(weights_table.size());
    curve_sum = curve_sum / nbase;
    weighted_curve_sum = weighted_curve_sum / nbase;

    // Normalize.
    if (area_renorm_)
    {
        vnormalize(curve_sum,          curve_reference_);
        vnormalize(weighted_curve_sum, curve_reference_);
    }

    // Print scale unweighted and weighted to file.
    std::string filename = basename + "." + curve_name_;
    std::ofstream outfile(filename.c_str());

    // Check that the file is ok.
    if (outfile.bad())
    {
        open_file_error(filename, LOCATION);
    }

    // Print to the file.
    outfile << "        SCALE                CURVE                 WEIGHTED             REFERENCE" << std::endl;
    for (size_t i = 0; i < scale_.size(); ++i)
    {
        char line[300];
        sprintf(line, "%20.10f %20.10f  %20.10f %20.10f\n", scale_[i], curve_sum[i], weighted_curve_sum[i], curve_reference_[i]);
        outfile << std::string(line);
    }
    outfile.close();
}

