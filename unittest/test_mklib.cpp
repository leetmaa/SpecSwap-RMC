/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/


// Include the test definition.
#include "test_mklib.h"

// Include the files to test.
#include "../src/mklib.h"

// Other inclusion.
#include "../src/ioexception.h"
#include "../src/library.h"
#include "../src/ioutils.h"

// -------------------------------------------------------------------------- //
//
void Test_Mklib::testConstruction()
{
    CPPUNIT_ASSERT_NO_THROW(Mklib());
    CPPUNIT_ASSERT_NO_THROW(Mklib(false));
    CPPUNIT_ASSERT_NO_THROW(Mklib(true));
}

// -------------------------------------------------------------------------- //
//
void Test_Mklib::testRead()
{
    // Setup a 'debug' mklib.
    Mklib mklib(true);
    // Read the file that works.
    CPPUNIT_ASSERT_NO_THROW(mklib.library_from_info_file("dummy", "testfiles/testmklib/testmklibA.info"));
    CPPUNIT_ASSERT_NO_THROW(mklib.library_from_info_file("dummy", "testfiles/testmklib/testmklibB.info"));
}

// -------------------------------------------------------------------------- //
//
void Test_Mklib::testReadFail1()
{
    // Setup a 'debug' mklib.
    Mklib mklib(true);

    // The file to read.
    const std::string testfile("testfiles/testmklib/testmklib_empty.info");

    // Read the file that works.
    CPPUNIT_ASSERT_THROW( mklib.library_from_info_file("dummy", testfile), IOException );

    // Now we know it fails. Catch the error and check the message.
    try
    {
        mklib.library_from_info_file("dummy", testfile);
    }
    catch (IOException e)
    {
        // This is the message we should get.
        std::string ref_message("File 'testfiles/testmklib/testmklib_empty.info' is empty.\n");
        // Check that we got the right message.
        CPPUNIT_ASSERT_EQUAL(e.message(), ref_message);
    }
}

// -------------------------------------------------------------------------- //
//
void Test_Mklib::testReadFail2()
{
    // Setup a 'debug' mklib.
    Mklib mklib(true);

    // Read.
    const std::string testfile("testfiles/testmklib/testmklib_fail2.info");
    CPPUNIT_ASSERT_THROW( mklib.library_from_info_file("dummy", testfile), IOException );

    try
    {
        mklib.library_from_info_file("dummy", testfile);
    }
    catch (IOException e)
    {
        std::string ref_message("Error when reading from file: testfiles/testmklib/testmklib_fail2.info\nExpected : NCURVES\nFound    : N\n");

        //printf("%s\n", e.message().c_str());
        //error_exit("tests");
        // Check that we got the right message.
        CPPUNIT_ASSERT_EQUAL(e.message(), ref_message);
    }
}

// -------------------------------------------------------------------------- //
//
void Test_Mklib::testReadFail3()
{
    // Setup a 'debug' mklib.
    Mklib mklib(true);

    // Read.
    const std::string testfile("testfiles/testmklib/testmklib_fail3.info");
    CPPUNIT_ASSERT_THROW( mklib.library_from_info_file("dummy", testfile), IOException );

    try
    {
        mklib.library_from_info_file("dummy", testfile);
    }
    catch (IOException e)
    {
        std::string ref_message("Error when reading from file: testfiles/testmklib/testmklib_fail3.info\nExpected : CONVOLUTE\nFound    : ONVOLUTE\n");

        //printf("%s\n", e.message().c_str());
        //error_exit("tests");
        // Check that we got the right message.
        CPPUNIT_ASSERT_EQUAL(e.message(), ref_message);
    }
}

// -------------------------------------------------------------------------- //
//
void Test_Mklib::testReadFail4()
{
    // Setup a 'debug' mklib.
    Mklib mklib(true);

    // Read.
    const std::string testfile("testfiles/testmklib/testmklib_fail4.info");
    CPPUNIT_ASSERT_THROW( mklib.library_from_info_file("dummy", testfile), IOException );

    try
    {
        mklib.library_from_info_file("dummy", testfile);
    }
    catch (IOException e)
    {
        std::string ref_message("Error when reading from file: testfiles/testmklib/testmklib_fail4.info\nExpected : FORMAT\nFound    : FOAT\n");

        //printf("%s\n", e.message().c_str());
        //error_exit("tests");
        // Check that we got the right message.
        CPPUNIT_ASSERT_EQUAL(e.message(), ref_message);
    }
}

// -------------------------------------------------------------------------- //
//
void Test_Mklib::testReadFail5()
{
    // Setup a 'debug' mklib.
    Mklib mklib(true);

    // Read.
    const std::string testfile("testfiles/testmklib/testmklib_fail5.info");
    CPPUNIT_ASSERT_THROW( mklib.library_from_info_file("dummy", testfile), IOException );

    try
    {
        mklib.library_from_info_file("dummy", testfile);
    }
    catch (IOException e)
    {
        std::string ref_message("Error when reading from file: testfiles/testmklib/testmklib_fail5.info\nExpected : SCALE\nFound    : dummy_exafs_ref.data\n");

        //printf("%s\n", e.message().c_str());
        //error_exit("tests");
        // Check that we got the right message.
        CPPUNIT_ASSERT_EQUAL(e.message(), ref_message);
    }
}

// -------------------------------------------------------------------------- //
//
void Test_Mklib::testReadFail6()
{
    // Setup a 'debug' mklib.
    Mklib mklib(true);

    // Read.
    const std::string testfile("testfiles/testmklib/testmklib_fail6.info");
    CPPUNIT_ASSERT_THROW( mklib.library_from_info_file("dummy", testfile), IOException );

    try
    {
        mklib.library_from_info_file("dummy", testfile);
    }
    catch (IOException e)
    {
        std::string ref_message("Error when reading from file: testfiles/testmklib/testmklib_fail6.info\nExpected : ENDING\nFound    : DONGDONG\n");

        //printf("%s\n", e.message().c_str());
        //error_exit("tests");
        // Check that we got the right message.
        CPPUNIT_ASSERT_EQUAL(e.message(), ref_message);
    }
}

// -------------------------------------------------------------------------- //
//
void Test_Mklib::testReadFail7()
{
    // Setup a 'debug' mklib.
    Mklib mklib(true);

    // Read.
    const std::string testfile("testfiles/testmklib/testmklib_fail7.info");
    CPPUNIT_ASSERT_THROW( mklib.library_from_info_file("dummy", testfile), IOException );

    try
    {
        mklib.library_from_info_file("dummy", testfile);
    }
    catch (IOException e)
    {
        std::string ref_message("Error when reading from file: testfiles/testmklib/testmklib_fail7.info\nExpected : NSCALARS\nFound    : SCALAR\n");

        //printf("%s\n", e.message().c_str());
        //error_exit("tests");
        // Check that we got the right message.
        CPPUNIT_ASSERT_EQUAL(e.message(), ref_message);
    }
}

// -------------------------------------------------------------------------- //
//
void Test_Mklib::testReadFail8()
{
    // Setup a 'debug' mklib.
    Mklib mklib(true);

    // Read.
    const std::string testfile("testfiles/testmklib/testmklib_fail8.info");
    CPPUNIT_ASSERT_THROW( mklib.library_from_info_file("dummy", testfile), IOException );

    try
    {
        mklib.library_from_info_file("dummy", testfile);
    }
    catch (IOException e)
    {
        std::string ref_message("Error when reading from file: testfiles/testmklib/testmklib_fail8.info\nExpected : NAMES\nFound    : NAME\n");

        //printf("%s\n", e.message().c_str());
        //error_exit("tests");
        // Check that we got the right message.
        CPPUNIT_ASSERT_EQUAL(e.message(), ref_message);
    }
}

// -------------------------------------------------------------------------- //
//
void Test_Mklib::testReadFail9()
{
    // Setup a 'debug' mklib.
    Mklib mklib(true);

    // Read.
    const std::string testfile("testfiles/testmklib/testmklib_fail9.info");
    CPPUNIT_ASSERT_THROW( mklib.library_from_info_file("dummy", testfile), IOException );

    try
    {
        mklib.library_from_info_file("dummy", testfile);
    }
    catch (IOException e)
    {
        std::string ref_message("Error when reading from file: testfiles/testmklib/testmklib_fail9.info\nExpected : NATOMTYPES\nFound    : TYPES\n");

        //printf("%s\n", e.message().c_str());
        //error_exit("tests");
        // Check that we got the right message.
        CPPUNIT_ASSERT_EQUAL(e.message(), ref_message);
    }
}

// -------------------------------------------------------------------------- //
//
void Test_Mklib::testReadFail10()
{
    // Setup a 'debug' mklib.
    Mklib mklib(true);

    // Read.
    const std::string testfile("testfiles/testmklib/testmklib_fail10.info");
    CPPUNIT_ASSERT_THROW( mklib.library_from_info_file("dummy", testfile), IOException );

    try
    {
        mklib.library_from_info_file("dummy", testfile);
    }
    catch (IOException e)
    {
        std::string ref_message("Error when reading from file: testfiles/testmklib/testmklib_fail10.info\nExpected : ATOMS or START\nFound    : RUN\n");

        //printf("%s\n", e.message().c_str());
        //error_exit("tests");
        // Check that we got the right message.
        CPPUNIT_ASSERT_EQUAL(e.message(), ref_message);
    }
}

// -------------------------------------------------------------------------- //
//
void Test_Mklib::testReadFail11()
{
    // Setup a 'debug' mklib.
    Mklib mklib(true);

    // Read.
    const std::string testfile("testfiles/testmklib/testmklib_fail11.info");
    CPPUNIT_ASSERT_THROW( mklib.library_from_info_file("dummy", testfile), IOException );

    try
    {
        mklib.library_from_info_file("dummy", testfile);
    }
    catch (IOException e)
    {
        std::string ref_message("Error when reading from file: testfiles/testmklib/testmklib_fail11.info\nExpected : END\nFound    : STOP\n");

        //printf("%s\n", e.message().c_str());
        //error_exit("tests");
        // Check that we got the right message.
        CPPUNIT_ASSERT_EQUAL(e.message(), ref_message);
    }
}

// -------------------------------------------------------------------------- //
//
void Test_Mklib::testReadFail12()
{
    // Setup a 'debug' mklib.
    Mklib mklib(true);

    // Read.
    const std::string testfile("testfiles/testmklib/testmklib_fail12.info");
    CPPUNIT_ASSERT_THROW( mklib.library_from_info_file("dummy", testfile), IOException );

    try
    {
        mklib.library_from_info_file("dummy", testfile);
    }
    catch (IOException e)
    {
        std::string ref_message("Error when reading from file: testfiles/testmklib/testmklib_fail12.info\nExpected : NATOMTYPES\nFound    : 2\n");

        //printf("%s\n", e.message().c_str());
        //error_exit("tests");
        // Check that we got the right message.
        CPPUNIT_ASSERT_EQUAL(e.message(), ref_message);
    }
}

// -------------------------------------------------------------------------- //
//
void Test_Mklib::testReadFail13()
{
    // Setup a 'debug' mklib.
    Mklib mklib(true);

    // Read.
    const std::string testfile("testfiles/testmklib/testmklib_fail13.info");
    CPPUNIT_ASSERT_THROW( mklib.library_from_info_file("dummy", testfile), IOException );

    try
    {
        mklib.library_from_info_file("dummy", testfile);
    }
    catch (IOException e)
    {
        std::string ref_message("Format error or EOF when reading from: testfiles/testmklib/testmklib_fail13.info\n");

        //printf("%s\n", e.message().c_str());
        //error_exit("tests");
        // Check that we got the right message.
        CPPUNIT_ASSERT_EQUAL(e.message(), ref_message);
    }
}

// -------------------------------------------------------------------------- //
//
void Test_Mklib::testReadFail14()
{
    // Setup a 'debug' mklib.
    Mklib mklib(true);

    // Read.
    const std::string testfile("testfiles/testmklib/testmklib_fail14.info");
    CPPUNIT_ASSERT_THROW( mklib.library_from_info_file("dummy", testfile), IOException );

    try
    {
        mklib.library_from_info_file("dummy", testfile);
    }
    catch (IOException e)
    {
        std::string ref_message("Error when reading from file: testfiles/testmklib/testmklib_fail14.info\nExpected : CONVOLUTE\nFound    : NSCALARS\n");

        //printf("%s\n", e.message().c_str());
        //error_exit("tests");
        // Check that we got the right message.
        CPPUNIT_ASSERT_EQUAL(e.message(), ref_message);
    }
}


// -------------------------------------------------------------------------- //
//
void Test_Mklib::testReadFail15()
{
    // Setup a 'debug' mklib.
    Mklib mklib(true);

    // Read.
    const std::string testfile("testfiles/testmklib/testmklib_fail15.info");
    CPPUNIT_ASSERT_THROW( mklib.library_from_info_file("dummy", testfile), IOException );

    try
    {
        mklib.library_from_info_file("dummy", testfile);
    }
    catch (IOException e)
    {
        std::string ref_message("Error when reading from file: testfiles/testmklib/testmklib_fail15.info\nExpected : ATOMS or START\nFound    : TOMS\n");

        //printf("%s\n", e.message().c_str());
        //error_exit("tests");
        // Check that we got the right message.
        CPPUNIT_ASSERT_EQUAL(e.message(), ref_message);
    }
}

// -------------------------------------------------------------------------- //
//
void Test_Mklib::testReadFail16()
{
    // Setup a 'debug' mklib.
    Mklib mklib(true);

    // Read.
    const std::string testfile("testfiles/testmklib/testmklib_fail16.info");
    CPPUNIT_ASSERT_THROW( mklib.library_from_info_file("dummy", testfile), IOException );

    try
    {
        mklib.library_from_info_file("dummy", testfile);
    }
    catch (IOException e)
    {
        std::string ref_message("Error when reading from file: testfiles/testmklib/testmklib_fail16.info\nExpected : MOL\nFound    : 9\n");

        //printf("%s\n", e.message().c_str());
        //error_exit("tests");
        // Check that we got the right message.
        CPPUNIT_ASSERT_EQUAL(e.message(), ref_message);
    }
}

