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
${prog} test6 > test6.stdout
echo "DONE"

# Create the results directory.
mkdir ${resdir}

# Move all results files to this dir.
mv test6.stdout    ${resdir}
mv test6.log       ${resdir}
mv test6_weights_and_names.ANALYSIS   ${resdir}

echo "TESTING..."

# Compare ANALYSIS output.

filename="test6_chunk2.ANALYSIS.Distribution-VP-Volume"
analysisdiff=$( diff ANALYSIS/${filename} ./references/${filename} )

# The stdout should differe in the time stamps only.
echo "Printing chunk distribution diff."
echo ""
echo $analysisdiff
echo "------------------------------------------"
echo ""
echo "DONE"


filename="test6_chunk2.ANALYSIS.Mean-Tetrahedrality"
analysisdiff=$( diff ANALYSIS/${filename} ./references/${filename} )

# The stdout should differe in the time stamps only.
echo "Printing chunk mean diff."
echo ""
echo $analysisdiff
echo "------------------------------------------"
echo ""
echo "DONE"


filename="test6_chunk2.ANALYSIS.Value-VP-Volume"
analysisdiff=$( diff ANALYSIS/${filename} ./references/${filename} )

# The stdout should differe in the time stamps only.
echo "Printing chunk value diff."
echo ""
echo $analysisdiff
echo "------------------------------------------"
echo ""
echo "DONE"


filename="test6_chunk2.ANALYSIS_partial_0_0.pcf"
analysisdiff=$( diff ANALYSIS/${filename} ./references/${filename} )

# The stdout should differe in the time stamps only.
echo "Printing chunk partial diff."
echo ""
echo $analysisdiff
echo "------------------------------------------"
echo ""
echo "DONE"


filename="test6_chunk2.ANALYSIS_partial_0_1.pcf"
analysisdiff=$( diff ANALYSIS/${filename} ./references/${filename} )

# The stdout should differe in the time stamps only.
echo "Printing chunk partial diff."
echo ""
echo $analysisdiff
echo "------------------------------------------"
echo ""
echo "DONE"


filename="test6_chunk2.ANALYSIS.bas"
analysisdiff=$( diff ANALYSIS/${filename} ./references/${filename} )

# The stdout should differe in the time stamps only.
echo "Printing chunk curve diff."
echo ""
echo $analysisdiff
echo "------------------------------------------"
echo ""
echo "DONE"

# -------------------------------------------------------------


filename="test6_chunk3.ANALYSIS.Distribution-VP-Volume"
analysisdiff=$( diff ANALYSIS/${filename} ./references/${filename} )

# The stdout should differe in the time stamps only.
echo "Printing chunk distribution diff."
echo ""
echo $analysisdiff
echo "------------------------------------------"
echo ""
echo "DONE"


filename="test6_chunk3.ANALYSIS.Mean-Tetrahedrality"
analysisdiff=$( diff ANALYSIS/${filename} ./references/${filename} )

# The stdout should differe in the time stamps only.
echo "Printing chunk mean diff."
echo ""
echo $analysisdiff
echo "------------------------------------------"
echo ""
echo "DONE"


filename="test6_chunk3.ANALYSIS.Value-VP-Volume"
analysisdiff=$( diff ANALYSIS/${filename} ./references/${filename} )

# The stdout should differe in the time stamps only.
echo "Printing chunk value diff."
echo ""
echo $analysisdiff
echo "------------------------------------------"
echo ""
echo "DONE"


filename="test6_chunk3.ANALYSIS_partial_0_0.pcf"
analysisdiff=$( diff ANALYSIS/${filename} ./references/${filename} )

# The stdout should differe in the time stamps only.
echo "Printing chunk partial diff."
echo ""
echo $analysisdiff
echo "------------------------------------------"
echo ""
echo "DONE"


filename="test6_chunk3.ANALYSIS_partial_0_1.pcf"
analysisdiff=$( diff ANALYSIS/${filename} ./references/${filename} )

# The stdout should differe in the time stamps only.
echo "Printing chunk partial diff."
echo ""
echo $analysisdiff
echo "------------------------------------------"
echo ""
echo "DONE"


filename="test6_chunk3.ANALYSIS.bas"
analysisdiff=$( diff ANALYSIS/${filename} ./references/${filename} )

# The stdout should differe in the time stamps only.
echo "Printing chunk curve diff."
echo ""
echo $analysisdiff
echo "------------------------------------------"
echo ""
echo "DONE"


# Compare output info.

analysisdiff=$( diff ${resdir}/test6_weights_and_names.ANALYSIS ./references/test6_weights_and_names.ANALYSIS )

# The stdout should differe in the time stamps only.
echo "Printing the weights and names diff."
echo ""
echo $analysisdiff
echo "------------------------------------------"
echo ""
echo "DONE"

# ------------------

stdoutdiff=$( diff ${resdir}/test6.stdout ./references/test6.stdout )

# The stdout should differe in the time stamps only.
echo "Printing the stdout diff."
echo "This should ONLY differ in the timestamps:"
echo ""
echo $stdoutdiff
echo "------------------------------------------"
echo ""
echo "DONE"
