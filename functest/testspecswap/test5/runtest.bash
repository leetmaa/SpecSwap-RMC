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
${prog} test5 > test5.stdout
echo "DONE"

# Create the results directory.
mkdir ${resdir}
# Moove all results files to this dir.
mv test5.stdout    ${resdir}
mv test5.log       ${resdir}
mv start.sample_curvedata0.bas ${resdir}
mv stop.sample_curvedata0.bas  ${resdir}
mv start.sample_pcfdata0_partial_0_1.pcf ${resdir}
mv start.sample_pcfdata1_partial_0_0.pcf ${resdir}
mv stop.sample_pcfdata0_partial_0_1.pcf  ${resdir}
mv stop.sample_pcfdata1_partial_0_0.pcf  ${resdir}
mv test5.restart                         ${resdir}
mv test5_0._curvedata0.bas               ${resdir}
mv test5_1._curvedata0.bas               ${resdir}
mv test5_2._curvedata0.bas               ${resdir}
mv test5_3._curvedata0.bas               ${resdir}
mv test5_4._curvedata0.bas               ${resdir}
mv test5_5._curvedata0.bas               ${resdir}
mv test5_6._curvedata0.bas               ${resdir}
mv test5_7._curvedata0.bas               ${resdir}
mv test5_0._pcfdata0_partial_0_1.pcf     ${resdir}
mv test5_0._pcfdata1_partial_0_0.pcf     ${resdir}
mv test5_1._pcfdata0_partial_0_1.pcf     ${resdir}
mv test5_1._pcfdata1_partial_0_0.pcf     ${resdir}
mv test5_2._pcfdata0_partial_0_1.pcf     ${resdir}
mv test5_2._pcfdata1_partial_0_0.pcf     ${resdir}
mv test5_3._pcfdata0_partial_0_1.pcf     ${resdir}
mv test5_3._pcfdata1_partial_0_0.pcf     ${resdir}
mv test5_4._pcfdata0_partial_0_1.pcf     ${resdir}
mv test5_4._pcfdata1_partial_0_0.pcf     ${resdir}
mv test5_5._pcfdata0_partial_0_1.pcf     ${resdir}
mv test5_5._pcfdata1_partial_0_0.pcf     ${resdir}
mv test5_6._pcfdata0_partial_0_1.pcf     ${resdir}
mv test5_6._pcfdata1_partial_0_0.pcf     ${resdir}
mv test5_7._pcfdata0_partial_0_1.pcf     ${resdir}
mv test5_7._pcfdata1_partial_0_0.pcf     ${resdir}

echo "TESTING..."


# Compare log info.
logdiff=$( diff ${resdir}/test5.log ./references/test5.log )

# Test that the logs are identical.
echo "Printing the log diff... (this should be empty)"
echo $logdiff
echo "done"
if [[ $( echo $logdiff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Log missmatch.  !!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi

# Compare the start and stop samples.
samplediff=$( diff ${resdir}/start.sample_curvedata0.bas ./references/start.sample_curvedata0.bas )

# Test that the logs are identical.
echo "Printing the start.sampleXXX diff... (this should be empty)"
echo $samplediff
echo "done"
if [[ $( echo $samplediff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Sample start missmatch!!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi

# Compare the start and stop samples.
samplediff=$( diff ${resdir}/stop.sample_curvedata0.bas ./references/stop.sample_curvedata0.bas )

# Test that the logs are identical.
echo "Printing the start.sampleXXX diff... (this should be empty)"
echo $samplediff
echo "done"
if [[ $( echo $samplediff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Sample stop missmatch !!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi


# Compare the start and stop samples.
samplediff=$( diff ${resdir}/start.sample_pcfdata0_partial_0_1.pcf ./references/start.sample_pcfdata0_partial_0_1.pcf )

# Test that the logs are identical.
echo "Printing the start.sampleXXX diff... (this should be empty)"
echo $samplediff
echo "done"
if [[ $( echo $samplediff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Sample start missmatch!!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi

# Compare the start and stop samples.
samplediff=$( diff ${resdir}/start.sample_pcfdata1_partial_0_0.pcf ./references/start.sample_pcfdata1_partial_0_0.pcf )

# Test that the logs are identical.
echo "Printing the start.sampleXXX diff... (this should be empty)"
echo $samplediff
echo "done"
if [[ $( echo $samplediff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Sample start missmatch !!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi

# Compare the start and stop samples.
samplediff=$( diff ${resdir}/stop.sample_pcfdata0_partial_0_1.pcf ./references/stop.sample_pcfdata0_partial_0_1.pcf )

# Test that the logs are identical.
echo "Printing the stop.sampleXXX diff... (this should be empty)"
echo $samplediff
echo "done"
if [[ $( echo $samplediff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Sample stop missmatch!!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi

# Compare the start and stop samples.
samplediff=$( diff ${resdir}/stop.sample_pcfdata1_partial_0_0.pcf ./references/stop.sample_pcfdata1_partial_0_0.pcf )

# Test that the logs are identical.
echo "Printing the stop.sampleXXX diff... (this should be empty)"
echo $samplediff
echo "done"
if [[ $( echo $samplediff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Sample stop missmatch !!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi

# Compare output info.
stdoutdiff=$( diff ${resdir}/test5.stdout ./references/test5.stdout )

# The stdout should differe in the time stamps only.
echo "Printing the stdout diff."
echo "This should ONLY differ in the timestamps:"
echo ""
echo $stdoutdiff
echo "------------------------------------------"
echo ""
echo "DONE"

