/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/


// Include the test definition.
#include "test_library.h"

// Include the files to test.
#include "../src/library.h"

// Other inclusion.
#include "../src/ioexception.h"
#include "../src/mathutils.h"

// -------------------------------------------------------------------------- //
//
void Test_Library::testConstruction()
{
    CPPUNIT_ASSERT_NO_THROW(Library library);
    CPPUNIT_ASSERT_NO_THROW(Library library("testfiles/testlibSmall"));

    // Setup a library and check that the member data was set.
    {
        Library library;
        CPPUNIT_ASSERT( !library.use_atoms_info_ );
        CPPUNIT_ASSERT( !library.setup_done_ );
        CPPUNIT_ASSERT_EQUAL( library.nadded_scale_, 0 );
        CPPUNIT_ASSERT_EQUAL( library.nadded_,       0 );
        CPPUNIT_ASSERT_EQUAL( library.nbase_,        0 );
        CPPUNIT_ASSERT_EQUAL( library.ntypes_,       0 );
        CPPUNIT_ASSERT_EQUAL( library.npairs_,       0 );
        CPPUNIT_ASSERT_EQUAL( library.ncurves_,      0 );
        CPPUNIT_ASSERT_EQUAL( library.nscalars_,     0 );
    }

    {
        Library library("testfiles/testlibSmall");
        CPPUNIT_ASSERT( !library.use_atoms_info_ );
        CPPUNIT_ASSERT( library.setup_done_ );
        CPPUNIT_ASSERT_EQUAL( library.nadded_scale_,   0 );
        CPPUNIT_ASSERT_EQUAL( library.nadded_,       500 );
        CPPUNIT_ASSERT_EQUAL( library.nbase_,        500 );
        CPPUNIT_ASSERT_EQUAL( library.ntypes_,         2 );
        CPPUNIT_ASSERT_EQUAL( library.npairs_,         3 );
        CPPUNIT_ASSERT_EQUAL( library.ncurves_,        1 );
        CPPUNIT_ASSERT_EQUAL( library.nscalars_,       5 );
    }

}

// -------------------------------------------------------------------------- //
//
void Test_Library::testConstructionFail()
{
    CPPUNIT_ASSERT_THROW( Library library("testfiles/NOTAFILE"),    IOException );
    CPPUNIT_ASSERT_THROW( Library library("testfiles/NOTALIBRARY"), IOException );
    CPPUNIT_ASSERT_THROW( Library library("testfiles/EMPTY_FILE"),  IOException );
}

// -------------------------------------------------------------------------- //
//
void Test_Library::testName()
{
    Library library;
    library.name_ = "NONAME";
    library.set_name("NAME");
    CPPUNIT_ASSERT_EQUAL( library.name_, std::string("NAME") );
    CPPUNIT_ASSERT_EQUAL( library.name_, library.get_name() );

    // Make sure it fails if setup is done.
    library.setup_done_ = true;
    CPPUNIT_ASSERT_THROW(library.set_name("NAME"), IOException);
}

// -------------------------------------------------------------------------- //
//
void Test_Library::testNcurves()
{
    Library library;
    library.ncurves_ = -1;
    library.set_ncurves(12);
    CPPUNIT_ASSERT_EQUAL( library.ncurves_, 12 );
    CPPUNIT_ASSERT_EQUAL( library.ncurves_, library.get_ncurves() );

    // Make sure it fails if setup is done.
    library.setup_done_ = true;
    CPPUNIT_ASSERT_THROW(library.set_ncurves(12), IOException);
}

// -------------------------------------------------------------------------- //
//
void Test_Library::testNbase()
{
    Library library;
    library.nbase_ = -1;
    library.set_nbase(12);
    CPPUNIT_ASSERT_EQUAL( library.nbase_, 12 );
    CPPUNIT_ASSERT_EQUAL( library.nbase_, library.get_nbase() );

    // Make sure it fails if setup is done.
    library.setup_done_ = true;
    CPPUNIT_ASSERT_THROW(library.set_nbase(12), IOException);
}

// -------------------------------------------------------------------------- //
//
void Test_Library::testNscalars()
{
    Library library;
    library.nscalars_ = -1;
    library.set_nscalars(12);
    CPPUNIT_ASSERT_EQUAL( library.nscalars_, 12 );
    CPPUNIT_ASSERT_EQUAL( library.nscalars_, library.get_nscalars() );

    // Make sure it fails if setup is done.
    library.setup_done_ = true;
    CPPUNIT_ASSERT_THROW(library.set_nscalars(12), IOException);
}

// -------------------------------------------------------------------------- //
//
void Test_Library::testAtomsInfo()
{
    Library library;
    CPPUNIT_ASSERT( !library.use_atoms_info_ );

    // Setup the input.
    std::vector<int> atoms_per_type(3);
    std::vector<int> central_molecule(4);
    atoms_per_type[0] = 100;
    atoms_per_type[1] = 34;
    atoms_per_type[2] = 76;
    central_molecule[0] = 21;
    central_molecule[0] = 11;
    central_molecule[0] = 1;
    central_molecule[0] = 2;

    // Call.
    library.set_atoms_info(atoms_per_type, central_molecule);

    // Check.
    for (size_t i = 0; i < atoms_per_type.size(); ++i)
    {
        CPPUNIT_ASSERT_EQUAL( library.atoms_per_type_[i], atoms_per_type[i] );
    }
    for (size_t i = 0; i < central_molecule.size(); ++i)
    {
        CPPUNIT_ASSERT_EQUAL( library.central_molecule_[i], central_molecule[i] );
    }

    // Make sure it fails if setup is done.
    library.setup_done_ = true;
    CPPUNIT_ASSERT_THROW(library.set_atoms_info(atoms_per_type, central_molecule),
                         IOException);

}

// -------------------------------------------------------------------------- //
//
void Test_Library::testConvolute()
{
    Library library;
    library.add_convolute( 1.0,  2.0,  3.0,  4.0);
    library.add_convolute(11.0, 12.0, 13.0, 14.0);
    library.add_convolute(21.0, 22.0, 23.0, 24.0);
    //
    const double eps = 1.0e-12;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_convolute(0).no1, 1.0, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_convolute(0).no2, 2.0, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_convolute(0).no3, 3.0, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_convolute(0).no4, 4.0, eps);
    //
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_convolute(2).no1, 21.0, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_convolute(2).no2, 22.0, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_convolute(2).no3, 23.0, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_convolute(2).no4, 24.0, eps);

    // Should fail if called after setup is done.
    library.setup_done_ = true;
    CPPUNIT_ASSERT_THROW(library.add_convolute(21.0, 22.0, 23.0, 24.0),
                         IOException);
}

// -------------------------------------------------------------------------- //
//
void Test_Library::testFormat()
{
    Library library;
    library.add_format( 1.0,  2.0,  3.0);
    library.add_format(11.0, 12.0, 13.0);
    library.add_format(21.0, 22.0, 23.0);
    //
    const double eps = 1.0e-12;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_format(0).start, 1.0, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_format(0).stop,  2.0, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_format(0).step,  3.0, eps);
    //
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_format(2).start, 21.0, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_format(2).stop,  22.0, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_format(2).step,  23.0, eps);

    // Should fail if called after setup is done.
    library.setup_done_ = true;
    CPPUNIT_ASSERT_THROW(library.add_format(21.0, 22.0, 23.0),
                         IOException);
}

// -------------------------------------------------------------------------- //
//
void Test_Library::testScale()
{
    // FIXME : The file reading in library must be updated / fixed.

    Library library;
    library.name_ = "testfiles";
    std::string filename("scale_ref.data");

    // Setup.
    library.dimension_.push_back(6);
    library.step_.push_back(6);
    library.start_.push_back(0.1);
    library.stop_.push_back(10000.0);
    // Read.
    library.add_scale(filename);
    // Check.
    std::vector<double> scale = library.get_scale(0);
    const double eps = 1.0e-12;
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scale[0], 0.1, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scale[1], 1.0, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scale[2], 10.0, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scale[3], 100.0, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scale[4], 1000.0, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL(scale[5], 10000.0, eps );

    // Should fail if called after setup is done.
    library.setup_done_ = true;
    // Setup.
    library.dimension_.push_back(6);
    library.step_.push_back(6);
    library.start_.push_back(0.1);
    library.stop_.push_back(10000.0);
    // Read.
    CPPUNIT_ASSERT_THROW(library.add_scale(filename),
                         IOException);
}

// -------------------------------------------------------------------------- //
//
void Test_Library::testEnding()
{
    Library library;
    library.add_ending("A");
    library.add_ending("B");
    library.add_ending("C");

    CPPUNIT_ASSERT_EQUAL(library.get_ending(0), std::string("A"));
    CPPUNIT_ASSERT_EQUAL(library.get_ending(1), std::string("B"));
    CPPUNIT_ASSERT_EQUAL(library.get_ending(2), std::string("C"));

    // Should fail if called after setup is done.
    library.setup_done_ = true;
    CPPUNIT_ASSERT_THROW(library.add_ending("D"),
                         IOException);
}

// -------------------------------------------------------------------------- //
//
void Test_Library::testScalarName()
{
    Library library;
    library.add_scalar_name("A");
    library.add_scalar_name("B");
    library.add_scalar_name("C");

    CPPUNIT_ASSERT_EQUAL(library.get_scalar_name(0), std::string("A"));
    CPPUNIT_ASSERT_EQUAL(library.get_scalar_name(1), std::string("B"));
    CPPUNIT_ASSERT_EQUAL(library.get_scalar_name(2), std::string("C"));

    // Should fail if called after setup is done.
    library.setup_done_ = true;
    CPPUNIT_ASSERT_THROW(library.add_scalar_name("D"),
                         IOException);
}

// -------------------------------------------------------------------------- //
//
void Test_Library::testAtomTypes()
{
    Library library;
    std::vector<std::string> atomtypes;
    atomtypes.push_back("A");
    atomtypes.push_back("B");
    atomtypes.push_back("C");
    library.set_atomtypes(atomtypes);

    // Check
    CPPUNIT_ASSERT_EQUAL(library.atomtypes_[0], std::string("A"));
    CPPUNIT_ASSERT_EQUAL(library.atomtypes_[1], std::string("B"));
    CPPUNIT_ASSERT_EQUAL(library.atomtypes_[2], std::string("C"));

    // Should fail if called after setup is done.
    library.setup_done_ = true;
    CPPUNIT_ASSERT_THROW(library.set_atomtypes(atomtypes),
                         IOException);
}

// -------------------------------------------------------------------------- //
//
void Test_Library::testAddBase()
{
    // Setup.
    Library library;
    library.set_name("testfiles");

    // Data set only once.
    std::vector<std::string> atomtypes;
    atomtypes.push_back("O");
    atomtypes.push_back("H");
    library.set_atomtypes(atomtypes);
    library.set_ncurves(2);
    library.set_nscalars(5);
    library.set_nbase(2);

    // Data set for each curve.
    library.add_format(2.3, 7.65, 108);
    library.add_ending("bas");
    library.add_format(530.0, 545.0, 151.0);
    library.add_ending("abc");

    // Read in the files.
    library.add_base("testbase1");
    library.add_base("testbase2");

    // Check that every thing was read in as expected.
    const double eps = 1.0e-8;

    // Check the curves.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_at(0,0)[0],   0.03516950, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_at(0,0)[4],   0.70589900, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_at(0,0)[17], -0.00891369, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_at(0,0)[86], -0.18480400, eps );
    //
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_at(1,0)[0],   0.08707840, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_at(1,0)[4],   0.18923400, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_at(1,0)[17],  0.01546750, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_at(1,0)[86], -0.00190591, eps );
    //
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_at(0,1)[0],   0.00000000, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_at(0,1)[4],   0.00000000, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_at(0,1)[17],  0.00000000, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_at(0,1)[86],  6.07050930, eps );
    //
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_at(1,1)[53],  2.18500660, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_at(1,1)[86],  7.88945840, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_at(1,1)[99],  12.4494090, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_at(1,1)[104], 11.1735990, eps );

    // Check the geometry.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_geo(0)(0,0),  0.00000000, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_geo(0)(0,1),  0.00000000, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_geo(0)(0,2),  0.00000000, eps );
    //
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_geo(0)(24,0), 4.78829000, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_geo(0)(24,1),-1.28521000, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_geo(0)(24,2), 2.76465000, eps );
    //
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_geo(1)(24,0), 4.58121000, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_geo(1)(24,1), 2.42320000, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_geo(1)(24,2), 1.86724000, eps );
    //
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_geo(1)(251,0), -8.314590, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_geo(1)(251,1), -0.681770, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_geo(1)(251,2),  2.779230, eps );

    // Check the central molecule.
    CPPUNIT_ASSERT_EQUAL( library.get_central(0)[0], 0 );
    CPPUNIT_ASSERT_EQUAL( library.get_central(0)[1], 92);
    CPPUNIT_ASSERT_EQUAL( library.get_central(0)[2], 93);
    //
    CPPUNIT_ASSERT_EQUAL( library.get_central(1)[0], 2 );
    CPPUNIT_ASSERT_EQUAL( library.get_central(1)[1], 96);
    CPPUNIT_ASSERT_EQUAL( library.get_central(1)[2], 97);

    // Check partials.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_partial(0, 0, 0)[12], 4.59593525189379, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_partial(0, 0, 1)[23], 3.56597179444818, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_partial(0, 1, 1)[0],  2.23215666134794, eps);
    //
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_partial(1, 0, 0)[19], 5.27321754480128, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_partial(1, 0, 1)[78], 5.35939934262227, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_partial(1, 1, 1)[5],  2.60428412745230, eps);

    // Check that the partials are symmetric in indices.
    for (size_t i = 0; i < library.get_partial(0,0,1).size(); ++i)
    {
        CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_partial(0, 0, 1)[i],
                                      library.get_partial(0, 1, 0)[i],
                                      eps );
    }

    // Check internals.
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(library.get_internal(0, 0, 0).size()), 0 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(library.get_internal(0, 0, 1).size()), 2 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(library.get_internal(0, 1, 0).size()), 2 );
    CPPUNIT_ASSERT_EQUAL( static_cast<int>(library.get_internal(0, 1, 1).size()), 1 );
    //
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_internal(0, 0, 1)[0], 0.93665624057068, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_internal(1, 1, 0)[1], 1.10200879787777, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_internal(0, 1, 1)[0], 1.50652886799424, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_internal(1, 1, 1)[0], 1.81622017745647, eps);

    // Check the scalars.
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_scalar_at(0, 0), 53.459, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_scalar_at(0, 1), 28.230, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_scalar_at(0, 2), 1.6950, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_scalar_at(0, 3), 0.7640, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_scalar_at(0, 4),-32.159, eps);
    //
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_scalar_at(1, 0), 57.960, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_scalar_at(1, 1), 32.971, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_scalar_at(1, 2), 1.5840, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_scalar_at(1, 3), 0.7010, eps);
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_scalar_at(1, 4),-14.453, eps);

    // Check the names.
    CPPUNIT_ASSERT_EQUAL( library.get_basis_name(0), std::string("testbase1") );
    CPPUNIT_ASSERT_EQUAL( library.get_basis_name(1), std::string("testbase2") );

    // All done.
}

// -------------------------------------------------------------------------- //
//
void Test_Library::testCompleteSetup()
{
    Library library;
    CPPUNIT_ASSERT( !library.setup_done_ );
    library.complete_setup();
    CPPUNIT_ASSERT( library.setup_done_ );
}

// -------------------------------------------------------------------------- //
//
void Test_Library::testGetBasisName()
{
    Library library;
    library.names_.resize(12);
    library.names_[10] = "NAME";
    CPPUNIT_ASSERT_EQUAL( library.get_basis_name(10), std::string("NAME"));
}

// -------------------------------------------------------------------------- //
//
void Test_Library::testGetPartial()
{
    // Setup.
    Library library;
    int type1 = 0;
    int type2 = 1;
    int index = 2;
    // Need the index matrix to determine what data to return.
    library.ntypes_ = 2;
    library.index_matrix_ = setup_index_matrix(2);
    library.partials_.push_back(std::vector<std::vector<double> >(2, std::vector<double>(3,0.0)));
    library.partials_.push_back(std::vector<std::vector<double> >(2, std::vector<double>(3,0.0)));
    library.partials_.push_back(std::vector<std::vector<double> >(2, std::vector<double>(3,0.0)));
    library.partials_[2][1][0] = 1.0;
    library.partials_[2][1][1] = 1.1;
    library.partials_[2][1][2] = 1.2;

    // Call.
    std::vector<double> partial = library.get_partial(index, type1, type2);

    // Check.
    const double eps = 1.0e-12;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( partial[0], 1.0, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( partial[1], 1.1, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( partial[2], 1.2, eps );
}

// -------------------------------------------------------------------------- //
//
void Test_Library::testGetInternal()
{
    // Setup.
    Library library;
    int type1 = 0;
    int type2 = 1;
    int index = 2;
    // Need the index matrix to determine what data to return.
    library.ntypes_ = 2;
    library.index_matrix_ = setup_index_matrix(2);
    library.internals_.push_back(std::vector<std::vector<double> >(2, std::vector<double>(3,0.0)));
    library.internals_.push_back(std::vector<std::vector<double> >(2, std::vector<double>(3,0.0)));
    library.internals_.push_back(std::vector<std::vector<double> >(2, std::vector<double>(3,0.0)));
    library.internals_[2][1][0] = 1.0;
    library.internals_[2][1][1] = 1.1;
    library.internals_[2][1][2] = 1.2;

    // Call.
    std::vector<double> internal = library.get_internal(index, type1, type2);

    // Check.
    const double eps = 1.0e-12;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( internal[0], 1.0, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( internal[1], 1.1, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( internal[2], 1.2, eps );
}

// -------------------------------------------------------------------------- //
//
void Test_Library::testGetCentral()
{
    // Setup.
    Library library;
    library.central_mol_.push_back(std::vector<int>(1,8));
    library.central_mol_.push_back(std::vector<int>(1,3));
    library.central_mol_.push_back(std::vector<int>(1,9));
    // Check.
    CPPUNIT_ASSERT_EQUAL( library.get_central(0)[0], 8 );
    CPPUNIT_ASSERT_EQUAL( library.get_central(1)[0], 3 );
    CPPUNIT_ASSERT_EQUAL( library.get_central(2)[0], 9 );
}

// -------------------------------------------------------------------------- //
//
void Test_Library::testGetGeo()
{
    // Setup.
    Library library;

    Matrix geo0(2,3);
    geo0(1,2) = 8.0;
    library.geo_.push_back(geo0);

    Matrix geo1(2,3);
    geo1(1,0) = 9.0;
    library.geo_.push_back(geo1);

    // Check.
    CPPUNIT_ASSERT_EQUAL( library.get_geo(0)(1,2), 8.0 );
    CPPUNIT_ASSERT_EQUAL( library.get_geo(1)(1,0), 9.0 );
}

// -------------------------------------------------------------------------- //
//
void Test_Library::testGetScalarAt()
{
    // Setup.
    Library library;
    library.scalars_.push_back(std::vector<double>(3,0.0));
    library.scalars_.push_back(std::vector<double>(3,0.0));
    library.scalars_[0][2] = 3.2;
    library.scalars_[1][0] = 1.2;

    // Check.
    const double eps = 1.0e-12;
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_scalar_at(0,2), 3.2, eps );
    CPPUNIT_ASSERT_DOUBLES_EQUAL( library.get_scalar_at(1,0), 1.2, eps );
}
