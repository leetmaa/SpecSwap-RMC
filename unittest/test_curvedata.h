/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>

*/


#ifndef __TEST_CURVEDATA__
#define __TEST_CURVEDATA__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_CurveData : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_CurveData );
    CPPUNIT_TEST( testConstruction );
    CPPUNIT_TEST( testPosition );
    CPPUNIT_TEST( testConstructionFail1 );
    CPPUNIT_TEST( testConstructionFail2 );
    CPPUNIT_TEST( testConstructionFail3 );
    CPPUNIT_TEST( testConstructionFail4 );
    CPPUNIT_TEST( testInitNotifyAccept );
    CPPUNIT_TEST( testGetChi2 );
    CPPUNIT_TEST( testGetChi2New );
    CPPUNIT_TEST( testCalculateChi2 );
    CPPUNIT_TEST_SUITE_END();

    void testConstruction();
    void testPosition();
    void testConstructionFail1();
    void testConstructionFail2();
    void testConstructionFail3();
    void testConstructionFail4();
    void testInitNotifyAccept();
    void testGetChi2();
    void testGetChi2New();
    void testCalculateChi2();

};

#endif

