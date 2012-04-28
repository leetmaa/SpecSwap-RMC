/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/


#ifndef __TEST_MATHUTILS__
#define __TEST_MATHUTILS__

#include <iostream>
#include <string>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/TestRunner.h>

#include <cppunit/extensions/HelperMacros.h>

class Test_Mathutils : public CppUnit::TestCase {

public:

    CPPUNIT_TEST_SUITE( Test_Mathutils );
    CPPUNIT_TEST( testFastFloor );
    CPPUNIT_TEST( testCalculateDistance1 );
    CPPUNIT_TEST( testCalculateDistance2 );
    CPPUNIT_TEST( testSetupIndexMatrix );
    CPPUNIT_TEST( testSumVectorElementsInt );
    CPPUNIT_TEST( testSumVectorElementsDouble );
    CPPUNIT_TEST( testVectorScalarMultiplicationDoubleOperator );
    CPPUNIT_TEST( testVectorScalarDivisionDoubleOperator );
    CPPUNIT_TEST( testElementwiseVectorVectorMultiplicationDoubleOperator );
    CPPUNIT_TEST( testElementwiseVectorVectorAdditionDoubleOperator );
    CPPUNIT_TEST( testElementwiseVectorVectorSubtractionDoubleOperator );
    CPPUNIT_TEST( testElementwiseVectorVectorSubtractionIntOperator );
    CPPUNIT_TEST( testElementwiseVectorVectorDoubleAddition );
    CPPUNIT_TEST( testElementwiseVectorVectorIntAddition );
    CPPUNIT_TEST( testElementwiseVectorVectorDoubleSubtraction );
    CPPUNIT_TEST( testElementwiseVectorVectorIntSubtraction );
    CPPUNIT_TEST( testElementwiseVectorDoubleSquare );
    CPPUNIT_TEST( testVectorNormalization );
    CPPUNIT_TEST_SUITE_END();

    void testFastFloor();
    void testCalculateDistance1();
    void testCalculateDistance2();
    void testSetupIndexMatrix();
    void testSumVectorElementsInt();
    void testSumVectorElementsDouble();
    void testVectorScalarMultiplicationDoubleOperator();
    void testVectorScalarDivisionDoubleOperator();
    void testElementwiseVectorVectorMultiplicationDoubleOperator();
    void testElementwiseVectorVectorAdditionDoubleOperator();
    void testElementwiseVectorVectorSubtractionDoubleOperator();
    void testElementwiseVectorVectorSubtractionIntOperator();
    void testElementwiseVectorVectorDoubleAddition();
    void testElementwiseVectorVectorIntAddition();
    void testElementwiseVectorVectorDoubleSubtraction();
    void testElementwiseVectorVectorIntSubtraction();
    void testElementwiseVectorDoubleSquare();
    void testVectorNormalization();

};

#endif

