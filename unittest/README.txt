TO SETUP THE TEST ENVIRONMENT:
------------------------------
cd cppunit
tar -xzvf cppunit-1.12.1.tar.gz
cd cppunit-1.12.1
./configure --prefix=/your/absolute/path/unittest/cppunit/
make
make check
make install
------------
Now make and run the tests by typing
'make'
in the unittest directory.

If the linker complains about
/usr/lib64/libcppunit.so: undefined reference to `dlsym'
/usr/lib64/libcppunit.so: undefined reference to `dlopen'
/usr/lib64/libcppunit.so: undefined reference to `dlclose'

or similar you have to set the LDFLAGS to
LDFLAGS="-Wl,-rpath,/usr/lib -ldl"
before you build cppunit

And you will probably also have to run the tests with
the cppunit library in your LD_LIBRARY_PATH
LD_LIBRARY_PATH="LD_LIBRARY_PATH:./cppunit/lib/" ./run_tests.x


TO ADD TESTS FOR A NEW CLASS/FILE:
----------------------------------
1) Setup a new test header file myclass_test.h, see e.g. test_specswap.h.

2) Implement the tests in a file myclass_test.cpp, see e.g. test_specswap.cpp.

3) Add the test in the testRunner.h file.

The test will now be compiled and included in the test runner executable
when typing
'make'
in the unitteest directory.

