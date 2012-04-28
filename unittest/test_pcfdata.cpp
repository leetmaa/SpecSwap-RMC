/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/


// Include the test definition.
#include "test_pcfdata.h"

// Include the files to test.
#include "../src/pcfdata.h"

// Other inclusion.
#include "../src/ioexception.h"
#include "../src/library.h"

// -------------------------------------------------------------------------- //
//
void Test_PCFData::testConstruction()
{
    // Test default construction.
    CPPUNIT_ASSERT_NO_THROW(PCFData pcf);

    // Setup input.
    const double rmin  = 0.0;
    const double rmax  = 5.0;
    const double dr    = 0.02;
    const double sigma = 0.01;
    const double numberdensity = 0.3;
    const std::pair<double, double> fit_inteval(0.2, 1.0);
    const int nbins = 250;
    const std::pair<int,int> partial(0,1);
    const std::string reference_data("./testfiles/gr_ref.data");

    // Check that the correct memberdata was set.
    PCFData pcf(rmin, rmax, dr, sigma, numberdensity, fit_inteval, nbins, partial, reference_data);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( rmin, pcf.rmin_, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( rmax, pcf.rmax_, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( dr, pcf.dr_, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/dr, pcf.one_over_dr_, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sigma, pcf.sigma_, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0/(sigma*sigma), pcf.one_over_sigma2_, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( numberdensity, pcf.numberdensity_, 1.0e-12 );
    CPPUNIT_ASSERT_EQUAL( partial.first, pcf.partial_.first );
    CPPUNIT_ASSERT_EQUAL( partial.second, pcf.partial_.second );
    CPPUNIT_ASSERT_EQUAL( 10, pcf.fit_interval_.first );
    CPPUNIT_ASSERT_EQUAL( 50, pcf.fit_interval_.second );

    // Check a few of the reference values.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( pcf.pcf_reference_[12],  0.000000000, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( pcf.pcf_reference_[39],  0.166035800, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( pcf.pcf_reference_[48], 15.308580000, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( pcf.pcf_reference_[123], 0.547787900, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( pcf.pcf_reference_[168], 1.341329000, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( pcf.pcf_reference_[231], 0.955944800, 1.0e-12 );
}


// -------------------------------------------------------------------------- //
//
void Test_PCFData::testConstructionFail()
{
    // Setup input.
    const double rmin  = -0.02;
    const double rmax  = 5.0;
    const double dr    = 0.02;
    const int nbins    = 250;

    const double sigma = 0.01;
    const double numberdensity = 0.3;
    const std::pair<double, double> fit_inteval(0.2, 1.0);
    const std::pair<int,int> partial(0,1);
    const std::string reference_data("./testfiles/gr_ref.data");

    // Error with inconsistent rmin, rmax, dr and nbins.
    CPPUNIT_ASSERT_THROW( PCFData(rmin, rmax, dr, sigma, numberdensity, fit_inteval, nbins, partial, reference_data), IOException );
}


// -------------------------------------------------------------------------- //
//
void Test_PCFData::testConstructionFail2()
{
    // Setup input.
    const double rmin  = 0.0;
    const double rmax  = 5.0;
    const double dr    = 0.02;
    const int nbins    = 250;

    const double sigma = 0.01;
    const double numberdensity = 0.3;
    const std::pair<double, double> fit_inteval(0.2, 5.4);
    const std::pair<int,int> partial(0,1);
    const std::string reference_data("./testfiles/gr_ref.data");

    // Error with fit interval out of bounds.
    CPPUNIT_ASSERT_THROW( PCFData(rmin, rmax, dr, sigma, numberdensity, fit_inteval, nbins, partial, reference_data), IOException );
}


// -------------------------------------------------------------------------- //
//
void Test_PCFData::testConstructionFail3()
{
    // Setup input.
    const double rmin  = 0.0;
    const double rmax  = 5.0;
    const double dr    = 0.02;
    const double sigma = 0.01;
    const double numberdensity = 0.3;
    const std::pair<double, double> fit_inteval(0.2, 1.0);
    const int nbins = 250;
    const std::pair<int,int> partial(0,1);

    // Error with wrong file name.
    std::string reference_data("NOTANAME");
    CPPUNIT_ASSERT_THROW( PCFData(rmin, rmax, dr, sigma, numberdensity, fit_inteval, nbins, partial, reference_data), IOException );

    // Error with truncated file.
    reference_data = "./testfiles/gr_ref.data_trunc";
    CPPUNIT_ASSERT_THROW( PCFData(rmin, rmax, dr, sigma, numberdensity, fit_inteval, nbins, partial, reference_data), IOException );

}


// -------------------------------------------------------------------------- //
//
void Test_PCFData::testInitNotifyAccept()
{
    // Setup.
    const double rmin  = 0.0;
    const double rmax  = 5.0;
    const double dr    = 0.02;
    const double sigma = 0.01;
    const double numberdensity = 0.3;
    const std::pair<double, double> fit_inteval(0.2, 1.0);
    const int nbins = 250;
    const std::pair<int,int> partial(0,1);
    const std::string reference_data("./testfiles/gr_ref.data");
    PCFData pcf(rmin, rmax, dr, sigma, numberdensity, fit_inteval, nbins, partial, reference_data);

    // Read a library.
    Library library("./testfiles/testlibSmall");

    // Setup a small sampleset.
    std::vector<int> sampleset(3);
    sampleset[0] =  1;
    sampleset[1] = 14;
    sampleset[2] =  4;

    // Call init.
    pcf.init(sampleset, library);

    // Check that the data was correctly set.
    CPPUNIT_ASSERT_EQUAL( 3, pcf.nsample_ );
    const double chi2 = 266976.424804889;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2, pcf.chi2_, 1.0e-8 );

    for (size_t i = 0; i < pcf.histogram_.size(); ++i)
    {
        CPPUNIT_ASSERT_DOUBLES_EQUAL( pcf.histogram_[i], pcf.histogram_new_[i], 1.0e-12 );
    }

    // Call notify.
    const int from_sample = 1;
    const int from_basis  = 3;
    pcf.notify(from_sample, from_basis, library);

    // Check chi2 and chi2_new.
    const double chi2_new = 266976.424804889;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2_new, pcf.chi2_new_, 1.0e-8 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2,     pcf.chi2_,     1.0e-8 );

    // Check that the new histogram is differenct.
    double sum1 = 0.0;
    double sum2 = 0.0;
    for (size_t i = 0; i < pcf.histogram_.size(); ++i)
    {
        sum1 += pcf.histogram_[i];
        sum2 += pcf.histogram_new_[i];
    }
    CPPUNIT_ASSERT( sum1 != sum2 );

    // Call accept.
    pcf.accept();

    // Check that chi2 was set.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2_new, pcf.chi2_, 1.0e-8 );

    // Check that the histograms are identical again.

    for (size_t i = 0; i < pcf.histogram_.size(); ++i)
    {
        CPPUNIT_ASSERT_DOUBLES_EQUAL( pcf.histogram_[i], pcf.histogram_new_[i], 1.0e-12 );
    }

}


// -------------------------------------------------------------------------- //
//
void Test_PCFData::testGetChi2()
{
    PCFData pcf;
    const double chi2 = 1.1246722311651241;
    pcf.chi2_ = chi2;

    CPPUNIT_ASSERT_DOUBLES_EQUAL( pcf.get_chi2(), chi2, 1.0e-12 );
}


// -------------------------------------------------------------------------- //
//
void Test_PCFData::testGetChi2New()
{
    PCFData pcf;
    const double chi2_new = 1.1246722311651241;
    pcf.chi2_new_ = chi2_new;

    CPPUNIT_ASSERT_DOUBLES_EQUAL( pcf.get_chi2_new(), chi2_new, 1.0e-12 );
}


// -------------------------------------------------------------------------- //
//
void Test_PCFData::testGetR()
{
    PCFData pcf;
    // Set the values.
    pcf.rmin_ = 0.98;
    pcf.dr_   = 0.01;
    // Test.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( pcf.get_r(4),      1.025,    1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( pcf.get_r(-12),    0.865,    1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( pcf.get_r(123620), 1237.185, 1.0e-12 );
}


// -------------------------------------------------------------------------- //
//
void Test_PCFData::testCalculateBin()
{
    PCFData pcf;
    // Set the values.
    pcf.rmin_ = 0.98;
    pcf.dr_   = 0.01;
    pcf.one_over_dr_ = 1.0/pcf.dr_;
    // Test.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( pcf.calculate_bin(1.025),         4, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( pcf.calculate_bin(0.865),       -12, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( pcf.calculate_bin(1237.185), 123620, 1.0e-12 );
}


// -------------------------------------------------------------------------- //
//
void Test_PCFData::testGetRAndCalculateBin()
{
    PCFData pcf;
    // Set the values.
    pcf.rmin_ = 0.98;
    pcf.dr_   = 0.01;
    pcf.one_over_dr_ = 1.0/pcf.dr_;
    // Test.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( pcf.calculate_bin(pcf.get_r(123)), 123, 1.0e-12 );
}


// -------------------------------------------------------------------------- //
//
void Test_PCFData::testCalculatePartialHistogram()
{
    // Setup.
    const double rmin  = 0.0;
    const double rmax  = 5.0;
    const double dr    = 0.02;
    const double sigma = 0.01;
    const double numberdensity = 0.3;
    const std::pair<double, double> fit_inteval(0.2, 1.0);
    const int nbins = 250;
    const std::pair<int,int> partial(0,1);
    const std::string reference_data("./testfiles/gr_ref.data");
    PCFData pcf(rmin, rmax, dr, sigma, numberdensity, fit_inteval, nbins, partial, reference_data);

    // Read in a library.
    Library library("./testfiles/testlibSmall");
    int index = 3;

    // Get the histogram.
    std::vector<double> histogram = pcf.calculate_partial_histogram(library, index);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(histogram.size()), nbins);

    // Test.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[80],  1.0, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[81],  0.0, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[188], 3.0, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[191], 2.0, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[225], 1.0, 1.0e-12 );

    // Get another histogram.
    index = 8;
    histogram = pcf.calculate_partial_histogram(library, index);
    CPPUNIT_ASSERT_EQUAL(static_cast<int>(histogram.size()), nbins);

    // Test.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[75],  1.0, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[81],  0.0, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[188], 1.0, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[191], 0.0, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( histogram[192], 3.0, 1.0e-12 );

}


// -------------------------------------------------------------------------- //
//
void Test_PCFData::testCalculateChi2()
{
    // Setup.
    const double rmin  = 0.0;
    const double rmax  = 5.0;
    const double dr    = 0.02;
    const double sigma = 0.01;
    const double numberdensity = 0.3;
    const std::pair<double, double> fit_inteval(0.2, 1.0);
    const int nbins = 250;
    const std::pair<int,int> partial(0,1);
    const std::string reference_data("./testfiles/gr_ref.data");
    PCFData pcf(rmin, rmax, dr, sigma, numberdensity, fit_inteval, nbins, partial, reference_data);

    // Setup a histogram to use for testing.
    std::vector<double> histogram(nbins, 1.0);
    for (size_t i = 0; i < histogram.size(); ++i)
    {
        histogram[i] += 0.003*i;
    }

    // Calculate chi2.
    double chi2 = pcf.calculate_chi2(histogram);

    // Check against hardcoded value.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2, 861059546.103726, 1.0e-12 );
}

