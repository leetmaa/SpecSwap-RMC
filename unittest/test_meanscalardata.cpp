/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/


// Include the test definition.
#include "test_meanscalardata.h"

// Include the files to test.
#include "../src/meanscalardata.h"

// Other inclusion.
#include "../src/ioutils.h"
#include "../src/library.h"

// -------------------------------------------------------------------------- //
//
void Test_MeanScalarData::testConstruction()
{
    // Check that we can use the default constructor.
    CPPUNIT_ASSERT_NO_THROW(MeanScalarData msd);

    // Setup.
    const std::string name("VP-Volume");
    const double target = 31.23;
    const double sigma = 0.3;
    const int pos = 1;
    MeanScalarData msd(name, target, sigma, pos);

    // Check values of member data.
    const double eps = 1.0e-12;
    CPPUNIT_ASSERT_EQUAL(msd.name_, name);
    CPPUNIT_ASSERT_EQUAL(msd.pos_, pos);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(msd.sigma_, sigma, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(msd.one_over_sigma2_, 11.11111111111111, eps);

}


// -------------------------------------------------------------------------- //
//
void Test_MeanScalarData::testInitNotifyAccept()
{
    // Read a library.
    Library library("./testfiles/testlibSmall");

    // Setup input.
    const std::string name("VP-Volume");
    const double target = 31.23;
    const double sigma = 0.3;
    const int pos = 1;
    MeanScalarData msd(name, target, sigma, pos);

    // Setup a small sampleset.
    std::vector<int> sampleset(3);
    sampleset[0] =  7;
    sampleset[1] =  9;
    sampleset[2] = 13;

    // Call init.
    msd.init(sampleset, library);

    // Check that the data was correctly set.
    CPPUNIT_ASSERT_EQUAL( 3, msd.nsample_ );
    const double chi2 = 1.73361111111114;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2, msd.chi2_, 1.0e-10 );

    // Check a few values.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( msd.value_, 30.835, 1.0e-12 );

    // Call notify.
    const int from_sample =  9;
    const int from_basis  = 14;
    msd.notify(from_sample, from_basis, library);

    // Check chi2 and chi2_new.
    const double chi2_new = 0.549245679012354;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2_new, msd.chi2_new_, 1.0e-8 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2,     msd.chi2_,     1.0e-8 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( msd.value_,     30.835,           1.0e-8 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( msd.value_new_, 31.0076666666667, 1.0e-8 );

    // Call accept.
    msd.accept();

    // Check that chi2 was set.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2_new, msd.chi2_, 1.0e-10 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( msd.value_, msd.value_new_, 1.0e-12 );
}


// -------------------------------------------------------------------------- //
//
void Test_MeanScalarData::testGetChi2()
{
    MeanScalarData msd;
    const double chi2 = 1.1246722311651241;
    msd.chi2_ = chi2;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( msd.get_chi2(), chi2, 1.0e-12 );
}


// -------------------------------------------------------------------------- //
//
void Test_MeanScalarData::testGetChi2New()
{
    MeanScalarData msd;
    const double chi2_new = 1.1246722311651241;
    msd.chi2_new_ = chi2_new;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( msd.get_chi2_new(), chi2_new, 1.0e-12 );
}


// -------------------------------------------------------------------------- //
//
void Test_MeanScalarData::testCalculateChi2()
{
    MeanScalarData msd;
    msd.target_ = 3.0;
    msd.one_over_sigma2_ = 9.0;

    CPPUNIT_ASSERT_DOUBLES_EQUAL(msd.calculate_chi2(1.0), 36.0, 1.0e-12);
}

