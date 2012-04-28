/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/


#ifndef __TEST_SCALARDISTRIBUTIONDATA__
#define __TEST_SCALARDISTRIBUTIONDATA__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_ScalarDistributionData : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_ScalarDistributionData );
    CPPUNIT_TEST( testConstruction );
    CPPUNIT_TEST( testConstructionFail );
    CPPUNIT_TEST( testConstructionFail2 );
    CPPUNIT_TEST( testInitNotifyAccept );
    CPPUNIT_TEST( testGetChi2 );
    CPPUNIT_TEST( testGetChi2New );
    CPPUNIT_TEST( testCalculateChi2 );
    CPPUNIT_TEST( testGetBin );
    CPPUNIT_TEST_SUITE_END();

    void testConstruction();
    void testConstructionFail();
    void testConstructionFail2();
    void testInitNotifyAccept();
    void testGetChi2();
    void testGetChi2New();
    void testCalculateChi2();
    void testGetBin();

};

#endif

