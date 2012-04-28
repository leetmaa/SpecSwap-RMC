/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/



// Include the test definition.
#include "test_mlrnd.h"

// Include the files to test.
#include "../src/mlrnd.h"

// Other inclusion.
#include "../src/mathutils.h"
#include <cmath>

// -------------------------------------------------------------------------- //
//
void Test_Mlrnd::testConstruction()
{
    CPPUNIT_ASSERT_NO_THROW( Mlrnd mlrnd );

    Mlrnd mlrnd;
    CPPUNIT_ASSERT_EQUAL(mlrnd.seed_, 13);
}


// -------------------------------------------------------------------------- //
//
void Test_Mlrnd::testSetSeed()
{
    Mlrnd mlrnd;
    mlrnd.set_seed(12);
    CPPUNIT_ASSERT_EQUAL(mlrnd.seed_, 12);
}


// -------------------------------------------------------------------------- //
//
void Test_Mlrnd::testRandom()
{
    Mlrnd mlrnd;
    mlrnd.set_seed(193);

    // Check the sperad and boundaries.
    std::vector<double> values(1000000);
    for (size_t i = 0; i < values.size(); ++i)
    {
        const double rnd = mlrnd.random();
        CPPUNIT_ASSERT(rnd > 0.0);
        CPPUNIT_ASSERT(rnd < 1.0);
        values[i] = rnd;
    }

    // Calculate the mean value.
    const double mean = vsum(values)/values.size();

    // Calculate the spread.
    double sum = 0.0;
    for (size_t i = 0; i < values.size(); ++i)
    {
        sum += std::sqrt((mean - values[i])*(mean - values[i]));
    }
    const double spread = sum/values.size();

    // Check that it is close to 0.25.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(spread, 0.250, 1.0e-3);
}




