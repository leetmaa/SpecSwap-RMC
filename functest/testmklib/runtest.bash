#!/bin/bash

echo "TESTINFO:  ===================================================================="
cat TESTINFO
echo "==============================================================================="
echo ""

sleep 4

srcdir=../../src

# Extract the raw library data for the small library.
echo "extracting the raw library data..."
tar -xzvf testlib.tar.gz
echo "done"

sleep 4

# Compile the library.
echo "compiling the library..."
${srcdir}/mklib3.0.x -c testlib > output_testlib
echo "done"

sleep 4

# Extract the raw library data for the large library.
echo "extracting the raw library data..."
tar -xzvf testlib1.tar.gz
echo "done"

sleep 4

# Compile the library.
echo "compiling the library..."
${srcdir}/mklib3.0.x -c testlib1 > output_testlib1
echo "done"

sleep 4

# Extract the raw library data for the newer ice library.
echo "extracting the raw library data..."
tar -xzvf testlib2.tar.gz
echo "done"

sleep 4

# Compile the library.
echo "compiling the library..."
${srcdir}/mklib3.0.x -c testlib2 > output_testlib2
echo "done"

sleep 4

# Compare output log info.
logdiff=$( diff output_testlib ./references/output_testlib )

# Test that the logs are identical.
echo "Printing the log diff... (this should be empty)"
echo $logdiff
echo "done"
if [[ $( echo $logdiff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Logg missmatch. !!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi

# Test that the binary files are identical.
bindiff=$( diff testlib.library ./references/testlib.library )

echo "Printing the bin diff... (this should be empty)"
echo $bindiff
echo "done"
if [[ $( echo $bindiff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Binary missmatch. !!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi

# Compare output log info.
logdiff=$( diff output_testlib1 ./references/output_testlib1 )

# Test that the logs are identical.
echo "Printing the log diff... (this should be empty)"
echo $logdiff
echo "done"
if [[ $( echo $logdiff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Logg missmatch. !!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi

# Test that the binary files are identical.
bindiff=$( diff testlib1.library ./references/testlib1.library )

echo "Printing the bin diff... (this should be empty)"
echo $bindiff
echo "done"
if [[ $( echo $bindiff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Binary missmatch. !!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi

# Compare output log info.
logdiff=$( diff output_testlib2 ./references/output_testlib2 )

# Test that the logs are identical.
echo "Printing the log diff... (this should be empty)"
echo $logdiff
echo "done"
if [[ $( echo $logdiff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Logg missmatch. !!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi

# Test that the binary files are identical.
bindiff=$( diff testlib2.library ./references/testlib2.library )

echo "Printing the bin diff... (this should be empty)"
echo $bindiff
echo "done"
if [[ $( echo $bindiff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Binary missmatch. !!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi

