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


/*! \file  basiscontainer.h
 *  \brief File for the BasisContainer definition and implementation.
 */

#ifndef __BASIS_CONTAINER__
#define __BASIS_CONTAINER__

/// A minimal struct to bunch together an index, a weight and a basis name.
struct BasisContainer {
    /// The index
    int index;
    /// The weight.
    double weight;
    /// The name.
    std::string name;
    /// Compare function to sort according to lowest index.
    static bool compareIndex(const BasisContainer & first, const BasisContainer & second) { return first.index < second.index; }
    /// Compare function to sort according to highest weight.
    static bool compareWeight(const BasisContainer & first, const BasisContainer & second) { return first.weight > second.weight; }

};


#endif
