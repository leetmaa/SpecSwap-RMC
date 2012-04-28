/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/


#ifndef __TEST_MKLIB__
#define __TEST_MKLIB__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_Mklib : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_Mklib );
    CPPUNIT_TEST( testConstruction );
    CPPUNIT_TEST( testRead );
    CPPUNIT_TEST( testReadFail1 );
    CPPUNIT_TEST( testReadFail2 );
    CPPUNIT_TEST( testReadFail3 );
    CPPUNIT_TEST( testReadFail4 );
    CPPUNIT_TEST( testReadFail5 );
    CPPUNIT_TEST( testReadFail6 );
    CPPUNIT_TEST( testReadFail7 );
    CPPUNIT_TEST( testReadFail8 );
    CPPUNIT_TEST( testReadFail9 );
    CPPUNIT_TEST( testReadFail10 );
    CPPUNIT_TEST( testReadFail11 );
    CPPUNIT_TEST( testReadFail12 );
    CPPUNIT_TEST( testReadFail13 );
    CPPUNIT_TEST( testReadFail14 );
    CPPUNIT_TEST( testReadFail15 );
    CPPUNIT_TEST( testReadFail16 );
    CPPUNIT_TEST_SUITE_END();

    void testConstruction();
    void testRead();
    void testReadFail1();
    void testReadFail2();
    void testReadFail3();
    void testReadFail4();
    void testReadFail5();
    void testReadFail6();
    void testReadFail7();
    void testReadFail8();
    void testReadFail9();
    void testReadFail10();
    void testReadFail11();
    void testReadFail12();
    void testReadFail13();
    void testReadFail14();
    void testReadFail15();
    void testReadFail16();

};

#endif

