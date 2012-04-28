/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/


#ifndef __TEST_SPECSWAP__
#define __TEST_SPECSWAP__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_Specswap : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_Specswap );
    CPPUNIT_TEST( testConstruction );
    CPPUNIT_TEST( testAddScalarMean );
    CPPUNIT_TEST( testAddScalarValue );
    CPPUNIT_TEST( testAddScalarDistribution );
    CPPUNIT_TEST( testAddCurve );
    CPPUNIT_TEST( testAddPCF );
    CPPUNIT_TEST( testSetup );
    CPPUNIT_TEST( testMove );
    CPPUNIT_TEST( testGetChi2 );
    CPPUNIT_TEST( testNotify );
    CPPUNIT_TEST( testAccept );
    CPPUNIT_TEST( testCollectWeights );
    CPPUNIT_TEST( testCalcScalarPos );
    CPPUNIT_TEST( testRandom );
    CPPUNIT_TEST( testRandomBasis );
    CPPUNIT_TEST( testPrepareForAnalysis );
    CPPUNIT_TEST_SUITE_END();

    void testConstruction();
    void testAddScalarMean();
    void testAddScalarValue();
    void testAddScalarDistribution();
    void testAddCurve();
    void testAddPCF();
    void testSetup();
    void testMove();
    void testGetChi2();
    void testNotify();
    void testAccept();
    void testCollectWeights();
    void testCalcScalarPos();
    void testRandom();
    void testRandomBasis();
    void testPrepareForAnalysis();
};

#endif


