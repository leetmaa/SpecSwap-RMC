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


/*! \file  mathutils.h
 *  \brief File containing definitions of utility functions and constants for mathematical operation.
 */

#include <vector>

// Forward declarations.
class Matrix;

/// Global definition of PI.
const double PI__ = 3.1415926535897932384626433832795028841971693993;

/*!\brief Fast floor implementation as suggested by shark (the mac profiling program)
 * \param f : The double to floor.
 * \return  : The floor value, i.e. the integer part of the value.
 */
inline
double fastfloor( const double f )
{
    register double twoTo52 = 4503599627370496.0;
    double c = ( f >= 0.0 ? -twoTo52 : twoTo52 );
    double result = (f - c) + c;
    if( f < result ) result -= 1.0;
    return result;
}

/*!\brief Function to calculate the distance between two points.
 * \param point1 The first point.
 * \param point2 The second point.
 * \return The distance between the two points.
 */
double calculate_distance(const double point1[3], const double point2[3]);


/*!\brief Generate the index matrix that maps partials to array indiceds.
 * \param ntypes : The number of atom types.
 * \return The index matrix.
 */
Matrix setup_index_matrix(const int ntypes);


/*!\brief Sum the elements of a vector.
 * \param data : The vector to sum.
 * \return     : The sum of the vector elements.
 */
double vsum(std::vector<double> const & data);


/*!\brief Sum the elements of a vector.
 * \param data : The vector to sum.
 * \return     : The sum of the vector elements.
 */
int vsum(std::vector<int> const & data);


/*!\brief Multiplication of a vector with a scalar.
 * \param data      : The vector to multiply.
 * \param  constant : The scalar to multiply with.
 * \return          : A vector where all values are scaled with the constant.
 */
std::vector<double> operator*(std::vector<double> const & data, double constant);


/*!\brief Division of a vector with a scalar.
 * \param data      : The vector to divide.
 * \param  constant : The scalar to divide by.
 * \return          : A vector where all values are scaled with inverse of the constant.
 */
std::vector<double> operator/(std::vector<double> const & data, double constant);


/*!\brief Elementwise multiplication of two vectors.
 * \param  data1 : The first vector.
 * \param  data2 : The second vector.
 * \return       : A vector where all values are the elementwise products of the input vectors.
 */
std::vector<double> operator*(std::vector<double> const & data1, std::vector<double> const & data2);


/*!\brief Elementwise addition of two vectors.
 * \param  data1 : The first vector.
 * \param  data2 : The second vector.
 * \return       : The elementwise sum vector.
 */
std::vector<double> operator+(std::vector<double> const & data1, std::vector<double> const & data2);


/*!\brief Elementwise subtraction of two vectors.
 * \param  data1 : The first vector.
 * \param  data2 : The second vector.
 * \return       : The elementwise difference vector.
 */
std::vector<double> operator-(std::vector<double> const & data1, std::vector<double> const & data2);


/*!\brief Elementwise subtraction of two vectors.
 * \param  data1 : The first vector.
 * \param  data2 : The second vector.
 * \return       : The elementwise difference vector.
 */
std::vector<int> operator-(std::vector<int> const & data1, std::vector<int> const & data2);


/*!\brief Elementwise addition of two vectors.
 * \param  data1 : The vector to wich the other vector is added.
 * \param  data2 : The vector to add.
 */
void vadd(std::vector<double> & data1, std::vector<double> const & data2);


/*!\brief Elementwise addition of two vectors.
 * \param  data1 : The vector to wich the other vector is added.
 * \param  data2 : The vector to add.
 */
void vadd(std::vector<int> & data1, std::vector<int> const & data2);


/*!\brief Elementwise subtraction of two vectors.
 * \param  data1 : The vector from wich the other vector is subtracted.
 * \param  data2 : The vector to subtract.
 */
void vsub(std::vector<double> & data1, std::vector<double> const & data2);


/*!\brief Elementwise subtraction of two vectors.
 * \param  data1 : The vector from wich the other vector is subtracted.
 * \param  data2 : The vector to subtract.
 */
void vsub(std::vector<int> & data1, std::vector<int> const & data2);


/*!\brief Elementwise square of a vector.
 * \param  data : The vector to square.
 */
void vsquare(std::vector<double> & data1);


/*!\brief Normalize a vector to the same norm as another vector. Vectors are
 *        in this function treated as histograms, and the 'norm' is to be
 *        understood as the area of the histogram, here the sum of the vector
 *        elements. This procedure will not work well for negative valued vectors.
 * \param  data1 : The vector to normalize.
 * \param  data2 : The vector that provides the norm to use.
 */
void vnormalize(std::vector<double> & data1, std::vector<double> const & data2);
