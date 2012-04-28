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


/*! \file  mlrnd.h
 *  \brief File for the Mlrnd random number generator class definition.
 */


#ifndef __MLRND__
#define __MLRND__

/// Class for representing the random number generator.
class Mlrnd {

 public:

    /*! \brief Default constructor.
     */
    Mlrnd();

    /*! \brief Function for setting the seed value.
     *  \param seed : The seed value to use.
     */
    void set_seed(int seed);

    /*! \brief Function for getting the next random number.
     *  \note : This function calls the 'static' fortran backend.
     *  \return : A pseudo random number between 0 and 1.
     */
    double random() const;

 private:

    /// The seed value.
    int seed_;

    /// Declaring the test class as friend to facilitate testing.
    friend class Test_Mlrnd;

};

#endif

