/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/


// Include the test definition.
#include "test_matrix.h"

// Include the files to test.
#include "../src/matrix.h"

// Other inclusion.


// -------------------------------------------------------------------------- //
//
void Test_Matrix::testConstruction()
{
    CPPUNIT_ASSERT_NO_THROW(Matrix m);

    // Default construction.
    Matrix m0;
    CPPUNIT_ASSERT_EQUAL(m0.rows_, 0);
    CPPUNIT_ASSERT_EQUAL(m0.columns_, 0);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(m0.data_.size()), 0);

    /// Construction with sizes.
    Matrix m1(12,21);
    CPPUNIT_ASSERT_EQUAL(m1.rows_, 12);
    CPPUNIT_ASSERT_EQUAL(m1.columns_, 21);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(m1.data_.size()), 12);
    // Check dimensions.
    for (size_t i = 0; i < m1.data_.size(); ++i)
    {
        CPPUNIT_ASSERT_EQUAL(static_cast<int>(m1.data_[i].size()), 21);
    }

    // Check the values.
    for (size_t i = 0; i < m1.data_.size(); ++i)
    {
        for (size_t j = 0; j < m1.data_[i].size(); ++j)
        {
            CPPUNIT_ASSERT_DOUBLES_EQUAL(m1.data_[i][j], 0.0, 1.0e-12);
        }
    }
}


// -------------------------------------------------------------------------- //
//
void Test_Matrix::testResize()
{
    // Setup with size.
    Matrix m1(12,21);
    // Check dimensions.
    CPPUNIT_ASSERT_EQUAL(m1.rows_, 12);
    CPPUNIT_ASSERT_EQUAL(m1.columns_, 21);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(m1.data_.size()), 12);
    for (size_t i = 0; i < m1.data_.size(); ++i)
    {
        CPPUNIT_ASSERT_EQUAL(static_cast<int>(m1.data_[i].size()), 21);
    }

    // Set values != 0.0
    for (size_t i = 0; i < m1.data_.size(); ++i)
    {
        for (size_t j = 0; j < m1.data_[i].size(); ++j)
        {
            m1.data_[i][j] = 1.234*i*j;
        }
    }

    // Resize.
    m1.resize(13,31);
    // Check dimensions.
    CPPUNIT_ASSERT_EQUAL(m1.rows_, 13);
    CPPUNIT_ASSERT_EQUAL(m1.columns_, 31);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(m1.data_.size()), 13);
    for (size_t i = 0; i < m1.data_.size(); ++i)
    {
        CPPUNIT_ASSERT_EQUAL(static_cast<int>(m1.data_[i].size()), 31);
    }

    // Check that the values are set to zero.
    for (size_t i = 0; i < m1.data_.size(); ++i)
    {
        for (size_t j = 0; j < m1.data_[i].size(); ++j)
        {
            CPPUNIT_ASSERT_DOUBLES_EQUAL(m1.data_[i][j], 0.0, 1.0e-12);
        }
    }
}


// -------------------------------------------------------------------------- //
//
void Test_Matrix::testAccess()
{
    // Setup with size.
    Matrix m1(1,2);

    // Make sure the data is zero from start.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(m1.data_[0][0], 0.0, 1.0e-12);

    // Set the value using the non const access.
    m1(0,0) = 1.3;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(m1.data_[0][0], 1.3, 1.0e-12);

    CPPUNIT_ASSERT_DOUBLES_EQUAL(m1(0,0), m1.data_[0][0], 1.0e-12);

    const Matrix m2(1,2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(m2(0,0), 0.0, 1.0e-12);

}
