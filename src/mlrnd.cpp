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


/*! \file  mlrnd.cpp
 *  \brief File for the Mlrnd random number generator class implementation.
 */

#include <cstdlib>
#include <cstdio>

#include "mlrnd.h"
#include "randf.h"


// ----------------------------------------------------------------------------
//
Mlrnd::Mlrnd() : seed_(13)
{
    // NOTHING HERE
}


// ----------------------------------------------------------------------------
//
void Mlrnd::set_seed(int seed)
{
    seed_ = seed;
}


// ----------------------------------------------------------------------------
//
double Mlrnd::random() const
{
    double rnd;
    int seed = seed_;
    rnd1_(seed, rnd);
    return rnd;
}

