This test is to make sure the "mlrmc" program can generate SpecSwap libraries,
and that the generated librarise are identical to the set of references.

The test unpacks the tar archives with raw library data, and compiles them with
the "mlrmc" program. Generated libraries and output are then compared to stored
references.

You need to build the "mklib" program before you run the test. Go the
the "../../src" directory and type "make mklib".

Run the test by executing the "runtest.bash" script. Clean up afterwards
with the "cleanup.bash" script.

The reference libraries in the reference directory are used for the mklib test,
but also for both the unittests and for the specswap functionality tests.

The data in these libraries are presented here as an illustrative example of
how to setup an use a library, and should not be trusted for any publications
or other scientiffic use.

Please see the README.txt file in the SpecSwap-RMC root directory for
further details.

