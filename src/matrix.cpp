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


/*! \file  matrix.cpp
 *  \brief File for the Matrix class implementation.
 */

#include "matrix.h"


// -------------------------------------------------------------------------- //
//
Matrix::Matrix() :
    rows_(0),
    columns_(0),
    data_(0, std::vector<double>(0))
{
    // NOTHING HERE
}

// -------------------------------------------------------------------------- //
//
Matrix::Matrix(int rows, int columns) :
    rows_(rows),
    columns_(columns),
    data_(rows, std::vector<double>(columns_, 0.0))
{
    // NOTHING HERE
}


// -------------------------------------------------------------------------- //
//
void Matrix::resize(int rows, int columns)
{
    // NOTE: This matrix is not use in any performance sensitive code.

    // Set the new member data.
    rows_ = rows;
    columns_ = columns;
    // Resize.
    data_.resize(0);
    for (int i = 0; i < rows_; ++i)
    {
        data_.push_back(std::vector<double>(columns_, 0.0));
    }
}



