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


/*! \file  sampleset.cpp
 *  \brief File for the Sampleset class implementation.
 */

#include <algorithm>
#include <fstream>
#include <cstdlib>

#include "sampleset.h"
#include "mathutils.h"
#include "ioutils.h"

// ----------------------------------------------------------------------------
//
Sampleset::Sampleset(const int nsample) :
    nsample_(nsample)
{
    // NOTHING HERE
}

// ----------------------------------------------------------------------------
//
void Sampleset::add_index(const int index)
{
    // This function is used for setup only, thus this check.
    if (static_cast<int>(indices_.size()) == nsample_)
    {
        error("Tried to add too many basis functions.\n", LOCATION);
    }
    // Push the index in the indices vector.
    indices_.push_back(index);
}

// ----------------------------------------------------------------------------
//
void Sampleset::swap_into_slot(const int new_index,
                               const int slot)
{
    // Replace the old index.
    indices_[slot] = new_index;
}

// ----------------------------------------------------------------------------
//
const bool Sampleset::is_added(const int index) const
{
    // Check if we have this one added.
    for (size_t i = 0; i < indices_.size(); ++i)
    {
        if (indices_[i] == index)
        {
            return true;
        }
    }
    return false;
}

