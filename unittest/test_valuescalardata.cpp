/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/


// Include the test definition.
#include "test_valuescalardata.h"

// Include the files to test.
#include "../src/valuescalardata.h"

// Other inclusion.
#include "../src/ioutils.h"
#include "../src/library.h"

// -------------------------------------------------------------------------- //
//
void Test_ValueScalarData::testConstruction()
{
    // Check that we can use the default constructor.
    CPPUNIT_ASSERT_NO_THROW(ValueScalarData vsd);

    // Setup.
    const std::string name("VP-Volume");
    const std::pair<double, double> interval(30.0, 32.0);
    const double target = 0.6;
    const double sigma = 0.3;
    const int pos = 1;
    ValueScalarData vsd(name, interval, target, sigma, pos);

    // Check values of member data.
    const double eps = 1.0e-12;
    CPPUNIT_ASSERT_EQUAL(vsd.name_, name);
    CPPUNIT_ASSERT_EQUAL(vsd.pos_, pos);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(vsd.sigma_, sigma, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(vsd.one_over_sigma2_, 11.11111111111111, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(vsd.interval_.first, interval.first, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(vsd.interval_.second, interval.second, eps);
}


// -------------------------------------------------------------------------- //
//
void Test_ValueScalarData::testInitNotifyAccept()
{
    // Read a library.
    Library library("./testfiles/testlibSmall");

    // Setup input.
    const std::string name("VP-Volume");
    const std::pair<double, double> interval(30.0, 32.0);
    const double target = 0.6;
    const double sigma = 0.3;
    const int pos = 1;
    ValueScalarData vsd(name, interval, target, sigma, pos);

    // Setup a small sampleset.
    std::vector<int> sampleset(3);
    sampleset[0] =  7;
    sampleset[1] =  9;
    sampleset[2] = 13;

    // Call init.
    vsd.init(sampleset, library);

    // Check that the data was correctly set.
    CPPUNIT_ASSERT_EQUAL( 3, vsd.nsample_ );
    const double chi2 = 0.790123456790123;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2, vsd.chi2_, 1.0e-10 );

    // Check a few values.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vsd.value_, 0.333333333333333, 1.0e-12 );

    // Call notify.
    const int from_sample =  7;
    const int from_basis  = 14;
    vsd.notify(from_sample, from_basis, library);

    // Check chi2 and chi2_new.
    const double chi2_new = 4.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2_new, vsd.chi2_new_, 1.0e-8 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2,     vsd.chi2_,     1.0e-8 );

    CPPUNIT_ASSERT_DOUBLES_EQUAL( vsd.value_,     0.333333333333333, 1.0e-8 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vsd.value_new_, 0.0, 1.0e-8 );

    // Call accept.
    vsd.accept();

    // Check that chi2 was set.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2_new, vsd.chi2_, 1.0e-10 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vsd.value_, vsd.value_new_, 1.0e-12 );
}


// -------------------------------------------------------------------------- //
//
void Test_ValueScalarData::testGetChi2()
{
    ValueScalarData vsd;
    const double chi2 = 1.1246722311651241;
    vsd.chi2_ = chi2;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vsd.get_chi2(), chi2, 1.0e-12 );
}


// -------------------------------------------------------------------------- //
//
void Test_ValueScalarData::testGetChi2New()
{
    ValueScalarData vsd;
    const double chi2_new = 1.1246722311651241;
    vsd.chi2_new_ = chi2_new;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vsd.get_chi2_new(), chi2_new, 1.0e-12 );
}


// -------------------------------------------------------------------------- //
//
void Test_ValueScalarData::testCalculateChi2()
{
    ValueScalarData vsd;
    vsd.target_ = 3.0;
    vsd.one_over_sigma2_ = 9.0;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(vsd.calculate_chi2(1.0), 36.0, 1.0e-12);
}

