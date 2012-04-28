/*
    Part of the unit tests for the SpecSwap-RMC program.
    Copyright (c) 2008 - 2012  Mikael Leetmaa

    Distributed under the terms of the GNU General Public License version 3
    See the SpecSwap-RMC LICENSE file for full copying conditions.

    Author: Mikael Leetmaa   <leetmaa@fysik.su.se>
*/


// Include the test definition.
#include "test_ioutils.h"

// Include the files to test.
#include "../src/ioutils.h"

// Other inclusion.
#include "../src/ioexception.h"


// -------------------------------------------------------------------------- //
//
void Test_IOUtils::testToString()
{
    const std::string three = to_string(3);
    const std::string ref0("3");
    CPPUNIT_ASSERT_EQUAL( three, ref0 );

    const std::string minus_five = to_string(-5);
    const std::string ref1("-5");
    CPPUNIT_ASSERT_EQUAL( minus_five, ref1 );
}


// -------------------------------------------------------------------------- //
//
void Test_IOUtils::testNewlineIndent()
{
    const std::string original0("Here\nis\n   a linebreak\n");
    const std::string padded0 = newline_indent(original0, 3);
    const std::string ref0("Here\n   is\n      a linebreak\n   ");
    CPPUNIT_ASSERT_EQUAL( padded0, ref0 );

    const std::string original1("Here\nis\n   a linebreak\n");
    const std::string padded1 = newline_indent(original1, 5);
    const std::string ref1("Here\n     is\n        a linebreak\n     ");
    CPPUNIT_ASSERT_EQUAL( padded1, ref1 );
}


// -------------------------------------------------------------------------- //
//
void Test_IOUtils::testEOF()
{
    // Open an empty file.
    std::ifstream empty("./testfiles/EMPTY_FILE");
    // Check that this is at eof.
    CPPUNIT_ASSERT( eof(empty) );
    // Open a file which is not empty.
    std::ifstream file("./testfiles/NOT_A_LIBRARY.library");
    // Check that this is not at eof.
    CPPUNIT_ASSERT( !eof(file) );
    char tmp_char;
    while(!file.eof())
    {
        file.get(tmp_char);
    }
    // Check that this is not at eof.
    CPPUNIT_ASSERT( eof(file) );
}


// -------------------------------------------------------------------------- //
//
void Test_IOUtils::testCheckPath()
{
    // This should work.
    const std::string path0("./testfiles/NOT_A_LIBRARY.library");
    CPPUNIT_ASSERT_NO_THROW( check_path(path0) );

    // Make sure this fails.
    const std::string path1("./testfiles/NO_SUCH_FILE");
    CPPUNIT_ASSERT_THROW( check_path(path1), IOException );

    // Check the error.
    try
    {
        check_path(path1, "somewhere in a file");
    }
    catch (IOException e)
    {
        const std::string msg(e.what());
        const std::string ref("\n   IOException: somewhere in a file\n       Message: Could not open file: ./testfiles/NO_SUCH_FILE\n                \n");
        CPPUNIT_ASSERT_EQUAL( msg, ref );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_IOUtils::testCheckSigma()
{
    // This should not fail.
    CPPUNIT_ASSERT_NO_THROW( check_sigma(1.34) );

    // Make sure it fails.
    CPPUNIT_ASSERT_THROW( check_sigma(-1.334), IOException );

    // Check the error.
    try
    {
        check_sigma(-1.334, "somewhere in a file");
    }
    catch (IOException e)
    {
        const std::string msg(e.what());
        const std::string ref("\n   IOException: somewhere in a file\n       Message: The SIGMA value squared must not be negative.\n                \n");
        CPPUNIT_ASSERT_EQUAL( msg, ref );
    }

    // Make sure it fails.
    CPPUNIT_ASSERT_THROW( check_sigma(1.0e-10), IOException );

    // Check the error.
    try
    {
        check_sigma(1.0e-10, "at this location");
    }
    catch (IOException e)
    {
        const std::string msg(e.what());
        const std::string ref("\n   IOException: at this location\n       Message: The SIGMA value squared must be larget than 1.0e-10\n                \n");
        CPPUNIT_ASSERT_EQUAL( msg, ref );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_IOUtils::testCheckPositiveInteger()
{
    CPPUNIT_ASSERT_NO_THROW( check_positive_integer(1, "filename", "msg") );
    CPPUNIT_ASSERT_THROW( check_positive_integer(-1, "filename", "msg"), IOException );

    try
    {
        check_positive_integer(-1, "filename", "msg");
    }
    catch (IOException e)
    {
        const std::string msg(e.what());
        const std::string ref("\n   IOException: msg\n       Message: Expected an integer value >= 0, found -1\n                When reading file:filename\n                \n");
        CPPUNIT_ASSERT_EQUAL( msg, ref );
    }

}


// -------------------------------------------------------------------------- //
//
void Test_IOUtils::testOpenFileError()
{
    // Make sure this fails.
    CPPUNIT_ASSERT_THROW( open_file_error("filename", "location"), IOException );

    // Check the error.
    try
    {
        open_file_error("filename", "location");
    }
    catch (IOException e)
    {
        const std::string msg(e.what());
        const std::string ref("\n   IOException: location\n       Message: Could not open file: filename\n                \n");
        CPPUNIT_ASSERT_EQUAL( msg, ref );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_IOUtils::testEmptyFileError()
{
    // Make sure this fails.
    CPPUNIT_ASSERT_THROW( empty_file_error("filename", "location"), IOException );

    // Check the error.
    try
    {
        empty_file_error("filename", "location");
    }
    catch (IOException e)
    {
        const std::string msg(e.what());
        const std::string ref("\n   IOException: location\n       Message: File 'filename' is empty.\n                \n");
        CPPUNIT_ASSERT_EQUAL( msg, ref );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_IOUtils::testUnknownSectionError()
{
    // Make sure this fails.
    CPPUNIT_ASSERT_THROW( unknown_section_error("SECTION", "location"), IOException );

    // Check the error.
    try
    {
        unknown_section_error("SECTION", "location");
    }
    catch (IOException e)
    {
        const std::string msg(e.what());
        const std::string ref("\n   IOException: location\n       Message: Unknown section keyword: SECTION\n                \n");
        CPPUNIT_ASSERT_EQUAL( msg, ref );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_IOUtils::testSameSectionError()
{
    // Make sure this fails.
    CPPUNIT_ASSERT_THROW( same_section_error("SECTION", "location"), IOException );

    // Check the error.
    try
    {
        same_section_error("SECTION", "location");
    }
    catch (IOException e)
    {
        const std::string msg(e.what());
        const std::string ref("\n   IOException: location\n       Message: Section SECTION can only be defined once.\n                \n");
        CPPUNIT_ASSERT_EQUAL( msg, ref );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_IOUtils::testMissingKeywordError()
{
    // Make sure this fails.
    CPPUNIT_ASSERT_THROW( missing_keyword_error("THISKEY", "here"), IOException );

    // Check the error.
    try
    {
        missing_keyword_error("THISKEY", "here");
    }
    catch (IOException e)
    {
        const std::string msg(e.what());
        const std::string ref("\n   IOException: here\n       Message: Required keyword THISKEY not found.\n                \n");
        CPPUNIT_ASSERT_EQUAL( msg, ref );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_IOUtils::testSameKeywordError()
{
    // Make sure this fails.
    CPPUNIT_ASSERT_THROW( same_keyword_error("THISKEY", "here"), IOException );

    // Check the error.
    try
    {
        same_keyword_error("THISKEY", "here");
    }
    catch (IOException e)
    {
        const std::string msg(e.what());
        const std::string ref("\n   IOException: here\n       Message: Keyword THISKEY can only be definded once within this section.\n                \n");
        CPPUNIT_ASSERT_EQUAL( msg, ref );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_IOUtils::testUnknownKeywordError()
{
    // Make sure this fails.
    CPPUNIT_ASSERT_THROW( unknown_keyword_error("THISKEY", "here"), IOException );

    // Check the error.
    try
    {
        unknown_keyword_error("THISKEY", "here");
    }
    catch (IOException e)
    {
        const std::string msg(e.what());
        const std::string ref("\n   IOException: here\n       Message: Unknown keyword: THISKEY\n                \n");
        CPPUNIT_ASSERT_EQUAL( msg, ref );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_IOUtils::testReadKeywordError()
{
    // Make sure this fails.
    CPPUNIT_ASSERT_THROW( read_keyword_error("this.file", "EXPKEY", "THISKEY", "here"), IOException );

    // Check the error.
    try
    {
        read_keyword_error("this.file", "EXPKEY", "THISKEY", "here");
    }
    catch (IOException e)
    {
        const std::string msg(e.what());
        const std::string ref("\n   IOException: here\n       Message: Error when reading from file: this.file\n                Expected : EXPKEY\n                Found    : THISKEY\n                \n");
        CPPUNIT_ASSERT_EQUAL( msg, ref );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_IOUtils::testTimer()
{
    // Make a timestamp.
    timestamp();

    // Now, start the timer.
    start_timer();
    // Sleep for a while.
    sleep(3);
    // Check the time passed.
    CPPUNIT_ASSERT_EQUAL( 3, timer() );

    start_timer();
    sleep(2);
    CPPUNIT_ASSERT_EQUAL( 2, timer() );

    start_timer();
    sleep(1);
    CPPUNIT_ASSERT_EQUAL( 1, timer() );

    // Make a timestamp again.
    timestamp();
}


// -------------------------------------------------------------------------- //
//
void Test_IOUtils::testError()
{
    // Make sure this throws the expected error.
    CPPUNIT_ASSERT_THROW( error("msg", "location"), IOException );

    // Check the error.
    try
    {
        error("msg", "location");
    }
    catch (IOException e)
    {
        const std::string msg(e.what());
        const std::string ref("\n   IOException: location\n       Message: msg\n");
        CPPUNIT_ASSERT_EQUAL( msg, ref );
    }
}


// -------------------------------------------------------------------------- //
//
void Test_IOUtils::testEatline()
{
    // Open a containing four lines.
    std::ifstream file("./testfiles/NOT_A_LIBRARY.library");

    // Check that this is not at eof.
    CPPUNIT_ASSERT( !eof(file) );

    // Use the eat line function to test.
    eatline(file);
    CPPUNIT_ASSERT( !eof(file) );

    eatline(file);
    CPPUNIT_ASSERT( !eof(file) );

    eatline(file);
    CPPUNIT_ASSERT( !eof(file) );

    eatline(file);
    CPPUNIT_ASSERT( eof(file) );
}
