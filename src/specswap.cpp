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


/*! \file  specswap.cpp
 *  \brief File for the Specswap class implementation.
 */

#include <cstdlib>
#include <cmath>
#include <fstream>
#include <stdexcept>

#include "specswap.h"
#include "mathutils.h"
#include "ioutils.h"


// ----------------------------------------------------------------------------
// Normal Constructor
Specswap::Specswap(const int nsample,
                   const Mlrnd & rand,
                   const std::string & library_path) :
    rand_(rand),
    library_(library_path.c_str()),
    sampleset_(nsample),
    nsample_(nsample),
    ncurves_(library_.get_ncurves()),
    nprobe_(0),
    dump_counter_(0),
    restart_(false)
{
    // Check that the number of basis functions in the sample set is not larger
    // than half the number of basis functions in total in the library. This is
    // a rather arbitrary constraint, however reasonable.
    if (nsample_ > (library_.get_nbase() / 2))
    {
        error("nsample_ too large. (nsample_ > (nbase / 2))\n", LOCATION);
    }
}


// ----------------------------------------------------------------------------
// Constructor for restart.
Specswap::Specswap(const int nsample,
                   const Mlrnd & rand,
                   const std::string & library_path,
                   const std::string & restart_path) :
    rand_(rand),
    library_(library_path.c_str()),
    sampleset_(nsample),
    nsample_(nsample),
    ncurves_(library_.get_ncurves()),
    nprobe_(0),
    dump_counter_(0),
    restart_(true),
    restart_path_(restart_path)
{
    if (nsample_ > (library_.get_nbase() / 2))
    {
        error("nsample_ too large. (nsample_ > (nbase / 2))\n", LOCATION);
    }
}

// ----------------------------------------------------------------------------
//
// Functions for handling input.
//
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//
void Specswap::add_scalar_mean(const std::string & scalarname,
                               const double target,
                               const double sigma)
{

    // find the position of the name.
    const int pos = calc_scalar_pos(scalarname);
    mean_scalar_data_.push_back(MeanScalarData(scalarname, target, sigma, pos));
}

// ----------------------------------------------------------------------------
//
void Specswap::add_scalar_value(const std::string& scalarname,
                                const double value_low,
                                const double value_high,
                                const double fraction,
                                const double sigma)
{
    // Find the position of the this name.
    const int pos = calc_scalar_pos(scalarname);
    value_scalar_data_.push_back(ValueScalarData(scalarname, std::pair<double,double>(value_low,value_high), fraction, sigma, pos));
}

// ----------------------------------------------------------------------------
//
void Specswap::add_scalar_distribution(const std::string& scalarname,
                                       const std::string& filename,
                                       const double sigma)
{
    // Find the position of the name.
    const int pos = calc_scalar_pos(scalarname);
    scalar_distribution_data_.push_back(ScalarDistributionData(scalarname, filename, sigma, pos));
}

// ----------------------------------------------------------------------------
//
const int Specswap::calc_scalar_pos(const std::string& scalarname) const
{
    // Find the position of the name in the library.
    int pos = 0;
    bool found = false;
    const int nscalars = library_.get_nscalars();

    for (int i = 0; i < nscalars; ++i)
    {
        if (library_.get_scalar_name(i) == scalarname)
        {
            found = true;
            break;
        }
        pos++;
    }

    if (!found)
    {
        // Throw an error if the scalar name was not present in the library.
        std::string msg = "Name \"" + scalarname +"\" not found in library.\n";
        msg = msg + "            Scalar names in library:\n";
        for (int i = 0; i < nscalars; ++i)
        {
            msg = msg + "            " + library_.get_scalar_name(i) + "\n";
        }
        error(msg, LOCATION);
    }

    // Return the number we have found.
    return pos;
}

// ----------------------------------------------------------------------------
//
void Specswap::add_curve(const double sigma,
                         const bool area_renorm,
                         const std::string & curve_name,
                         const std::string & path)
{
    // Add a new curve data set.
    curve_data_.push_back(CurveData(sigma,
                                    area_renorm,
                                    curve_name,
                                    path,
                                    library_));
}

// ----------------------------------------------------------------------------
//
void Specswap::add_pcf(const double rmin,
                       const double rmax,
                       const double dr,
                       const double sigma,
                       const double numberdensity,
                       const std::pair<double,double> & fit_interval,
                       const int nbins,
                       const std::pair<int,int> & partial,
                       const std::string & path)
{
    // Setup a new pcf data object.
    pcf_data_.push_back(PCFData(rmin,
                                rmax,
                                dr,
                                sigma,
                                numberdensity,
                                fit_interval,
                                nbins,
                                partial,
                                path));
}


// ----------------------------------------------------------------------------
//
// Functions for setup after input is read.
//
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
//
void Specswap::setup()
{
    // Setup the sampleset.
    setup_sampleset();

    // Setup the pcfs.
    for (size_t i = 0; i < pcf_data_.size(); ++i)
    {
        pcf_data_[i].init(sampleset_.get_indices(), library_);
    }

    // Setup the curves.
    for (size_t i = 0; i < curve_data_.size(); ++i)
    {
        curve_data_[i].init(sampleset_.get_indices(), library_);
    }

    // Setup the mean scalars.
    for (size_t i = 0; i < mean_scalar_data_.size(); ++i)
    {
        mean_scalar_data_[i].init(sampleset_.get_indices(), library_);
    }

    // Setup the value scalars.
    for (size_t i = 0; i < value_scalar_data_.size(); ++i)
    {
        value_scalar_data_[i].init(sampleset_.get_indices(), library_);
    }

    // Setup the scalar distributions.
    for (size_t i = 0; i < scalar_distribution_data_.size(); ++i)
    {
        scalar_distribution_data_[i].init(sampleset_.get_indices(), library_);
    }

    // Setup the initial chi2.
    setup_chi2();
}

// ----------------------------------------------------------------------------
//
void Specswap::setup_sampleset()
{
    // Get the number of basis elements in the library.
    const int nbase = library_.get_nbase();

    // Initialize the weights vector.
    weights_.resize(nbase, 0);

    // Read restart information.
    if (restart_)
    {
        printf("Reading restart information from file %s\n", restart_path_.c_str());

        // Open the restart file.
        std::ifstream restart;
        restart.open(restart_path_.c_str());

        // Check for opening errors.
        if (!restart)
        {
            open_file_error(restart_path_);
        }

        // Read the weights.
        std::vector<int> tmp_weights;
        int tmp_size;
        restart >> tmp_size;

        // Read the weights.
        for (int i = 0; i < tmp_size; ++i)
        {
            int tmp_value;
            restart >> tmp_value;
            tmp_weights.push_back(tmp_value);
        }

        // Check the size.
        if (static_cast<int>(tmp_weights.size()) != nbase)
        {
            error("Incompatible size of the restart file and the library.\n");
        }

        // Copy the weights over.
        for (int i = 0; i < nbase; ++i)
            weights_[i] = tmp_weights[i];

        // Read the nprobe info.
        restart >> nprobe_;

        // Read the sample set to restart from.
        std::vector<int> tmp_sample;
        restart >> tmp_size;
        for (int i = 0; i < tmp_size; ++i)
        {
            int tmp_value;
            restart >> tmp_value;
            tmp_sample.push_back(tmp_value);
        }

        // Check the size.
        if (static_cast<int>(tmp_sample.size()) != nsample_)
        {
            error("Incompatible size of the restart file and the sampleset.\n");
        }

        // Set up the sampleset using the restart information.
        for (int i = 0; i < nsample_; ++i)
        {
            // Take out the index.
            const int index = tmp_sample[i];
            // Add the index.
            sampleset_.add_index(index);
        }

        // Close the restart file after finnishing reading.
        restart.close();

        // Tell std out that we managed to read restart.
        printf("Done reading restart information.\n");
    }

    else // If not a restart.
    {
        // Setup a vector of added indices.
        std::vector<int> added;

        // Loop until we have added as many as we should.
        while(static_cast<int>(added.size()) < nsample_)
        {
            // Get the index of a random basis function.
            const int index = random_basis();

            // Check if it is allready added.
            bool break_flag = false;
            for (size_t i = 0; i < added.size(); ++i)
            {
                if (index == added[i])
                {
                    break_flag = true;
                    break;
                }
            }

            // If it is not allready added.
            if (!break_flag)
            {
                // Add to the list of added indices.
                added.push_back(index);
                // Add the index.
                sampleset_.add_index(index);
            }
        }
    }

}


// ----------------------------------------------------------------------------
//
void Specswap::setup_chi2() {

    // Start by setting the chi2 value on the base class to zero.
    chi2_ = 0.0;

    // Chi2 from pcfs.
    for (size_t i = 0; i < pcf_data_.size(); ++i)
    {
        chi2_ += pcf_data_[i].get_chi2();
    }

    // Chi2 from curves.
    for (size_t i = 0; i < curve_data_.size(); ++i)
    {
        chi2_ += curve_data_[i].get_chi2();
    }

    // Chi2 from mean scalars.
    for (size_t i = 0; i < mean_scalar_data_.size(); ++i)
    {
        chi2_ += mean_scalar_data_[i].get_chi2();
    }

    // Chi2 from value scalars.
    for (size_t i = 0; i < value_scalar_data_.size(); ++i)
    {
        chi2_ += value_scalar_data_[i].get_chi2();
    }

    // Chi2 from value scalar distributions.
    for (size_t i = 0; i < scalar_distribution_data_.size(); ++i)
    {
        chi2_ += scalar_distribution_data_[i].get_chi2();
    }

}


// ----------------------------------------------------------------------------
//
// Functions for running the simulation.
//
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
//
void Specswap::move()
{
    // A move is set up here by defining what elements to swap.

    // This is the global index of the element we want to swap out of the sampleset.
    slot_        = random()*nsample_;
    from_sample_ = sampleset_.index_at(slot_);

    // This is the global index of the element we want to swap in to the sampleset.
    from_basis_ = random_basis();
    // If the from_basis_ element is allready in the sample set, pick a new element to swap.
    while ( sampleset_.is_added(from_basis_) )
    {
        from_basis_ = random_basis();
    }
}

// ----------------------------------------------------------------------------
//
void Specswap::notify() {

    // Zero out the new chi2.
    chi2_new_ = 0.0;

    // Notify the pcfs and add their chi2 to the total.
    for (size_t i = 0; i < pcf_data_.size(); ++i)
    {
        pcf_data_[i].notify(from_sample_, from_basis_, library_);
        chi2_new_ += pcf_data_[i].get_chi2_new();
    }

    // Notify the curves and add their chi2 to the total.
    for (size_t i = 0; i < curve_data_.size(); ++i)
    {
        curve_data_[i].notify(from_sample_, from_basis_, library_);
        chi2_new_ += curve_data_[i].get_chi2_new();
    }

    // Notify the mean scalars and add their chi2 to the total.
    for (size_t i = 0; i < mean_scalar_data_.size(); ++i)
    {
        mean_scalar_data_[i].notify(from_sample_, from_basis_, library_);
        chi2_new_ += mean_scalar_data_[i].get_chi2_new();
    }

    // Notify the value scalars and add their chi2 to the total.
    for (size_t i = 0; i < value_scalar_data_.size(); ++i)
    {
        value_scalar_data_[i].notify(from_sample_, from_basis_, library_);
        chi2_new_ += value_scalar_data_[i].get_chi2_new();
    }

    // Notify the scalar distributions and add their chi2 to the total.
    for (size_t i = 0; i < scalar_distribution_data_.size(); ++i)
    {
        scalar_distribution_data_[i].notify(from_sample_, from_basis_, library_);
        chi2_new_ += scalar_distribution_data_[i].get_chi2_new();
    }
}


// ----------------------------------------------------------------------------
//
void Specswap::accept()
{
    // Update the sampleset.
    sampleset_.swap_into_slot(from_basis_, slot_);

    // Accept the new chi2 in the pcfs.
    for (size_t i = 0; i < pcf_data_.size(); ++i)
    {
        pcf_data_[i].accept();
    }

    // Accept the new chi2 in the curves.
    for (size_t i = 0; i < curve_data_.size(); ++i)
    {
        curve_data_[i].accept();
    }

    // Accept the new chi2 in the mean scalars.
    for (size_t i = 0; i < mean_scalar_data_.size(); ++i)
    {
        mean_scalar_data_[i].accept();
    }

    // Accept the new chi2 in the value scalars.
    for (size_t i = 0; i < value_scalar_data_.size(); ++i)
    {
        value_scalar_data_[i].accept();
    }

    // Accept the new chi2 in the scalar distributions.
    for (size_t i = 0; i < scalar_distribution_data_.size(); ++i)
    {
        scalar_distribution_data_[i].accept();
    }

    // Accept the new chi2.
    chi2_ = chi2_new_;
}

// ----------------------------------------------------------------------------
//
void Specswap::collect_weights()
{
    const std::vector<int> & indices = sampleset_.get_indices();
    // Loop over the sampleset.
    for (size_t i = 0; i < indices.size(); ++i)
    {
        // Increment the weight at each index.
        weights_[indices[i]] += 1;
    }

    // Incremente the probe counter.
    nprobe_ += 1;

}


// ----------------------------------------------------------------------------
//
//  Functions for IO.
//
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
//
void Specswap::print() const
{
    // consistent output please.
    printf(" Chi2 Information -------------------------- Chi2 ----------------------------\n");

    // Print pcfs.
    if (pcf_data_.size() > static_cast<size_t>(0))
    {
        printf("       From pcfs:\n");

        for (size_t i = 0; i < pcf_data_.size(); ++i)
        {
            pcf_data_[i].print();
        }
    }

    // Print curves.
    if (curve_data_.size() > static_cast<size_t>(0))
    {
        printf("       From curves:\n");

        for (size_t i = 0; i < curve_data_.size(); ++i)
        {
            curve_data_[i].print();
        }
    }

    // Print scalar distributions.
    if (scalar_distribution_data_.size() > static_cast<int>(0))
    {
        printf(" From scalar distributions:\n");
        for (size_t i = 0; i < scalar_distribution_data_.size(); ++i)
        {
            scalar_distribution_data_[i].print();
        }
    }

    // Print mean scalars.
    if (mean_scalar_data_.size() > static_cast<int>(0))
    {
        printf("  From mean scalars:\n                                                         Value        Target\n");
        for (size_t i = 0; i < mean_scalar_data_.size(); ++i)
        {
            mean_scalar_data_[i].print();
        }
    }

    // Print value scalars.
    if (value_scalar_data_.size() > static_cast<int>(0))
    {
        printf("  From value scalars:\n                       Interval                          Value        Target\n");
        for (size_t i = 0; i < value_scalar_data_.size(); ++i)
        {
            value_scalar_data_[i].print();
        }
    }

    // Print total chi2.
    printf("\n       Total Chi2:        %15.7e\n", chi2_);

    // Some pretty line.
    printf("_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n\n");

}


// ----------------------------------------------------------------------------
//
void Specswap::write_dump(const std::string & title)
{
    // Setup the base name.
    char tmp_char[300];
    sprintf(tmp_char, "_%i.", dump_counter_);
    std::string basename = title + tmp_char;
    // Print each dataset to file.
    print_to_file(basename);
    ++dump_counter_;
}


// ----------------------------------------------------------------------------
//
void Specswap::print_start() const
{
    std::string tmp_str("start.sample");
    print_to_file(tmp_str);
}


// ----------------------------------------------------------------------------
//
void Specswap::print_stop() const
{
    std::string tmp_str("stop.sample");
    print_to_file(tmp_str);
}


// ----------------------------------------------------------------------------
//
void Specswap::print_to_file(const std::string& basename) const
{
    // Print all pcf data.
    for (size_t i = 0; i < pcf_data_.size(); ++i)
    {
        char tmp_str[20];
        sprintf(tmp_str, "_pcfdata%i", static_cast<int>(i));
        std::string extended_basename = basename + tmp_str;
        pcf_data_[i].print_to_file(extended_basename);
    }

    // Print all curve data.
    for (size_t i = 0; i < curve_data_.size(); ++i)
    {
        char tmp_str[20];
        sprintf(tmp_str, "_curvedata%i", static_cast<int>(i));
        std::string extended_basename = basename + tmp_str;
        curve_data_[i].print_to_file(extended_basename);
    }
}


// ----------------------------------------------------------------------------
//
void Specswap::print_weights(std::ofstream& output, const int weight) const
{
    // Get the number pf basis elements in the library.
    const int nbase = static_cast<int>(weights_.size());

    std::vector<double> A(nbase);
    for (int i = 0; i < nbase; ++i)
        A[i] = (double)weights_[i];

    double one_over_weight = 1.0e0 / weight;
    A = A * one_over_weight;

    output << " weight: " << std::endl;
    output << A.size() << std::endl;
    for ( size_t i = 0; i < A.size(); ++i)
    {
        output << A[i] << " ";
    }
    output << std::endl;
    output.flush();
}


// ----------------------------------------------------------------------------
//
void Specswap::write_restart(const std::string & path) const
{
    // Open the output path in re-weite mode.
    std::ofstream out(path.c_str());

    // Write the weights.
    out << weights_.size() << std::endl;

    for (size_t i = 0; i < weights_.size(); ++i)
    {
        out << weights_[i] << " ";
    }
    out << std::endl;

    // Write the times we probed.
    out << nprobe_ << std::endl;

    // Write the sampleset.
    out << sampleset_.get_indices().size() << std::endl;

    for (size_t i = 0; i < sampleset_.get_indices().size(); ++i)
    {
        out << sampleset_.get_indices()[i] << " ";
    }
    out << std::endl;
    out.close();
}


// ----------------------------------------------------------------------------
//
//  Other utility functions.
//
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
//
const int Specswap::random_basis() const
{
    const int nbase = static_cast<int>(weights_.size());
    // return a random integer number between 0 and nbase - 1
    return static_cast<int> (random()*nbase);
}


// ----------------------------------------------------------------------------
//
double Specswap::random() const
{
    // returns a random floating point number between 0.0 and 1.0
    return rand_.random();
}


// ----------------------------------------------------------------------------
//
//  Functions for Analysis.
//
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
//
void Specswap::prepare_for_analysis()
{
    // Loop over all indices, place their name and index and weight in a
    // basis container.
    if (weights_table_.empty())
    {
        // Normalize such that the sum of all weights equals the number of
        // basis function in the library.
        const int sum = vsum(weights_);
        if (sum == 0)
        {
            error("Can not preform ANALYSIS with all weights zero.\n");
        }

        const double norm = static_cast<double>(weights_.size()) / sum;
        for (size_t i = 0; i < weights_.size(); ++i)
        {
            BasisContainer b;
            b.index  = i;
            b.weight = weights_[i] * norm;
            b.name   = library_.get_basis_name(i);
            weights_table_.push_back(b);
        }
    }
}


// ----------------------------------------------------------------------------
//
void Specswap::write_weights_and_names_list(const std::string & title)
{
    // Setup for analysis.
    prepare_for_analysis();

    // Sort the weights table according to weight.
    std::sort(weights_table_.begin(), weights_table_.end(), BasisContainer::compareWeight );

    // Setup the file name.
    const std::string filename = title + "_weights_and_names.ANALYSIS";

    // Open the file.
    std::ofstream outfile(filename.c_str());

    // Print to file.
    for (size_t i = 0; i < weights_table_.size(); ++i)
    {
        // Get the data out.
        const int index = weights_table_[i].index;
        const double weight = weights_table_[i].weight;
        const std::string name = weights_table_[i].name;

        // Print.
        char tmp_str[256];
        sprintf( tmp_str, "%10i %20.10f      %s\n", index, weight, name.c_str() );
        std::string line(tmp_str);
        outfile << line;
    }

    // Done.
    outfile.close();
}


// ----------------------------------------------------------------------------
//
void Specswap::weighted_analysis(const std::string & title)
{
    // Setup for analysis.
    prepare_for_analysis();

    // Sort the weights table according to index.
    std::sort(weights_table_.begin(), weights_table_.end(), BasisContainer::compareIndex );

    // Setup the file name.
    const std::string basename = "ANALYSIS/" + title + "_weighted.ANALYSIS";

    // Analyse as one large chunk.
    analyse_chunk(weights_table_, basename);
}


// ----------------------------------------------------------------------------
//
void Specswap::chunk_analysis(const std::string& title, const int chunks, const int chunk_size)
{
    // Setup for analysis.
    prepare_for_analysis();

    // Sort the weights table according to weight.
    std::sort(weights_table_.begin(), weights_table_.end(), BasisContainer::compareWeight );

    // Split the table up in chunks.
    int counter = 0;
    for (int i = 0; i < chunks; ++i)
    {
        std::vector<BasisContainer> chunk;
        for (int j = 0; j < chunk_size; ++j)
        {
            chunk.push_back(weights_table_[counter]);
            ++counter;

            // Break if we pass the length of the table.
            if (counter == static_cast<int>(weights_table_.size()))
            {
                break;
            }
        }

        // Normalize the chunk weights.
        double sum = 0.0;
        for (size_t j = 0; j < chunk.size(); ++j)
        {
            sum += chunk[j].weight;
        }

        // Handle the case if all weights in the chunk are zero.
        if ( sum < 1.0e-8 )
        {
            // NOTE: Write a note in the manual about this normalization.
            for (size_t j = 0; j < chunk.size(); ++j)
            {
                chunk[j].weight = 1.0;
            }
        }
        else
        {
            // The normal case, noramlize to one.
            double norm = chunk.size() / sum;
            for (size_t j = 0; j < chunk.size(); ++j)
            {
                chunk[j].weight *= norm;
            }
        }

        // Setup the file name.
        const std::string basename = "ANALYSIS/" + title + "_chunk" + to_string(i) + ".ANALYSIS";

        // Analyse the chunk.
        analyse_chunk(chunk, basename);

        // Break if we pass the length of the table.
        if (counter == static_cast<int>(weights_table_.size()))
        {
            break;
        }
    }
}


// ----------------------------------------------------------------------------
//
void Specswap::analyse_chunk(const std::vector<BasisContainer> & weights_table,
                             const std::string & basename)
{
    // Make weighted analysis of the curves.
    for (size_t i = 0; i < curve_data_.size(); ++i)
    {
        curve_data_[i].weighted_analysis(library_, weights_table, basename);
    }

    // Make weighted analysis of the pcf data.
    for (size_t i = 0; i < pcf_data_.size(); ++i)
    {
        pcf_data_[i].weighted_analysis(library_, weights_table, basename);
    }

    // Make weighted analysis of the scalar distribution data.
    for (size_t i = 0; i < scalar_distribution_data_.size(); ++i)
    {
        scalar_distribution_data_[i].weighted_analysis(library_, weights_table, basename);
    }

    // Make weighted analysis of the value scalar data.
    for (size_t i = 0; i < value_scalar_data_.size(); ++i)
    {
        value_scalar_data_[i].weighted_analysis(library_, weights_table, basename);
    }

    // Make weighted analysis of the mean scalar data.
    for (size_t i = 0; i < mean_scalar_data_.size(); ++i)
    {
        mean_scalar_data_[i].weighted_analysis(library_, weights_table, basename);
    }
}


