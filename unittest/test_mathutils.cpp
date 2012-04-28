/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/


// Include the test definition.
#include "test_mathutils.h"

// Include the files to test.
#include "../src/mathutils.h"

// Other inclusion.
#include "../src/matrix.h"
#include <cmath>

#define EPS 1.0e-12

// -------------------------------------------------------------------------- //
//
void Test_Mathutils::testFastFloor()
{
    double value = -9998823.1;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( std::floor(value),
                                  fastfloor(value), EPS);
    value = -23.1;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( std::floor(value),
                                  fastfloor(value), EPS);
    value = -2.99999;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( std::floor(value),
                                  fastfloor(value), EPS);
    value = -1.876543;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( std::floor(value),
                                  fastfloor(value), EPS);
    value = -0.00000000000000003;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( std::floor(value),
                                  fastfloor(value), EPS);
    value = 0.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( std::floor(value),
                                  fastfloor(value), EPS);
    value = 0.1345;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( std::floor(value),
                                  fastfloor(value), EPS);
    value = 2345.76543;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( std::floor(value),
                                  fastfloor(value), EPS);
    value = 3445676.66543;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( std::floor(value),
                                  fastfloor(value), EPS);
}


// -------------------------------------------------------------------------- //
//
void Test_Mathutils::testCalculateDistance1()
{
    const double point1[3] = {0.0, 0.0, 0.0};
    const double point2[3] = {1.0, 1.0, 1.0};
    const double distance = calculate_distance(point1, point2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(distance, std::sqrt(3.0), EPS);
}


// -------------------------------------------------------------------------- //
//
void Test_Mathutils::testCalculateDistance2()
{
    const double point1[3] = {1.0, 2.0, 3.0};
    const double point2[3] = {-3.0, -2.0, -1.0};
    const double distance = calculate_distance(point1, point2);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(distance, std::sqrt(3.0*16.0), EPS);
}


// -------------------------------------------------------------------------- //
//
void Test_Mathutils::testSetupIndexMatrix()
{
    const Matrix m0 = setup_index_matrix(2);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(m0(0,0)), 0);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(m0(0,1)), 1);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(m0(1,0)), 1);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(m0(1,1)), 2);

    const Matrix m1 = setup_index_matrix(3);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(m1(0,0)), 0);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(m1(0,1)), 1);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(m1(0,2)), 2);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(m1(1,0)), 1);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(m1(1,1)), 3);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(m1(1,2)), 4);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(m1(2,0)), 2);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(m1(2,1)), 4);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(m1(2,2)), 5);
}


// -------------------------------------------------------------------------- //
//
void Test_Mathutils::testSumVectorElementsInt()
{
    std::vector<int> vec(4);
    vec[0] = 4;
    vec[1] = 2;
    vec[2] = 3;
    vec[3] = -7;
    const int sum = vsum(vec);
    CPPUNIT_ASSERT_EQUAL(sum, 2);

}


// -------------------------------------------------------------------------- //
//
void Test_Mathutils::testSumVectorElementsDouble()
{
    std::vector<double> vec(4);
    vec[0] = 1.1;
    vec[1] = 2.2;
    vec[2] = 3.3;
    vec[3] = -7.7;
    const double sum = vsum(vec);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(sum, -1.1, EPS);

}


// -------------------------------------------------------------------------- //
//
void Test_Mathutils::testVectorScalarMultiplicationDoubleOperator()
{
    std::vector<double> vec(4);
    vec[0] = 1.1;
    vec[1] = 2.2;
    vec[2] = 3.3;
    vec[3] = -7.7;
    const double scalar0 = 2.5;
    // Call.
    std::vector<double> v1 = vec*scalar0;
    // Check.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(v1.size()), static_cast<int>(vec.size()));
    CPPUNIT_ASSERT_DOUBLES_EQUAL( v1[0], vec[0]*2.5, EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( v1[1], vec[1]*2.5, EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( v1[2], vec[2]*2.5, EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( v1[3], vec[3]*2.5, EPS );
}


// -------------------------------------------------------------------------- //
//
void Test_Mathutils::testVectorScalarDivisionDoubleOperator()
{
    std::vector<double> vec(4);
    vec[0] = 1.1;
    vec[1] = 2.2;
    vec[2] = 3.3;
    vec[3] = -7.7;
    const double scalar0 = std::sqrt(3);
    // Call.
    std::vector<double> v1 = vec/scalar0;
    // Check.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(v1.size()), static_cast<int>(vec.size()));
    CPPUNIT_ASSERT_DOUBLES_EQUAL( v1[0], vec[0]/scalar0, EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( v1[1], vec[1]/scalar0, EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( v1[2], vec[2]/scalar0, EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( v1[3], vec[3]/scalar0, EPS );
}


// -------------------------------------------------------------------------- //
//
void Test_Mathutils::testElementwiseVectorVectorMultiplicationDoubleOperator()
{
    // Setup.
    std::vector<double> vec0(4);
    vec0[0] =  1.1;
    vec0[1] =  2.2;
    vec0[2] =  3.3;
    vec0[3] = -7.7;

    std::vector<double> vec1(4);
    vec1[0] =   2.1;
    vec1[1] =   3.2;
    vec1[2] =  -4.3;
    vec1[3] = -11.7;

    // Call.
    std::vector<double> v1 = vec0*vec1;

    // Check.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(v1.size()), static_cast<int>(vec0.size()));
    CPPUNIT_ASSERT_DOUBLES_EQUAL( v1[0], vec0[0]*vec1[0], EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( v1[1], vec0[1]*vec1[1], EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( v1[2], vec0[2]*vec1[2], EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( v1[3], vec0[3]*vec1[3], EPS );
}


// -------------------------------------------------------------------------- //
//
void Test_Mathutils::testElementwiseVectorVectorAdditionDoubleOperator()
{
    // Setup.
    std::vector<double> vec0(4);
    vec0[0] =  1.1;
    vec0[1] =  2.2;
    vec0[2] =  3.3;
    vec0[3] = -7.7;

    std::vector<double> vec1(4);
    vec1[0] =   2.1;
    vec1[1] =   3.2;
    vec1[2] =  -4.3;
    vec1[3] = -11.7;

    // Call.
    std::vector<double> v1 = vec0+vec1;

    // Check.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(v1.size()), static_cast<int>(vec0.size()));
    CPPUNIT_ASSERT_DOUBLES_EQUAL( v1[0], vec0[0]+vec1[0], EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( v1[1], vec0[1]+vec1[1], EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( v1[2], vec0[2]+vec1[2], EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( v1[3], vec0[3]+vec1[3], EPS );
}


// -------------------------------------------------------------------------- //
//
void Test_Mathutils::testElementwiseVectorVectorSubtractionDoubleOperator()
{
    // Setup.
    std::vector<double> vec0(4);
    vec0[0] =  1.1;
    vec0[1] =  2.2;
    vec0[2] =  3.3;
    vec0[3] = -7.7;

    std::vector<double> vec1(4);
    vec1[0] =   2.1;
    vec1[1] =   3.2;
    vec1[2] =  -4.3;
    vec1[3] = -11.7;

    // Call.
    std::vector<double> v1 = vec0-vec1;

    // Check.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(v1.size()), static_cast<int>(vec0.size()));
    CPPUNIT_ASSERT_DOUBLES_EQUAL( v1[0], vec0[0]-vec1[0], EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( v1[1], vec0[1]-vec1[1], EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( v1[2], vec0[2]-vec1[2], EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( v1[3], vec0[3]-vec1[3], EPS );
}


// -------------------------------------------------------------------------- //
//
void Test_Mathutils::testElementwiseVectorVectorSubtractionIntOperator()
{
    // Setup.
    std::vector<int> vec0(4);
    vec0[0] =  1;
    vec0[1] =  2;
    vec0[2] =  3;
    vec0[3] = -7;

    std::vector<int> vec1(4);
    vec1[0] =   2;
    vec1[1] =   3;
    vec1[2] =  -4;
    vec1[3] = -11;

    // Call.
    std::vector<int> v1 = vec0-vec1;

    // Check.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(v1.size()), static_cast<int>(vec0.size()));
    CPPUNIT_ASSERT_EQUAL( v1[0], vec0[0]-vec1[0] );
    CPPUNIT_ASSERT_EQUAL( v1[1], vec0[1]-vec1[1] );
    CPPUNIT_ASSERT_EQUAL( v1[2], vec0[2]-vec1[2] );
    CPPUNIT_ASSERT_EQUAL( v1[3], vec0[3]-vec1[3] );
}


// -------------------------------------------------------------------------- //
//
void Test_Mathutils::testElementwiseVectorVectorDoubleAddition()
{
    // Setup.
    std::vector<double> vec0(4);
    vec0[0] =  1.1;
    vec0[1] =  2.2;
    vec0[2] =  3.3;
    vec0[3] = -7.7;

    std::vector<double> vec1(4);
    vec1[0] =   2.1;
    vec1[1] =   3.2;
    vec1[2] =  -4.3;
    vec1[3] = -11.7;

    // Take a reference.
    const std::vector<double> ref = vec0;
    const std::vector<double> ref1 = vec1;

    // Call.
    vadd(vec0, ref1);

    // Check.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(vec0.size()), static_cast<int>(ref.size()));
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vec0[0], ref[0]+vec1[0], EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vec0[1], ref[1]+vec1[1], EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vec0[2], ref[2]+vec1[2], EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vec0[3], ref[3]+vec1[3], EPS );
}


// -------------------------------------------------------------------------- //
//
void Test_Mathutils::testElementwiseVectorVectorIntAddition()
{
    // Setup.
    std::vector<int> vec0(4);
    vec0[0] =  1;
    vec0[1] =  2;
    vec0[2] =  3;
    vec0[3] = -7;

    std::vector<int> vec1(4);
    vec1[0] =   2;
    vec1[1] =   3;
    vec1[2] =  -4;
    vec1[3] = -11;

    // Take a reference.
    const std::vector<int> ref = vec0;
    const std::vector<int> ref1 = vec1;

    // Call.
    vadd(vec0, ref1);

    // Check.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(vec0.size()), static_cast<int>(ref.size()));
    CPPUNIT_ASSERT_EQUAL( vec0[0], ref[0]+vec1[0] );
    CPPUNIT_ASSERT_EQUAL( vec0[1], ref[1]+vec1[1] );
    CPPUNIT_ASSERT_EQUAL( vec0[2], ref[2]+vec1[2] );
    CPPUNIT_ASSERT_EQUAL( vec0[3], ref[3]+vec1[3] );
}


// -------------------------------------------------------------------------- //
//
void Test_Mathutils::testElementwiseVectorVectorDoubleSubtraction()
{
    // Setup.
    std::vector<double> vec0(4);
    vec0[0] =  1.1;
    vec0[1] =  2.2;
    vec0[2] =  3.3;
    vec0[3] = -7.7;

    std::vector<double> vec1(4);
    vec1[0] =   2.1;
    vec1[1] =   3.2;
    vec1[2] =  -4.3;
    vec1[3] = -11.7;

    // Take a reference.
    const std::vector<double> ref = vec0;
    const std::vector<double> ref1 = vec1;

    // Call.
    vsub(vec0, ref1);

    // Check.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(vec0.size()), static_cast<int>(ref.size()));
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vec0[0], ref[0]-vec1[0], EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vec0[1], ref[1]-vec1[1], EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vec0[2], ref[2]-vec1[2], EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vec0[3], ref[3]-vec1[3], EPS );
}


// -------------------------------------------------------------------------- //
//
void Test_Mathutils::testElementwiseVectorVectorIntSubtraction()
{
    // Setup.
    std::vector<int> vec0(4);
    vec0[0] =  1;
    vec0[1] =  2;
    vec0[2] =  3;
    vec0[3] = -7;

    std::vector<int> vec1(4);
    vec1[0] =   2;
    vec1[1] =   3;
    vec1[2] =  -4;
    vec1[3] = -11;

    // Take a reference.
    const std::vector<int> ref = vec0;
    const std::vector<int> ref1 = vec1;

    // Call.
    vsub(vec0, ref1);

    // Check.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(vec0.size()), static_cast<int>(ref.size()));
    CPPUNIT_ASSERT_EQUAL( vec0[0], ref[0]-vec1[0] );
    CPPUNIT_ASSERT_EQUAL( vec0[1], ref[1]-vec1[1] );
    CPPUNIT_ASSERT_EQUAL( vec0[2], ref[2]-vec1[2] );
    CPPUNIT_ASSERT_EQUAL( vec0[3], ref[3]-vec1[3] );
}


// -------------------------------------------------------------------------- //
//
void Test_Mathutils::testElementwiseVectorDoubleSquare()
{
    // Setup.
    std::vector<double> vec0(4);
    vec0[0] =  1.1;
    vec0[1] =  2.2;
    vec0[2] =  3.3;
    vec0[3] = -7.7;

    // Take a reference.
    const std::vector<double> ref = vec0;

    // Call.
    vsquare(vec0);

    // Check.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(vec0.size()), static_cast<int>(ref.size()));
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vec0[0], ref[0]*ref[0], EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vec0[1], ref[1]*ref[1], EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vec0[2], ref[2]*ref[2], EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vec0[3], ref[3]*ref[3], EPS );

}


// -------------------------------------------------------------------------- //
//
void Test_Mathutils::testVectorNormalization()
{
    // Setup.
    std::vector<double> vec0(4);
    vec0[0] =  1.1;
    vec0[1] =  2.2;
    vec0[2] =  3.3;
    vec0[3] =  7.7;

    const double norm0 = vec0[0] + vec0[1] + vec0[2] + vec0[3];

    std::vector<double> vec1(4);
    vec1[0] =   2.1;
    vec1[1] =   3.2;
    vec1[2] =   4.3;
    vec1[3] =  11.7;

    const double norm1 = vec1[0] + vec1[1] + vec1[2] + vec1[3];

    // Take a reference.
    const std::vector<double> ref = vec0;
    const std::vector<double> ref1 = vec1;

    // Call.
    vnormalize(vec0, ref1);

    // Check.
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(vec0.size()), static_cast<int>(ref.size()));
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vec0[0], ref[0]*norm1/norm0, EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vec0[1], ref[1]*norm1/norm0, EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vec0[2], ref[2]*norm1/norm0, EPS );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vec0[3], ref[3]*norm1/norm0, EPS );
}
