/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/


// Include the test definition.
#include "test_ioexception.h"

// Include the files to test.
#include "../src/ioexception.h"

// Other inclusion.

// -------------------------------------------------------------------------- //
//
void Test_IOException::testConstructionAndFunctionality()
{
    const std::string message("Here is an error.");
    const std::string location("This is where it came from.");
    // Check that it can be constructed.
    CPPUNIT_ASSERT_NO_THROW(IOException(message, location));

    // Construct.
    const IOException e(message, location);

    // Check that the return values are the same as input.
    CPPUNIT_ASSERT_EQUAL( message, e.message() );
    CPPUNIT_ASSERT_EQUAL( location, e.location() );

    // Check what the what() function says.
    const std::string what(e.what());
    const std::string what_ref("\n   IOException: This is where it came from.\n       Message: Here is an error.\n");

    CPPUNIT_ASSERT_EQUAL( what, what_ref );

    // Check that we can call print.
    e.print();

    // Check that we can throw the error and catch it as a std::runtime_error.
    CPPUNIT_ASSERT_THROW( throw e, std::runtime_error );

}

