/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/


// Include the test definition.
#include "test_sampleset.h"

// Include the files to test.
#include "../src/sampleset.h"

// Other inclusion.
#include "../src/ioutils.h"
#include "../src/ioexception.h"
#include "../src/library.h"


// -------------------------------------------------------------------------- //
//
void Test_Sampleset::testConstruction()
{
    // Check default construction works.
    CPPUNIT_ASSERT_NO_THROW(Sampleset ss);

    // Check construction and member data.
    Sampleset ss(12);
    CPPUNIT_ASSERT_EQUAL(ss.nsample_, 12);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(ss.indices_.size()), 0);
}


// -------------------------------------------------------------------------- //
//
void Test_Sampleset::testAddIndex()
{
    // Test that it fails correctly.
    {
        Sampleset ss(0);
        CPPUNIT_ASSERT_THROW(ss.add_index(1), IOException );
    }

    // Test that it works correctly.
    Sampleset ss(2);
    ss.add_index(334);
    ss.add_index(9);
    CPPUNIT_ASSERT_EQUAL(ss.indices_[0], 334);
    CPPUNIT_ASSERT_EQUAL(ss.indices_[1], 9);

}


// -------------------------------------------------------------------------- //
//
void Test_Sampleset::testSwapIntoSlot()
{
    // Setup.
    Sampleset ss(3);
    ss.add_index(334);
    ss.add_index(9);
    ss.add_index(12);
    // Swap.
    ss.swap_into_slot(14, 1);
    // Check.
    CPPUNIT_ASSERT_EQUAL(ss.indices_[1], 14);
}


// -------------------------------------------------------------------------- //
//
void Test_Sampleset::testIndexAt()
{
    // Setup.
    Sampleset ss(3);
    ss.add_index(334);
    ss.add_index(9);
    ss.add_index(12);
    // Check.
    CPPUNIT_ASSERT_EQUAL(ss.index_at(0), ss.indices_[0]);
    CPPUNIT_ASSERT_EQUAL(ss.index_at(1), ss.indices_[1]);
    CPPUNIT_ASSERT_EQUAL(ss.index_at(2), ss.indices_[2]);
}


// -------------------------------------------------------------------------- //
//
void Test_Sampleset::testGetIndices()
{
    // Setup.
    Sampleset ss(3);
    ss.add_index(334);
    ss.add_index(9);
    ss.add_index(12);

    // Get the non-const reference.
    const std::vector<int> & indices = ss.get_indices();
    CPPUNIT_ASSERT_EQUAL(indices[0], ss.indices_[0]);
    CPPUNIT_ASSERT_EQUAL(indices[1], ss.indices_[1]);
    CPPUNIT_ASSERT_EQUAL(indices[2], ss.indices_[2]);
}


// -------------------------------------------------------------------------- //
//
void Test_Sampleset::testIsAdded()
{
    // Setup.
    Sampleset ss(3);
    ss.add_index(334);
    ss.add_index(9);
    ss.add_index(12);

    // Check.
    CPPUNIT_ASSERT( ss.is_added(334) );
    CPPUNIT_ASSERT( ss.is_added(12) );
    CPPUNIT_ASSERT( ss.is_added(9) );

    CPPUNIT_ASSERT( !ss.is_added(123) );
    CPPUNIT_ASSERT( !ss.is_added(166) );
    CPPUNIT_ASSERT( !ss.is_added(0) );

}

