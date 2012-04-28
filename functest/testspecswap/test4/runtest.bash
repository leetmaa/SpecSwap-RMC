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
${prog} test4 > test4.stdout
echo "DONE"

# Create the results directory.
mkdir ${resdir}
# Moove all results files to this dir.
mv test4.stdout    ${resdir}
mv test4.log       ${resdir}
mv start.sample_curvedata0.XAS.DKSfwhm ${resdir}
mv stop.sample_curvedata0.XAS.DKSfwhm  ${resdir}
mv ice4_0._curvedata0.XAS.DKSfwhm ${resdir}
mv ice4_1._curvedata0.XAS.DKSfwhm ${resdir}
mv ice4_2._curvedata0.XAS.DKSfwhm ${resdir}
mv ice4_3._curvedata0.XAS.DKSfwhm ${resdir}
mv ice4.restart ${resdir}

echo "TESTING..."

# Compare log info.
logdiff=$( diff ${resdir}/test4.log ./references/test4.log )

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
samplediff=$( diff ${resdir}/start.sample_curvedata0.XAS.DKSfwhm ./references/start.sample_curvedata0.XAS.DKSfwhm )

# Test that the logs are identical.
echo "Printing the start.sampleXXX diff... (this should be empty)"
echo $samplediff
echo "done"
if [[ $( echo $samplediff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Sample start missmatch!!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi


samplediff=$( diff ${resdir}/stop.sample_curvedata0.XAS.DKSfwhm ./references/stop.sample_curvedata0.XAS.DKSfwhm )

echo "Printing the stop.sampleXXX diff... (this should be empty)"
echo $samplediff
echo "done"
if [[ $( echo $samplediff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Sample stop missmatch. !!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi


samplediff=$( diff ${resdir}/ice4_2._curvedata0.XAS.DKSfwhm ./references/ice4_2._curvedata0.XAS.DKSfwhm )

echo "Printing the ice4_2.XAS.DKSfwhm diff... (this should be empty)"
echo $samplediff
echo "done"
if [[ $( echo $samplediff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Sample 5 missmatch.   !!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi

# Comare the restart files.
restartdiff=$( diff ${resdir}/ice4.restart ./references/ice4.restart )

echo "Printing the restart diff... (this should be empty)"
echo $restartdiff
echo "done"
if [[ $( echo $restartdiff ) ]]; then
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
    echo " !!! Error. Restart missmatch.    !!!"
    echo " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
fi


# Compare output info.
stdoutdiff=$( diff ${resdir}/test4.stdout ./references/test4.stdout )

# The stdout should differe in the time stamps only.
echo "Printing the stdout diff."
echo "This should ONLY differ in the timestamps:"
echo ""
echo $stdoutdiff
echo "------------------------------------------"
echo ""
echo "DONE"
