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


/*! \file  mathutils.cpp
 *  \brief File containing implementations of the utility functions defined in mathutils.h
 */

#include <cmath>
#include <cstdio>

#include "matrix.h"
#include "mathutils.h"



// ----------------------------------------------------------------------------
//
double calculate_distance(const double point1[3], const double point2[3])
{
    return sqrt((point1[0]-point2[0])*(point1[0]-point2[0]) + (point1[1]-point2[1])*(point1[1]-point2[1]) + (point1[2]-point2[2])*(point1[2]-point2[2]));
}


// ----------------------------------------------------------------------------
//
Matrix setup_index_matrix(const int ntypes)
{
    // Allocate memory.
    Matrix index_matrix(ntypes,ntypes);

    // Setup the index matrix.
    int counter = 0;
    for (int i = 0; i < ntypes; ++i)
    {
        for (int j = i; j < ntypes; ++j)
        {
            index_matrix(i,j) = counter;
            index_matrix(j,i) = index_matrix(i,j);
            counter += 1;
        }
    }
    // Done.
    return index_matrix;
}


// ----------------------------------------------------------------------------
//
double vsum(std::vector<double> const & data)
{
    double ret = 0.0;

    for (size_t i = 0; i < data.size(); ++i)
    {
        ret += data[i];
    }

    /*
    // NOTE: This is slightly slower (on MacOS X 10.5 with gcc version 4.2.1)
    std::vector<double>::const_iterator iter1 = data.begin();
    const std::vector<double>::const_iterator iter_end = data.end();

    for ( ; iter1 != iter_end; ++iter1)
    {
        ret += *iter1;
    }
    */
    return ret;
}


// ----------------------------------------------------------------------------
//
int vsum(std::vector<int> const & data)
{
    int ret = 0.0;

    for (size_t i = 0; i < data.size(); ++i)
    {
        ret += data[i];
    }

    return ret;
}


// ----------------------------------------------------------------------------
//
std::vector<double> operator*(std::vector<double> const & data,
                              double constant)
{
    // Copy the data over to a new buffe.
    std::vector<double> ret(data);

    // Define the iterators.
    std::vector<double>::iterator iter1 = ret.begin();
    const std::vector<double>::iterator iter_end = ret.end();

    // Loop and perform the operation.
    for ( ; iter1 != iter_end; ++iter1)
    {
        *iter1 *= constant;
    }
    // Return.
    return ret;
}


// ----------------------------------------------------------------------------
//
std::vector<double> operator/(std::vector<double> const & data,
                              double constant)
{
    const double inv_constant = 1.0/constant;
    return data * inv_constant;
}


// ----------------------------------------------------------------------------
//
std::vector<double> operator*(std::vector<double> const & data1,
                              std::vector<double> const & data2)
{
    std::vector<double> ret(data1);

    for (size_t i = 0; i < ret.size(); ++i)
    {
        ret[i] *= data2[i];
    }
    return ret;
}


// ----------------------------------------------------------------------------
//
std::vector<double> operator+(std::vector<double> const & data1,
                              std::vector<double> const & data2)
{
    // Copy the data over to a new buffe.
    std::vector<double> ret(data1);

    // Define the iterators.
    std::vector<double>::iterator iter1 = ret.begin();
    std::vector<double>::const_iterator iter2 = data2.begin();
    const std::vector<double>::const_iterator iter_end = data2.end();

    // Loop and perform the operation.
    for ( ; iter2 != iter_end; ++iter1, ++iter2)
    {
        *iter1 += *iter2;
    }
    // Return.
    return ret;
}


// ----------------------------------------------------------------------------
//
std::vector<double> operator-(std::vector<double> const & data1,
                              std::vector<double> const & data2)
{
    // Copy the data over to a new buffe.
    std::vector<double> ret(data1);

    // Define the iterators.
    std::vector<double>::iterator iter1 = ret.begin();
    std::vector<double>::const_iterator iter2 = data2.begin();
    const std::vector<double>::const_iterator iter_end = data2.end();

    // Loop and perform the operation.
    for ( ; iter2 != iter_end; ++iter1, ++iter2)
    {
        *iter1 -= *iter2;
    }
    // Return.
    return ret;

}


// ----------------------------------------------------------------------------
//
std::vector<int> operator-(std::vector<int> const & data1,
                           std::vector<int> const & data2)
{
    // Copy the data over to a new buffe.
    std::vector<int> ret(data1);

    // Define the iterators.
    std::vector<int>::iterator iter1 = ret.begin();
    std::vector<int>::const_iterator iter2 = data2.begin();
    const std::vector<int>::const_iterator iter_end = data2.end();

    // Loop and perform the operation.
    for ( ; iter2 != iter_end; ++iter1, ++iter2)
    {
        *iter1 -= *iter2;
    }
    // Return.
    return ret;

}


// ----------------------------------------------------------------------------
//
void vadd(std::vector<double> & data1,
          std::vector<double> const & data2)
{
    // Define the iterators.
    std::vector<double>::iterator iter1 = data1.begin();
    std::vector<double>::const_iterator iter2 = data2.begin();
    const std::vector<double>::const_iterator iter_end = data2.end();

    // Loop and perform the operation.
    for ( ; iter2 != iter_end; ++iter1, ++iter2)
    {
        *iter1 += *iter2;
    }
}

// ----------------------------------------------------------------------------
//
void vadd(std::vector<int> & data1,
          std::vector<int> const & data2)
{
    // Define the iterators.
    std::vector<int>::iterator iter1 = data1.begin();
    std::vector<int>::const_iterator iter2 = data2.begin();
    const std::vector<int>::const_iterator iter_end = data2.end();

    // Loop and perform the operation.
    for ( ; iter2 != iter_end; ++iter1, ++iter2)
    {
        *iter1 += *iter2;
    }
}


// ----------------------------------------------------------------------------
//
void vsub(std::vector<double> & data1,
          std::vector<double> const & data2)
{
    // Define the iterators.
    std::vector<double>::iterator iter1 = data1.begin();
    std::vector<double>::const_iterator iter2 = data2.begin();
    const std::vector<double>::const_iterator iter_end = data2.end();

    // Loop and perform the operation.
    for ( ; iter2 != iter_end; ++iter1, ++iter2)
    {
        *iter1 -= *iter2;
    }
}


// ----------------------------------------------------------------------------
//
void vsub(std::vector<int> & data1,
          std::vector<int> const & data2)
{
    // Define the iterators.
    std::vector<int>::iterator iter1 = data1.begin();
    std::vector<int>::const_iterator iter2 = data2.begin();
    const std::vector<int>::const_iterator iter_end = data2.end();

    // Loop and perform the operation.
    for ( ; iter2 != iter_end; ++iter1, ++iter2)
    {
        *iter1 -= *iter2;
    }
}


// ----------------------------------------------------------------------------
//
void vsquare(std::vector<double> & data1)
{
    // Define the iterators.
    std::vector<double>::iterator iter1 = data1.begin();
    const std::vector<double>::iterator iter_end = data1.end();

    // Loop and perform the operation.
    for ( ; iter1 != iter_end; ++iter1)
    {
        *iter1 = (*iter1) * (*iter1);
    }
}


// ----------------------------------------------------------------------------
//
void vnormalize(std::vector<double> & data1,
                std::vector<double> const & data2)

{
    // Calculate the ratio to multiply with.
    const double norm2 = vsum(data2);
    const double norm1 = vsum(data1);
    const double ratio = norm2 / norm1;

    // Define the iterators.
    std::vector<double>::iterator iter1 = data1.begin();
    const std::vector<double>::iterator iter_end = data1.end();

    // Loop and perform the operation.
    for ( ; iter1 != iter_end; ++iter1)
    {
        *iter1 *= ratio;
    }
}

