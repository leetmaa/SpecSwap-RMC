/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/


#ifndef __TEST_MLRND__
#define __TEST_MLRND__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_Mlrnd : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_Mlrnd );
    CPPUNIT_TEST( testConstruction );
    CPPUNIT_TEST( testSetSeed );
    CPPUNIT_TEST( testRandom );
    CPPUNIT_TEST_SUITE_END();

    void testConstruction();
    void testSetSeed();
    void testRandom();

};

#endif

