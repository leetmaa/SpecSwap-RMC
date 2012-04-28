/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/


// Include the test definition.
#include "test_specswap.h"

// Include the files to test.
#include "../src/specswap.h"
#include "../src/ioexception.h"

// Other inclusions.
#include <cmath>


// -------------------------------------------------------------------------- //
//
void Test_Specswap::testConstruction()
{
    // Try to default construct. Make sure this does not throw an error.
    CPPUNIT_ASSERT_NO_THROW(Specswap ss);

    // Construct with the normal constructor.
    int sample = 250;
    Mlrnd rand;
    std::string path("./testfiles/testlibSmall");
    CPPUNIT_ASSERT_NO_THROW(Specswap ss(sample, rand, path));

    // Construct with the restart constructor.
    std::string restart_path("./testfiles/tmprun.restart");
    CPPUNIT_ASSERT_NO_THROW(Specswap ss(sample, rand, path, restart_path));

    // Make sure we fail with a too large sample.
    sample = 251;
    CPPUNIT_ASSERT_THROW(Specswap ss(sample, rand, path), IOException );
    CPPUNIT_ASSERT_THROW(Specswap ss(sample, rand, path, restart_path), IOException );

    // Make sure we fail with a path that does not exist.
    sample = 100;
    path = "./testfiles/NO_SUCH_FILE";
    CPPUNIT_ASSERT_THROW(Specswap ss(sample, rand, path), IOException );
    CPPUNIT_ASSERT_THROW(Specswap ss(sample, rand, path, restart_path), IOException );

    // Make sure we fail with a path pointing to a file which is not a library file.
    path = "./testfiles/NOT_A_LIBRARY";
    CPPUNIT_ASSERT_THROW(Specswap ss(sample, rand, path), IOException );
    CPPUNIT_ASSERT_THROW(Specswap ss(sample, rand, path, restart_path), IOException );

}


// -------------------------------------------------------------------------- //
//
void Test_Specswap::testAddScalarMean()
{
    // Setup a specswap object.
    int sample = 250;
    Mlrnd rand;
    std::string path("./testfiles/testlibLarge");
    Specswap ss(sample, rand, path);

    // Check that there are no mean scalars added.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ss.mean_scalar_data_.size()), 0 );

    // Setup input for adding a mean scalar.
    std::string scalarname("VP-Volume");
    double target = 0.5;
    double sigma = 1.0;
    ss.add_scalar_mean(scalarname, target, sigma);

    // Check that a scalar mean has been added.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ss.mean_scalar_data_.size()), 1 );

    // Add another one.
    ss.add_scalar_mean(scalarname, target, sigma);

    // Check.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ss.mean_scalar_data_.size()), 2 );

    // Make sure we fail with an unrecognized scalar.
    scalarname = "NOT_A_SCALARNAME";
    CPPUNIT_ASSERT_THROW( ss.add_scalar_mean(scalarname, target, sigma), IOException );

}


// -------------------------------------------------------------------------- //
//
void Test_Specswap::testAddScalarValue()
{
    // Setup a specswap object.
    int sample = 250;
    Mlrnd rand;
    std::string path("./testfiles/testlibLarge");
    Specswap ss(sample, rand, path);

    // Check that there are no value scalars added.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ss.value_scalar_data_.size()), 0 );

    // Setup input for adding a value scalar.
    std::string scalarname("VP-Area");
    double value_low = 0.5;
    double value_high = 1.0;
    double fraction = 0.6;
    double sigma = 0.0001;
    ss.add_scalar_value(scalarname, value_low, value_high, fraction, sigma);

    // Check that a scalar value has been added.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ss.value_scalar_data_.size()), 1 );

    // Add another one.
    ss.add_scalar_value(scalarname, value_low, value_high, fraction, sigma);

    // Check.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ss.value_scalar_data_.size()), 2 );

    // Make sure we fail with an unrecognized scalar.
    scalarname = "NOT_A_SCALARNAME";
    CPPUNIT_ASSERT_THROW( ss.add_scalar_value(scalarname, value_low, value_high, fraction, sigma), IOException );

}


// -------------------------------------------------------------------------- //
//
void Test_Specswap::testAddScalarDistribution()
{
    // Setup a specswap object.
    int sample = 250;
    Mlrnd rand;
    std::string path("./testfiles/testlibLarge");
    Specswap ss(sample, rand, path);

    // Check that there are no scalar distributions added.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ss.scalar_distribution_data_.size()), 0 );

    // Setup input for adding a scalar distribution.
    std::string scalarname("VP-Volume");
    std::string filename("./testfiles/vvol_ref.data");
    double sigma = 0.0002;
    ss.add_scalar_distribution(scalarname, filename, sigma);

    // Check that a scalar distribution has been added.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ss.scalar_distribution_data_.size()), 1 );

    // Add another one.
    ss.add_scalar_distribution(scalarname, filename, sigma);

    // Check.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ss.scalar_distribution_data_.size()), 2 );

    // Make sure we fail with an unrecognized scalar.
    scalarname = "NOT_A_SCALARNAME";
    //ss.add_scalar_distribution(scalarname, filename, sigma);
    CPPUNIT_ASSERT_THROW( ss.add_scalar_distribution(scalarname, filename, sigma), IOException );

    // Make sure we fail with a non existing input file.
    scalarname = "VP-Volume";
    filename = "./testfiles/NO_SUCH_FILE";
    //ss.add_scalar_distribution(scalarname, filename, sigma);
    CPPUNIT_ASSERT_THROW( ss.add_scalar_distribution(scalarname, filename, sigma), IOException );
}


// -------------------------------------------------------------------------- //
//
void Test_Specswap::testAddCurve()
{
    // Setup a specswap object.
    int sample = 250;
    Mlrnd rand;
    std::string path("./testfiles/testlibLarge");
    Specswap ss(sample, rand, path);

    // Check that there are no curve data added.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ss.curve_data_.size()), 0 );

    // Setup input for adding a curve.
    std::string curve_name("bas");
    std::string filename("./testfiles/exafs_ref.data");
    double sigma = 0.0002;
    bool area_renorm = true;
    ss.add_curve(sigma, area_renorm, curve_name, filename);

    // Check that a scalar distribution has been added.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ss.curve_data_.size()), 1 );

    // Add another one.
    ss.add_curve(sigma, area_renorm, curve_name, filename);

    // Check.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ss.curve_data_.size()), 2 );

    // Make sure we fail with an unrecognized curve.
    curve_name = "NOT_A_CURVENAME";
    //ss.add_curve(sigma, area_renorm, curve_name, filename);
    CPPUNIT_ASSERT_THROW( ss.add_curve(sigma, area_renorm, curve_name, filename), IOException );

    // Make sure we fail with a non existing reference file.
    curve_name = "bas";
    filename = "./testfiles/NO_SUCH_FILE";
    //ss.add_curve(sigma, area_renorm, curve_name, filename);
    CPPUNIT_ASSERT_THROW( ss.add_curve(sigma, area_renorm, curve_name, filename), IOException );

}


// -------------------------------------------------------------------------- //
//
void Test_Specswap::testAddPCF()
{
    // Setup a specswap object.
    int sample = 250;
    Mlrnd rand;
    std::string path("./testfiles/testlibLarge");
    Specswap ss(sample, rand, path);

    // Check that there are no pcf data added.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ss.pcf_data_.size()), 0 );

    // Setup input for adding a pcf.
    double rmin = 0.0;
    double rmax = 5.0;
    double dr = 0.02;
    double sigma = 0.01;
    double numberdensity = 0.4;
    std::pair<double,double> fit_interval(1.3, 3.0);
    int nbins = 250;
    std::pair<int,int> partial(0,1);
    std::string ref_path("./testfiles/gr_ref.data");

    ss.add_pcf(rmin, rmax, dr, sigma, numberdensity, fit_interval, nbins, partial, ref_path);

    // Check that a pcf has been added.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ss.pcf_data_.size()), 1 );

    // Add another one.
    ss.add_pcf(rmin, rmax, dr, sigma, numberdensity, fit_interval, nbins, partial, ref_path);

    // Check.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(ss.pcf_data_.size()), 2 );

    // Make sure we fail with wrong rmin rmax and dr.
    {
        double rmin = 2.3;
        double rmax = 4.0;
        double dr = 0.02;
        //ss.add_pcf(rmin, rmax, dr, sigma, numberdensity, fit_interval, nbins, partial, ref_path);
        CPPUNIT_ASSERT_THROW( ss.add_pcf(rmin, rmax, dr, sigma, numberdensity, fit_interval, nbins, partial, ref_path), IOException );
    }

    // Make sure we fail with wrong rmin rmax and dr.
    {
        std::pair<double, double> fit_interval(12.3,1.23);
        //ss.add_pcf(rmin, rmax, dr, sigma, numberdensity, fit_interval, nbins, partial, ref_path);
        CPPUNIT_ASSERT_THROW( ss.add_pcf(rmin, rmax, dr, sigma, numberdensity, fit_interval, nbins, partial, ref_path), IOException );
    }

    // Make sure we fail with a too large number of bins.
    {
        int nbins = 1000;
        double rmin = 0.0;
        double rmax = 10.0;
        double dr = 0.02;
        //ss.add_pcf(rmin, rmax, dr, sigma, numberdensity, fit_interval, nbins, partial, ref_path);
        CPPUNIT_ASSERT_THROW( ss.add_pcf(rmin, rmax, dr, sigma, numberdensity, fit_interval, nbins, partial, ref_path), IOException );
    }

    // Make sure we fail with wrong reference path.
    {
        std::string ref_path("./testfiles/NO_SUCH_FILE");
        //ss.add_pcf(rmin, rmax, dr, sigma, numberdensity, fit_interval, nbins, partial, ref_path);
        CPPUNIT_ASSERT_THROW( ss.add_pcf(rmin, rmax, dr, sigma, numberdensity, fit_interval, nbins, partial, ref_path), IOException );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_Specswap::testSetup()
{
    // Setup a specswap object.
    int sample = 250;
    Mlrnd rand;
    std::string path("./testfiles/testlibSmall");
    Specswap ss0(sample, rand, path);
    // Set the initial chi2_ value to some thing unreasonable.
    ss0.chi2_ = -1.123;
    // Call the setup function.
    ss0.setup();
    // Check that the chi2 value has been updated.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( ss0.chi2_, 0.0, 1.0e-12 );

    // Setup a specswap object with many different data sets.
    path = "./testfiles/testlibLarge";
    Specswap ss1(sample, rand, path);

    // Mean scalar.
    std::string scalarname("VP-Volume");
    double target = 0.5;
    double sigma = 1.0;
    ss1.add_scalar_mean(scalarname, target, sigma);
    sigma = 0.0012;
    ss1.add_scalar_mean(scalarname, target, sigma);

    // Value scalar.
    scalarname = "VP-Area";
    double value_low = 0.5;
    double value_high = 1.0;
    double fraction = 0.6;
    sigma = 0.0001;
    ss1.add_scalar_value(scalarname, value_low, value_high, fraction, sigma);
    fraction = 0.1;
    sigma = 0.0001;
    ss1.add_scalar_value(scalarname, value_low, value_high, fraction, sigma);
    value_low = 0.0;
    value_high = 1.3;
    fraction = 0.99;
    sigma = 0.01;
    ss1.add_scalar_value(scalarname, value_low, value_high, fraction, sigma);

    // Scalar distribution.
    scalarname = "VP-Volume";
    std::string filename("./testfiles/vvol_ref.data");
    sigma = 0.0002;
    ss1.add_scalar_distribution(scalarname, filename, sigma);
    sigma = 0.0099;
    ss1.add_scalar_distribution(scalarname, filename, sigma);

    // Curve.
    std::string curve_name("bas");
    filename = "./testfiles/exafs_ref.data";
    sigma = 0.0002;
    bool area_renorm = true;
    ss1.add_curve(sigma, area_renorm, curve_name, filename);
    area_renorm = false;
    ss1.add_curve(sigma, area_renorm, curve_name, filename);

    // Setup input for adding a pcf.
    double rmin = 0.0;
    double rmax = 5.0;
    double dr = 0.02;
    double numberdensity = 0.4;
    std::pair<double,double> fit_interval(1.3, 3.0);
    int nbins = 250;
    std::pair<int,int> partial(0,1);
    std::string ref_path("./testfiles/gr_ref.data");
    sigma = 0.01;
    ss1.add_pcf(rmin, rmax, dr, sigma, numberdensity, fit_interval, nbins, partial, ref_path);
    sigma = 0.0123;
    ss1.add_pcf(rmin, rmax, dr, sigma, numberdensity, fit_interval, nbins, partial, ref_path);


    // Set the initial chi2_ value to some thing unreasonable.
    ss1.chi2_ = -1.123;
    // Call the setup function.
    ss1.setup();
    // Check that the chi2 value has been updated.
    double ref_chi2 = 680957954.387089;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( ss1.chi2_, ref_chi2, 1.0e-12 );

}


// -------------------------------------------------------------------------- //
//
void Test_Specswap::testMove()
{
    // Setup a specswap intance.
    int sample = 20;
    Mlrnd rand;
    rand.set_seed(123);
    std::string path("./testfiles/testlibSmall");
    Specswap ss(sample, rand, path);

    // Setup must be called for the sampleset to be initialized.
    ss.setup();

    // Set the move parameters to dummy values.
    ss.slot_ = -1;
    ss.from_sample_ = -1;
    ss.from_basis_ = -1;

    // Call move and check the values.
    ss.move();
    CPPUNIT_ASSERT_EQUAL( ss.slot_,        1   );
    CPPUNIT_ASSERT_EQUAL( ss.from_sample_, 479 );
    CPPUNIT_ASSERT_EQUAL( ss.from_basis_,  486 );
    ss.move();
    CPPUNIT_ASSERT_EQUAL( ss.slot_,        12  );
    CPPUNIT_ASSERT_EQUAL( ss.from_sample_, 107 );
    CPPUNIT_ASSERT_EQUAL( ss.from_basis_,  499 );
    ss.move();
    CPPUNIT_ASSERT_EQUAL( ss.slot_,        3   );
    CPPUNIT_ASSERT_EQUAL( ss.from_sample_, 350 );
    CPPUNIT_ASSERT_EQUAL( ss.from_basis_,  127 );
    ss.move();
    CPPUNIT_ASSERT_EQUAL( ss.slot_,        2   );
    CPPUNIT_ASSERT_EQUAL( ss.from_sample_, 210 );
    CPPUNIT_ASSERT_EQUAL( ss.from_basis_,  472 );
    ss.move();
    CPPUNIT_ASSERT_EQUAL( ss.slot_,        17  );
    CPPUNIT_ASSERT_EQUAL( ss.from_sample_, 411 );
    CPPUNIT_ASSERT_EQUAL( ss.from_basis_,  156 );

}


// -------------------------------------------------------------------------- //
//
void Test_Specswap::testGetChi2()
{
    Specswap ss;
    double chi2 = 1.715432193931531e+03;
    ss.chi2_ = chi2;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2, ss.get_chi2(), 1.0e-10 );

    double chi2_new = 6.3394503698e+01;
    ss.chi2_new_ = chi2_new;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( chi2_new, ss.get_chi2_new(), 1.0e-10 );

}


// -------------------------------------------------------------------------- //
//
void Test_Specswap::testNotify()
{
    // Setup a specswap object with many different data sets.
    std::string path("./testfiles/testlibLarge");
    int sample = 12;
    Mlrnd rand;
    rand.set_seed(39);
    Specswap ss1(sample, rand, path);

    // Mean scalar.
    std::string scalarname("VP-Volume");
    double target = 0.5;
    double sigma = 1.0;
    ss1.add_scalar_mean(scalarname, target, sigma);
    sigma = 0.0012;
    ss1.add_scalar_mean(scalarname, target, sigma);

    // Value scalar.
    scalarname = "VP-Area";
    double value_low = 0.5;
    double value_high = 1.0;
    double fraction = 0.6;
    sigma = 0.0001;
    ss1.add_scalar_value(scalarname, value_low, value_high, fraction, sigma);
    fraction = 0.1;
    sigma = 0.0001;
    ss1.add_scalar_value(scalarname, value_low, value_high, fraction, sigma);
    value_low = 0.0;
    value_high = 1.3;
    fraction = 0.99;
    sigma = 0.01;
    ss1.add_scalar_value(scalarname, value_low, value_high, fraction, sigma);

    // Scalar distribution.
    scalarname = "VP-Volume";
    std::string filename("./testfiles/vvol_ref.data");
    sigma = 0.0002;
    ss1.add_scalar_distribution(scalarname, filename, sigma);
    sigma = 0.0099;
    ss1.add_scalar_distribution(scalarname, filename, sigma);

    // Curve.
    std::string curve_name("bas");
    filename = "./testfiles/exafs_ref.data";
    sigma = 0.0002;
    bool area_renorm = true;
    ss1.add_curve(sigma, area_renorm, curve_name, filename);
    area_renorm = false;
    ss1.add_curve(sigma, area_renorm, curve_name, filename);

    // Setup input for adding a pcf.
    double rmin = 0.0;
    double rmax = 5.0;
    double dr = 0.02;
    double numberdensity = 0.4;
    std::pair<double,double> fit_interval(1.3, 3.0);
    int nbins = 250;
    std::pair<int,int> partial(0,1);
    std::string ref_path("./testfiles/gr_ref.data");
    sigma = 0.01;
    ss1.add_pcf(rmin, rmax, dr, sigma, numberdensity, fit_interval, nbins, partial, ref_path);
    sigma = 0.0123;
    ss1.add_pcf(rmin, rmax, dr, sigma, numberdensity, fit_interval, nbins, partial, ref_path);


    // Set the initial chi2_ value to some thing unreasonable.
    ss1.chi2_new_ = -1.123;
    // Call the setup function.
    ss1.from_sample_ = 0;
    ss1.from_basis_ = 12;
    ss1.setup();
    // Call notify.
    ss1.notify();

    // Check that the chi2 value has been updated.
    CPPUNIT_ASSERT_DOUBLES_EQUAL(735129583.538380026817, ss1.chi2_new_, 1.0e-8);
}


// -------------------------------------------------------------------------- //
//
void Test_Specswap::testAccept()
{
    // Setup a specswap object with many different data sets.
    std::string path("./testfiles/testlibLarge");
    int sample = 12;
    Mlrnd rand;
    rand.set_seed(39);
    Specswap ss1(sample, rand, path);

    // Mean scalar.
    std::string scalarname("VP-Volume");
    double target = 0.5;
    double sigma = 1.0;
    ss1.add_scalar_mean(scalarname, target, sigma);
    sigma = 0.0012;
    ss1.add_scalar_mean(scalarname, target, sigma);

    // Value scalar.
    scalarname = "VP-Area";
    double value_low = 0.5;
    double value_high = 1.0;
    double fraction = 0.6;
    sigma = 0.0001;
    ss1.add_scalar_value(scalarname, value_low, value_high, fraction, sigma);
    fraction = 0.1;
    sigma = 0.0001;
    ss1.add_scalar_value(scalarname, value_low, value_high, fraction, sigma);
    value_low = 0.0;
    value_high = 1.3;
    fraction = 0.99;
    sigma = 0.01;
    ss1.add_scalar_value(scalarname, value_low, value_high, fraction, sigma);

    // Scalar distribution.
    scalarname = "VP-Volume";
    std::string filename("./testfiles/vvol_ref.data");
    sigma = 0.0002;
    ss1.add_scalar_distribution(scalarname, filename, sigma);
    sigma = 0.0099;
    ss1.add_scalar_distribution(scalarname, filename, sigma);

    // Curve.
    std::string curve_name("bas");
    filename = "./testfiles/exafs_ref.data";
    sigma = 0.0002;
    bool area_renorm = true;
    ss1.add_curve(sigma, area_renorm, curve_name, filename);
    area_renorm = false;
    ss1.add_curve(sigma, area_renorm, curve_name, filename);

    // Setup input for adding a pcf.
    double rmin = 0.0;
    double rmax = 5.0;
    double dr = 0.02;
    double numberdensity = 0.4;
    std::pair<double,double> fit_interval(1.3, 3.0);
    int nbins = 250;
    std::pair<int,int> partial(0,1);
    std::string ref_path("./testfiles/gr_ref.data");
    sigma = 0.01;
    ss1.add_pcf(rmin, rmax, dr, sigma, numberdensity, fit_interval, nbins, partial, ref_path);
    sigma = 0.0123;
    ss1.add_pcf(rmin, rmax, dr, sigma, numberdensity, fit_interval, nbins, partial, ref_path);

    // Setup.
    ss1.setup();
    // Move.
    ss1.move();

    // Check what slot and indices will be swapped.
    CPPUNIT_ASSERT_EQUAL( ss1.from_sample_, 583);
    CPPUNIT_ASSERT_EQUAL( ss1.from_basis_, 2354);
    CPPUNIT_ASSERT_EQUAL( ss1.slot_, 11);

    // Check that the sample set is correct.
    CPPUNIT_ASSERT_EQUAL( ss1.sampleset_.index_at(ss1.slot_), ss1.from_sample_);

    // Notify.
    ss1.notify();

    // Check the chi2 value.
    double diff = 7.2603029936e+08 - 6.5624713898e-04 - ss1.chi2_;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( diff, 0.0, 1.0e-10 );

    // Check that the chi2_new value has been updated.
    diff = 7.4256423439e+08 + 1.9059181213e-03 - ss1.chi2_new_;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( diff, 0.0, 1.0e-10 );

    // Call the accept and make sure Chi2 has been accepted.
    ss1.accept();
    CPPUNIT_ASSERT_DOUBLES_EQUAL( ss1.chi2_, ss1.chi2_new_, 1.0e-10 );

    // Check that the sample set is correct.
    CPPUNIT_ASSERT_EQUAL( ss1.sampleset_.index_at(ss1.slot_), ss1.from_basis_);
}


// -------------------------------------------------------------------------- //
//
void Test_Specswap::testCollectWeights()
{
    // Setup a specswap intance.
    int sample = 3;
    Mlrnd rand;
    rand.set_seed(123);
    std::string path("./testfiles/testlibSmall");
    Specswap ss(sample, rand, path);
    ss.setup();

    // Check that the weights are zero.
    for (size_t i = 0; i < ss.weights_.size(); ++i)
    {
        CPPUNIT_ASSERT_EQUAL( ss.weights_[i], 0 );
    }

    // Check the initial nprobe_ value.
    CPPUNIT_ASSERT_EQUAL( ss.nprobe_, 0 );

    // Collect the weights.
    ss.collect_weights();

    // Check that the weights were correctly updated.
    std::vector<int> indices = ss.sampleset_.get_indices();
    for (size_t i = 0; i < indices.size(); ++i)
    {
        CPPUNIT_ASSERT_EQUAL( ss.weights_[indices[i]], 1 );
    }

    // All done.
}


// -------------------------------------------------------------------------- //
//
void Test_Specswap::testCalcScalarPos()
{
    // Setup a specswap object.
    int sample = 250;
    Mlrnd rand;
    std::string path("./testfiles/testlibLarge");
    Specswap ss(sample, rand, path);

    // Test against known values..
    CPPUNIT_ASSERT_EQUAL( ss.calc_scalar_pos("VP-Volume"), 1);
    CPPUNIT_ASSERT_EQUAL( ss.calc_scalar_pos("VP-Area"), 0);
    CPPUNIT_ASSERT_EQUAL( ss.calc_scalar_pos("Tetrahedrality"), 3);
    // Test that it fails with a wrong name.
    CPPUNIT_ASSERT_THROW( ss.calc_scalar_pos("NOTANAME"), IOException );
}


// -------------------------------------------------------------------------- //
//
void Test_Specswap::testRandom()
{
    // Setup a specswap intance.
    int sample = 3;
    Mlrnd rand;
    rand.set_seed(683);
    std::string path("./testfiles/testlibSmall");
    Specswap ss(sample, rand, path);
    ss.setup();

    // Check that we can call the random funcion.
    const int samples = 1000000;
    double min = 10;
    double max = -10;
    double sum = 0.0;
    for (int i = 0; i < samples; ++i)
    {
        // Get a random number.
        const double rnd = ss.random();

        // Check boundaries.
        CPPUNIT_ASSERT( rnd >= 0.0 );
        CPPUNIT_ASSERT( rnd <  1.0 );

        if (rnd < min)
        {
            min = rnd;
        }

        if (rnd > max)
        {
            max = rnd;
        }

        sum += (0.5-rnd)*(0.5-rnd);

    }

    CPPUNIT_ASSERT( std::fabs(max - 1.0) < 1.0e-5 );
    CPPUNIT_ASSERT( std::fabs(min)       < 1.0e-5 );
    CPPUNIT_ASSERT( sum/samples          < 0.1    );
}


// -------------------------------------------------------------------------- //
//
void Test_Specswap::testRandomBasis()
{
    // Setup a specswap intance.
    int sample = 3;
    Mlrnd rand;
    rand.set_seed(9999997);
    std::string path("./testfiles/testlibSmall");
    Specswap ss(sample, rand, path);
    ss.setup();

    // Check that we can call the random basis function.
    // it should return a random number between 0 and
    // nbase-1.
    const int nbase = ss.weights_.size();
    const int samples = 500000;
    // Setup a map of the random distribution.
    std::vector<int> distribution(nbase,0);
    int min = nbase;
    int max = 0;
    for (int i = 0; i < samples; ++i)
    {
        // Get a random basis.
        const int rnd = ss.random_basis();

        // Check boundaries.
        CPPUNIT_ASSERT( rnd >= 0 );
        CPPUNIT_ASSERT( rnd < nbase );

        // Increment the distribution at this point.
        ++distribution[rnd];

        if (rnd < min)
        {
            min = rnd;
        }

        if (rnd > max)
        {
            max = rnd;
        }
    }

    CPPUNIT_ASSERT_EQUAL( min, 0 );
    CPPUNIT_ASSERT_EQUAL( max, nbase-1 );

    // Loop through the distribution and find the variance from mean.
    int sum = 0;
    for (size_t i = 0; i < distribution.size(); ++i)
    {
        distribution[i];
        sum += (samples/nbase - distribution[i])*(samples/nbase - distribution[i]);
    }
}


// -------------------------------------------------------------------------- //
//
void Test_Specswap::testPrepareForAnalysis()
{
    // Setup a specswap intance.
    int sample = 3;
    Mlrnd rand;
    rand.set_seed(123);
    std::string path("./testfiles/testlibSmall");
    Specswap ss(sample, rand, path);
    ss.setup();

    // Check that the weights table is not setup yet.
    CPPUNIT_ASSERT( ss.weights_table_.empty() );

    // This fails if there are no weights.
    CPPUNIT_ASSERT_THROW( ss.prepare_for_analysis(), IOException );

    // Set the weights with something.
    for (size_t i = 0; i < ss.weights_.size(); ++i)
    {
        ss.weights_[i] = static_cast<int>(i*1.3+0.006*i*i*i-0.0045*i*i*i*i*i);
    }

    // Call the prepare for analysis function.
    ss.prepare_for_analysis();

    // Check that the weights table is not setup yet.
    CPPUNIT_ASSERT( !ss.weights_table_.empty() );

    // Take a copy of the weights table.
    const std::vector<BasisContainer> table_copy = ss.weights_table_;

    // Set the weights with something else.
    for (size_t i = 0; i < ss.weights_.size(); ++i)
    {
        ss.weights_[i] = 2;
    }

    // Check that the weights table was not changed.
    for (size_t i = 0; i < ss.weights_table_.size(); ++i)
    {
        CPPUNIT_ASSERT_EQUAL( ss.weights_table_[i].index,  table_copy[i].index );
        CPPUNIT_ASSERT_EQUAL( ss.weights_table_[i].weight, table_copy[i].weight );
        CPPUNIT_ASSERT_EQUAL( ss.weights_table_[i].name,   table_copy[i].name );
    }
}
