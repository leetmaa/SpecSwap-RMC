/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/


#ifndef __TEST_LIBRARY__
#define __TEST_LIBRARY__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_Library : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_Library );
    CPPUNIT_TEST( testConstruction );
    CPPUNIT_TEST( testConstructionFail );
    CPPUNIT_TEST( testName );
    CPPUNIT_TEST( testNcurves );
    CPPUNIT_TEST( testNbase );
    CPPUNIT_TEST( testNscalars );
    CPPUNIT_TEST( testAtomsInfo );
    CPPUNIT_TEST( testConvolute );
    CPPUNIT_TEST( testFormat );
    CPPUNIT_TEST( testScale );
    CPPUNIT_TEST( testEnding );
    CPPUNIT_TEST( testScalarName );
    CPPUNIT_TEST( testAtomTypes );
    CPPUNIT_TEST( testAddBase );
    CPPUNIT_TEST( testCompleteSetup );
    CPPUNIT_TEST( testGetBasisName );
    CPPUNIT_TEST( testGetPartial );
    CPPUNIT_TEST( testGetInternal );
    CPPUNIT_TEST( testGetCentral );
    CPPUNIT_TEST( testGetGeo );
    CPPUNIT_TEST( testGetScalarAt );
    CPPUNIT_TEST_SUITE_END();

    void testConstruction();
    void testConstructionFail();
    void testName();
    void testNcurves();
    void testNscalars();
    void testNbase();
    void testAtomsInfo();
    void testConvolute();
    void testFormat();
    void testScale();
    void testEnding();
    void testScalarName();
    void testAtomTypes();
    void testAddBase();
    void testCompleteSetup();
    void testGetBasisName();
    void testGetPartial();
    void testGetInternal();
    void testGetCentral();
    void testGetScale();
    void testGetGeo();
    void testGetScalarAt();

};

#endif

