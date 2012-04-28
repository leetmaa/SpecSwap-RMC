/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/


// Include the test definition.
#include "test_scalardistributiondata.h"

// Include the files to test.
#include "../src/scalardistributiondata.h"

// Other inclusion.
#include "../src/ioexception.h"
#include "../src/library.h"
#include "../src/mathutils.h"


// -------------------------------------------------------------------------- //
//
void Test_ScalarDistributionData::testConstruction()
{
    // Make sure we can default construct without crashing.
    CPPUNIT_ASSERT_NO_THROW( ScalarDistributionData sdd );

    // Construct with the normal constructor.
    const std::string scalar_name("VP-Volume");
    const std::string ref_path("./testfiles/vvol_ref.data");
    const double sigma = 0.093;
    const int pos = 2;
    ScalarDistributionData sdd(scalar_name, ref_path, sigma, pos);
    // Check the member data.
    const double eps = 1.0e-12;
    CPPUNIT_ASSERT_EQUAL( sdd.name_, scalar_name );
    CPPUNIT_ASSERT_EQUAL( sdd.pos_, pos );
    CPPUNIT_ASSERT_EQUAL( sdd.nbins_, 125 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sdd.sigma_, sigma, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sdd.one_over_sigma2_, 1.0/(sigma*sigma), eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sdd.lowest_, 25.45, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sdd.highest_, 37.85, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sdd.one_over_binsize_, 10.0, eps );

    // Check dimensions.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(sdd.target_.size()), sdd.nbins_ );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(sdd.scale_.size()), sdd.nbins_ );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(sdd.factor_.size()), sdd.nbins_ );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(sdd.distribution_.size()), sdd.nbins_ );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(sdd.distribution_new_.size()), sdd.nbins_ );

    // Check hardcoded reference values.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sdd.target_[0],  0.00887034393507342, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sdd.target_[8],  0.00151451299040374, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sdd.target_[19], 0.00237382469428914, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sdd.target_[99], 0.00178721705381046, eps );
    //
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sdd.scale_[0],  25.5, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sdd.scale_[8],  26.3, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sdd.scale_[19], 27.4, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sdd.scale_[99], 35.4, eps );

    // Check the target norm.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( vsum(sdd.target_), 1.0, eps );

    // Test the edges of the binning routine in a realistic case.
    CPPUNIT_ASSERT_EQUAL(sdd.get_bin(25.0),   0);
    CPPUNIT_ASSERT_EQUAL(sdd.get_bin(31.83), 63);
    CPPUNIT_ASSERT_EQUAL(sdd.get_bin(37.87),124);
}


// -------------------------------------------------------------------------- //
//
void Test_ScalarDistributionData::testConstructionFail()
{
    // Fail with no file.
    const std::string scalar_name("VP-Area");
    const std::string ref_path("./testfiles/NOTAFILENAME");
    const double sigma = 0.093;
    const int pos = 2;
    CPPUNIT_ASSERT_THROW( ScalarDistributionData sdd(scalar_name, ref_path, sigma, pos), IOException );

}


// -------------------------------------------------------------------------- //
//
void Test_ScalarDistributionData::testConstructionFail2()
{
    // Fail with empty file.
    const std::string scalar_name("VP-Area");
    const std::string ref_path("./testfiles/EMPTY_FILE");
    const double sigma = 0.093;
    const int pos = 2;
    CPPUNIT_ASSERT_THROW( ScalarDistributionData sdd(scalar_name, ref_path, sigma, pos), IOException );

}


// -------------------------------------------------------------------------- //
//
void Test_ScalarDistributionData::testInitNotifyAccept()
{
    // Read a library.
    Library library("./testfiles/testlibSmall");

    // Construct with the normal constructor.
    const std::string scalar_name("VP-Volume");
    const std::string ref_path("./testfiles/vvol_ref.data");
    const double sigma = 0.093;
    const int pos = 1;
    ScalarDistributionData sdd(scalar_name, ref_path, sigma, pos);

    // Setup a small sampleset.
    std::vector<int> sampleset(10);
    sampleset[0] =  7;
    sampleset[1] =  9;
    sampleset[2] = 13;
    sampleset[3] = 27;
    sampleset[4] = 28;
    sampleset[5] = 29;
    sampleset[6] = 43;
    sampleset[7] = 53;
    sampleset[8] = 56;
    sampleset[9] = 57;

    // Call init.
    sdd.init(sampleset, library);

    // Check that the data was correctly set.
    CPPUNIT_ASSERT_EQUAL( 10, sdd.nsample_ );
    const double chi2 = 16.79464613935;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2, sdd.chi2_, 1.0e-10 );

    // Check a few values.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sdd.distribution_[1] , 0.1, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sdd.distribution_[26], 0.2, 1.0e-12 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sdd.distribution_[86], 0.1, 1.0e-12 );

    // Call notify.
    const int from_sample = 29;
    const int from_basis  = 34;
    sdd.notify(from_sample, from_basis, library);

    // Check chi2 and chi2_new.
    const double chi2_new = 16.7499785124567;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2_new, sdd.chi2_new_, 1.0e-8 );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2,     sdd.chi2_,     1.0e-8 );

    // Call accept.
    sdd.accept();

    // Check that chi2 was set.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2_new, sdd.chi2_, 1.0e-10 );

    // Check that the curve was set.
    for (size_t i = 0; i < sdd.distribution_.size(); ++i)
    {
        CPPUNIT_ASSERT_DOUBLES_EQUAL( sdd.distribution_[i], sdd.distribution_new_[i], 1.0e-12 );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_ScalarDistributionData::testGetChi2()
{
    ScalarDistributionData sdd;
    const double chi2 = 1.1246722311651241;
    sdd.chi2_ = chi2;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sdd.get_chi2(), chi2, 1.0e-12 );
}


// -------------------------------------------------------------------------- //
//
void Test_ScalarDistributionData::testGetChi2New()
{
    ScalarDistributionData sdd;
    const double chi2_new = 1.1246722311651241;
    sdd.chi2_new_ = chi2_new;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( sdd.get_chi2_new(), chi2_new, 1.0e-12 );
}


// -------------------------------------------------------------------------- //
//
void Test_ScalarDistributionData::testCalculateChi2()
{
    ScalarDistributionData sdd;
    // Set the target.
    std::vector<double> target;
    target.push_back(0.3);
    target.push_back(0.5);
    target.push_back(0.2);
    sdd.target_ = target;

    // Set the factor.
    std::vector<double> factor;
    factor.push_back(0.1);
    factor.push_back(0.2);
    factor.push_back(1.0);
    sdd.factor_ = factor;

    // Setup a distribution.
    std::vector<double> distribution;
    distribution.push_back(0.8);
    distribution.push_back(0.2);
    distribution.push_back(0.0);

    // Setup a one_over_sigma2
    const double one_over_sigma2 = 123.0;
    sdd.one_over_sigma2_ = one_over_sigma2;

    // Calculate chi2.
    const double chi2 = sdd.calculate_chi2(distribution);

    // Setup the reference chi2 (pen and paper).
    // (0.1*(0.8-0.3)^2 + 0.2*(0.2-0.5)^2 + 1.0*(0.0-0.2)^2)*123.0 = 10.209
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2, 10.209, 1.0e-12 );

}


// -------------------------------------------------------------------------- //
//
void Test_ScalarDistributionData::testGetBin()
{
    ScalarDistributionData sdd;

    // Bins 0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0
    sdd.lowest_ = 0.0 - 0.05;
    sdd.highest_ = 1.0 - 0.05;
    sdd.one_over_binsize_ = 10.0;
    sdd.nbins_ = 11;

    CPPUNIT_ASSERT_EQUAL(sdd.get_bin(-2.05), 0);
    CPPUNIT_ASSERT_EQUAL(sdd.get_bin(-0.05), 0);
    CPPUNIT_ASSERT_EQUAL(sdd.get_bin(-0.04), 0);
    CPPUNIT_ASSERT_EQUAL(sdd.get_bin( 0.04), 0);
    CPPUNIT_ASSERT_EQUAL(sdd.get_bin( 0.05), 1);
    CPPUNIT_ASSERT_EQUAL(sdd.get_bin( 0.12), 1);
    CPPUNIT_ASSERT_EQUAL(sdd.get_bin( 0.76), 8);
    CPPUNIT_ASSERT_EQUAL(sdd.get_bin( 0.91), 9);
    CPPUNIT_ASSERT_EQUAL(sdd.get_bin( 0.95), 10);
    CPPUNIT_ASSERT_EQUAL(sdd.get_bin( 3.95), 10);

}




