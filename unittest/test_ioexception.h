/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/


#ifndef __TEST_IOEXCEPTION__
#define __TEST_IOEXCEPTION__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_IOException : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_IOException );
    CPPUNIT_TEST( testConstructionAndFunctionality );
    CPPUNIT_TEST_SUITE_END();

    void testConstructionAndFunctionality();

};

#endif

