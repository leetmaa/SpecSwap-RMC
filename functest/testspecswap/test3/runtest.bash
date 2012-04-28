#!/bin/bash

echo "TESTINFO:  ===================================================================="
cat TESTINFO
echo "==============================================================================="
echo ""

resdir="./results"

# Cleanup any previous run.
echo "CLEANUP..."
./cleanup.bash
echo "DONE"

# Set the executable.
prog="../../../src/SpecSwapRMC_1.0a0.x"

# Run the calculation.
echo "RUNNING SPECSWAP..."
${prog} test3 > test3.stdout
echo "DONE"

# Create the results directory.
mkdir ${resdir}
# Moove all results files to this dir.
mv test3.stdout    ${resdir}
mv test3.restart   ${resdir}
mv test3.log       ${resdir}
mv start.sample_curvedata0.bas ${resdir}
mv stop.sample_curvedata0.bas  ${resdir}
mv start.sample_curvedata1.bas ${resdir}
mv stop.sample_curvedata1.bas  ${resdir}
mv start.sample_curvedata2.bas ${resdir}
mv stop.sample_curvedata2.bas  ${resdir}

echo "TESTING..."

# Compare log info.
logdiff=$( diff ${resdir}/test3.log ./references/test3.log )

# Test that the logs are identical.
echo "Printing the log diff... (this should be empty)"
echo $logdiff
echo "done"
if [[ $( echo $logdiff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Log missmatch.  !!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi

# Compare the samplebas.
samplebasdiff=$( diff ${resdir}/start.sample_curvedata0.bas ./references/start.sample_curvedata0.bas )

# Test that the logs are identical.
echo "Printing the start.sample_curvedata0.bas diff... (this should be empty)"
echo $samplebasdiff
echo "done"
if [[ $( echo $samplebasdiff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Samplebas missmatch.  !!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi
# Compare the samplebas.
samplebasdiff=$( diff ${resdir}/start.sample_curvedata1.bas ./references/start.sample_curvedata1.bas )

# Test that the logs are identical.
echo "Printing the start.sample_curvedata1.bas diff... (this should be empty)"
echo $samplebasdiff
echo "done"
if [[ $( echo $samplebasdiff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Samplebas missmatch.  !!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi
# Compare the samplebas.
samplebasdiff=$( diff ${resdir}/start.sample_curvedata2.bas ./references/start.sample_curvedata2.bas )

# Test that the logs are identical.
echo "Printing the start.sample_curvedata2.bas diff... (this should be empty)"
echo $samplebasdiff
echo "done"
if [[ $( echo $samplebasdiff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Samplebas missmatch.  !!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi

# Compare the samplebas.
samplebasdiff=$( diff ${resdir}/stop.sample_curvedata0.bas ./references/stop.sample_curvedata0.bas )

# Test that the logs are identical.
echo "Printing the start.sample_curvedata0.bas diff... (this should be empty)"
echo $samplebasdiff
echo "done"
if [[ $( echo $samplebasdiff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Samplebas missmatch.  !!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi
# Compare the samplebas.
samplebasdiff=$( diff ${resdir}/stop.sample_curvedata1.bas ./references/stop.sample_curvedata1.bas )

# Test that the logs are identical.
echo "Printing the start.sample_curvedata0.bas diff... (this should be empty)"
echo $samplebasdiff
echo "done"
if [[ $( echo $samplebasdiff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Samplebas missmatch.  !!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi
# Compare the samplebas.
samplebasdiff=$( diff ${resdir}/stop.sample_curvedata2.bas ./references/stop.sample_curvedata2.bas )

# Test that the logs are identical.
echo "Printing the start.sample_curvedata2.bas diff... (this should be empty)"
echo $samplebasdiff
echo "done"
if [[ $( echo $samplebasdiff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Samplebas missmatch.  !!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi

# Compare output info.
stdoutdiff=$( diff ${resdir}/test3.stdout ./references/test3.stdout )

# The stdout should differe in the time stamps only.
echo "Printing the stdout diff."
echo "This should ONLY differ in the timestamps:"
echo ""
echo $stdoutdiff
echo "------------------------------------------"
echo ""
echo "DONE"
