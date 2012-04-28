/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/


#ifndef __TEST_IOUTILS__
#define __TEST_IOUTILS__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_IOUtils : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_IOUtils );
    CPPUNIT_TEST( testToString );
    CPPUNIT_TEST( testNewlineIndent );
    CPPUNIT_TEST( testEOF );
    CPPUNIT_TEST( testCheckPath );
    CPPUNIT_TEST( testCheckSigma );
    CPPUNIT_TEST( testCheckPositiveInteger );
    CPPUNIT_TEST( testOpenFileError );
    CPPUNIT_TEST( testEmptyFileError );
    CPPUNIT_TEST( testUnknownSectionError );
    CPPUNIT_TEST( testSameSectionError );
    CPPUNIT_TEST( testMissingKeywordError );
    CPPUNIT_TEST( testSameKeywordError );
    CPPUNIT_TEST( testUnknownKeywordError );
    CPPUNIT_TEST( testReadKeywordError );
    CPPUNIT_TEST( testTimer );
    CPPUNIT_TEST( testError );
    CPPUNIT_TEST( testEatline );
    CPPUNIT_TEST_SUITE_END();

    void testToString();
    void testNewlineIndent();
    void testEOF();
    void testCheckPath();
    void testCheckSigma();
    void testCheckPositiveInteger();
    void testOpenFileError();
    void testEmptyFileError();
    void testUnknownSectionError();
    void testSameSectionError();
    void testMissingKeywordError();
    void testSameKeywordError();
    void testUnknownKeywordError();
    void testReadKeywordError();
    void testTimer();
    void testError();
    void testEatline();

};

#endif

