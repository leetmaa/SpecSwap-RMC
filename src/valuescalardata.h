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


/*! \file  valuescalardata.h
 *  \brief File for the ValueScalarData class definition.
 */

#ifndef __VALUE_SCALAR_DATA__
#define __VALUE_SCALAR_DATA__

#include <string>
#include <vector>

#include "basiscontainer.h"

// Forward declarations.
class Library;

/// Class representing a value scalar data set.
class ValueScalarData {

public:

    /*! \brief Default constructor needed for initialization from specswap.
     */
    ValueScalarData() {};

    /*! \brief Constructor.
     *  \param name     : The name (ending) of the scalar.
     *  \param interval : The bounds the fit concern.
     *  \param target   : Target fraction value.
     *  \param sigma    : The sigma value.
     *  \param pos      : The index of the scalar in the library.
     */
    ValueScalarData(const std::string & name,
                    const std::pair<double,double> & interval,
                    const double target,
                    const double sigma,
                    const int pos);

    /*! \brief Function for setting up the initial mean value and initial chi2.
     *  \param sampleset : Vector holding the sample set indices.
     *  \param library   : The library to take the data from.
     */
    void init(const std::vector<int> & sampleset,
                const Library & library);

    /*! \brief Function to notify of an attempted move.
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

    /*! \brief Helper routine to calculate chi2.
     *  \param value : The value to calculate for.
     *  \return : chi2
     */
    double calculate_chi2(const double value) const;

    /// The name (ending) of the scalar.
    std::string name_;
    /// The interval we are fitting to.
    std::pair<double,double> interval_;
    /// The target value.
    double target_;
    /// The sigma value.
    double sigma_;
    /// The one over sigma2.
    double one_over_sigma2_;
    /// The value.
    double value_;
    /// The value for the attempted move.
    double value_new_;
    /// The chi2 value.
    double chi2_;
    /// The chi2 value of the attempted move.
    double chi2_new_;
    /// The index of the scalar in the library.
    int pos_;
    /// The number of elements in the sample set.
    int nsample_;

    /// Declaring the test class as friend to facilitate testing.
    friend class Test_ValueScalarData;
};

#endif
