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


/*! \file  matrix.h
 *  \brief File for the Matrix class definition.
 */

#ifndef __MATRIX__
#define __MATRIX__

#include <vector>


/// Class for representing a 2D double matrix.
class Matrix {

public:

    /*!\brief Default constructor.
     */
    Matrix();

    /*!\brief Constructor.
     * \param rows : The number of rows in the matrix.
     * \param columns : The number of columns in the matrix.
     */
    Matrix(int rows, int columns);

    /*!\brief Function for resizing the matrix.
     * \param rows : The number of rows in the matrix.
     * \param columns : The number of columns in the matrix.
     */
    void resize(int rows, int columns);

    /*!\brief Access operator.
     * \param row : The row to access.
     * \param column : The column to access.
     */
    double & operator()(int row, int column) { return data_[row][column]; }

    /*!\brief Access operator (const version).
     * \param row : The row to access.
     * \param column : The column to access.
     */
    double operator()(int row, int column) const { return data_[row][column]; }

protected:

private:

    /// The number of rows.
    int rows_;
    /// The number of columns.
    int columns_;
    /// The matrix data.
    std::vector< std::vector<double> > data_;

    /// Declaring the test class as friend to facilitate testing.
    friend class Test_Matrix;

};


#endif
