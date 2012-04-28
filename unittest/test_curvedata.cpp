/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/


// Include the test definition.
#include "test_curvedata.h"

// Include the files to test.
#include "../src/curvedata.h"

// Other inclusion.
#include "../src/ioexception.h"
#include "../src/library.h"

// -------------------------------------------------------------------------- //
//
void Test_CurveData::testConstruction()
{
    // Test default construction.
    CPPUNIT_ASSERT_NO_THROW( CurveData cd );

    // Read a library.
    Library library("./testfiles/testlibSmall");

    // Setup input.
    const double sigma = 0.31;
    const bool arearenorm = true;
    const std::string curvename("bas");
    const std::string reference_path("./testfiles/bas_ref.data");
    CurveData cd(sigma, arearenorm, curvename, reference_path, library);

    // Check that the correct member data was set.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sigma, cd.sigma_, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/(sigma*sigma), cd.one_over_sigma2_, 1.0e-12 );
    CPPUNIT_ASSERT( cd.area_renorm_ );
    CPPUNIT_ASSERT_EQUAL( curvename ,cd.curve_name_ );

    CPPUNIT_ASSERT_EQUAL( 0, cd.pos_ );

    // Check the vectors.
    const int size = static_cast<int>(cd.scale_.size());
    CPPUNIT_ASSERT_EQUAL( 108, size );

    // Dimensions.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(cd.curve_.size()), size );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(cd.curve_reference_.size()), size );

    // Check values.
    for ( size_t i = 0; i < size; ++i )
    {
        CPPUNIT_ASSERT_DOUBLES_EQUAL( cd.scale_[i], library.get_scale(cd.pos_)[i], 1.0e-12 );
        CPPUNIT_ASSERT_DOUBLES_EQUAL( cd.curve_[i], 0.0, 1.0e-12 );
    }

    CPPUNIT_ASSERT_DOUBLES_EQUAL( cd.curve_reference_[13],  0.116650600, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( cd.curve_reference_[67],  0.231314650, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( cd.curve_reference_[85], -0.022099955, 1.0e-12 );

}


// -------------------------------------------------------------------------- //
//
void Test_CurveData::testPosition()
{
    // Test with a library with more than one curve that the
    // right one gets selected for fitting.

    // NEEDS IMPLEMENTATION

}


// -------------------------------------------------------------------------- //
//
void Test_CurveData::testConstructionFail1()
{
    // Fail with wrong curve name.

    // Test default construction.
    CPPUNIT_ASSERT_NO_THROW( CurveData cd );

    // Read a library.
    Library library("./testfiles/testlibSmall");

    // Setup input.
    const double sigma = 0.31;
    const bool arearenorm = true;
    const std::string curvename("basss");
    const std::string reference_path("./testfiles/bas_ref.data");
    CPPUNIT_ASSERT_THROW(CurveData cd(sigma, arearenorm, curvename, reference_path, library), IOException );
}


// -------------------------------------------------------------------------- //
//
void Test_CurveData::testConstructionFail2()
{
    // Fail with wrong reference path.

    // Test default construction.
    CPPUNIT_ASSERT_NO_THROW( CurveData cd );

    // Read a library.
    Library library("./testfiles/testlibSmall");

    // Setup input.
    const double sigma = 0.31;
    const bool arearenorm = true;
    const std::string curvename("bas");
    const std::string reference_path("./testfiles/NOTAFILENAME");
    CPPUNIT_ASSERT_THROW( CurveData cd(sigma, arearenorm, curvename, reference_path, library), IOException );
}


// -------------------------------------------------------------------------- //
//
void Test_CurveData::testConstructionFail3()
{
    // Fail with errors in the reference file (wrong format).

    // Test default construction.
    CPPUNIT_ASSERT_NO_THROW( CurveData cd );

    // Read a library.
    Library library("./testfiles/testlibSmall");

    // Setup input.
    const double sigma = 0.31;
    const bool arearenorm = true;
    const std::string curvename("bas");
    const std::string reference_path("./testfiles/vvol_ref.data");
    CPPUNIT_ASSERT_THROW( CurveData cd(sigma, arearenorm, curvename, reference_path, library), IOException );
}


// -------------------------------------------------------------------------- //
//
void Test_CurveData::testConstructionFail4()
{
    // Fail with errors in the reference file (eof).

    // Test default construction.
    CPPUNIT_ASSERT_NO_THROW( CurveData cd );

    // Read a library.
    Library library("./testfiles/testlibSmall");

    // Setup input.
    const double sigma = 0.31;
    const bool arearenorm = true;
    const std::string curvename("bas");
    const std::string reference_path("./testfiles/bas_ref.data_eof");
    CPPUNIT_ASSERT_THROW( CurveData cd(sigma, arearenorm, curvename, reference_path, library), IOException );
}


// -------------------------------------------------------------------------- //
//
void Test_CurveData::testInitNotifyAccept()
{
    // Read a library.
    Library library("./testfiles/testlibSmall");

    // Setup input.
    const double sigma = 0.31;
    const bool arearenorm = true;
    const std::string curvename("bas");
    const std::string reference_path("./testfiles/bas_ref.data");
    CurveData cd(sigma, arearenorm, curvename, reference_path, library);

    // Setup a small sampleset.
    std::vector<int> sampleset(3);
    sampleset[0] =  7;
    sampleset[1] =  9;
    sampleset[2] = 13;

    // Call init.
    cd.init(sampleset, library);

    // Check that the data was correctly set.
    CPPUNIT_ASSERT_EQUAL( 3, cd.nsample_ );
    const double chi2 = 15.47725891756;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2, cd.chi2_, 1.0e-10 );

    // Check a few values.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( cd.curve_[12], 0.2494640000000000, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( cd.curve_[65], 0.0335777333333333, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( cd.curve_[89], 0.0332166666666667, 1.0e-12 );

    // Call notify.
    const int from_sample =  9;
    const int from_basis  = 14;
    cd.notify(from_sample, from_basis, library);

    // Check chi2 and chi2_new.
    const double chi2_new = 12.9720636980397;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2_new, cd.chi2_new_, 1.0e-8 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2,     cd.chi2_,     1.0e-8 );

    // Check that the new curve is differenct.
    double sum1 = 0.0;
    double sum2 = 0.0;
    for (size_t i = 0; i < cd.curve_.size(); ++i)
    {
        sum1 += cd.curve_[i];
        sum2 += cd.curve_new_[i];
    }
    CPPUNIT_ASSERT( sum1 != sum2 );

    // Call accept.
    cd.accept();

    // Check that chi2 was set.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2_new, cd.chi2_, 1.0e-10 );

    // Check that the curve was set.
    for (size_t i = 0; i < cd.curve_.size(); ++i)
    {
        CPPUNIT_ASSERT_DOUBLES_EQUAL( cd.curve_[i], cd.curve_new_[i], 1.0e-12 );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_CurveData::testGetChi2()
{
    CurveData cd;
    const double chi2 = 1.1246722311651241;
    cd.chi2_ = chi2;

    CPPUNIT_ASSERT_DOUBLES_EQUAL( cd.get_chi2(), chi2, 1.0e-12 );
}


// -------------------------------------------------------------------------- //
//
void Test_CurveData::testGetChi2New()
{
    CurveData cd;
    const double chi2_new = 1.1246722311651241;
    cd.chi2_new_ = chi2_new;

    CPPUNIT_ASSERT_DOUBLES_EQUAL( cd.get_chi2_new(), chi2_new, 1.0e-12 );
}


// -------------------------------------------------------------------------- //
//
void Test_CurveData::testCalculateChi2()
{
    // Read a library.
    Library library("./testfiles/testlibSmall");

    // Setup input.
    const double sigma = 0.31;
    const bool arearenorm = true;
    const std::string curvename("bas");
    const std::string reference_path("./testfiles/bas_ref.data");
    CurveData cd(sigma, arearenorm, curvename, reference_path, library);

    // Setup a curve to use for testing.
    std::vector<double> curve(cd.curve_.size(), 1.0);
    for (size_t i = 0; i < curve.size(); ++i)
    {
        curve[i] += 0.023*i - 0.009*i*i;
    }

    // Calculate chi2.
    const double chi2 = cd.calculate_chi2(curve);

    // Check against hardcoded value.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2, 54.4867765829326, 1.0e-12 );
}

