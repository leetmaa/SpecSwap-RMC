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


/*! \file  sampleset.h
 *  \brief File for the Sampleset class definition.
 */

#ifndef __SAMPLE__
#define __SAMPLE__

#include <vector>
#include <string>


/// Class representing the Sampleset in the SpecSwap simulation, for book keeping indices and performing swap moves.
class Sampleset {

public:

    /*! \brief Default constructor needed for setup from specswap.
     *  \param nsample    : The number of basis elements in the sample set.
     */
    Sampleset(const int nsample=0);

    /*! \brief Used during setup to add elements to the sampleset.
     *  \param index     : The corresponding index.
     */
    void add_index(int index);

    /*! \brief Routine for performing the swap moves.
     *  \param new_index          : The new index at the specified slot.
     *  \param slot               : The slot in the sampleset to swap at.
     */
    void swap_into_slot(int new_index,
                        int slot);

    /*! \brief Query function for the basis index at a specified slot.
     *  \param slot : The slot to the index at.
     *  \return     : The corresponding index.
     */
    const int index_at(const int slot) {return indices_[slot];};

    /*! \brief Query function for the index vector.
     *  \return     : The indices.
     */
    const std::vector<int> & get_indices() const {return indices_;};

    /*! \brief Function to check if an index is present in the sampleset.
     *  \return    : True if the index is present in the sampleset, otherwise false.
     */
    const bool is_added(const int index) const;

protected:

private:

    /// The indices vector, holding all indices in the sampleset.
    std::vector<int> indices_;

    /// Total number of basis functions in the sampleset.
    int nsample_;

    // Declaring the test class as friend to facilitate testing.
    friend class Test_Sampleset;

};//class

#endif


