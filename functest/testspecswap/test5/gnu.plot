set terminal postscript eps enhanced color solid "Times-Roman" 26

set grid

set output "plot1.eps"
plot [1.3:5][:] \
 "results/stop.sample_pcfdata0_partial_0_1.pcf"  u ($1):($2) title "histogram"      w l lt 1\
,"results/stop.sample_pcfdata0_partial_0_1.pcf"  u ($1):($3) title "pcf"            w l lt 2\
,"results/start.sample_pcfdata0_partial_0_1.pcf" u ($1):($3) title "pcf -start"     w l lt 5\
,"results/stop.sample_pcfdata0_partial_0_1.pcf"  u ($1):($4) title "reference"      w l lt 3\
,"results/stop.sample_pcfdata0_partial_0_1.pcf"  u ($1):($5) title "normalization"  w l lt 4\


set output "plot2.eps"
plot [1.5:10][:] \
 "results/stop.sample_pcfdata1_partial_0_0.pcf"  u ($1):($2) title "histogram"      w l lt 1\
,"results/stop.sample_pcfdata1_partial_0_0.pcf"  u ($1):($3) title "pcf"            w l lt 2\
,"results/start.sample_pcfdata1_partial_0_0.pcf" u ($1):($3) title "pcf -start"     w l lt 5\
,"results/stop.sample_pcfdata1_partial_0_0.pcf"  u ($1):($4) title "reference"      w l lt 3\
,"results/stop.sample_pcfdata1_partial_0_0.pcf"  u ($1):($5) title "normalization"  w l lt 4\


set output "plot3.eps"
plot [1.3:5][:] \
 "ANALYSIS/t5_weighted.ANALYSIS_partial_0_0.pcf"  u ($1):($2) title "histogram"      w l lt 1\
,"ANALYSIS/t5_weighted.ANALYSIS_partial_0_0.pcf"  u ($1):($3) title "unewighted"     w l lt 2\
,"ANALYSIS/t5_weighted.ANALYSIS_partial_0_0.pcf"  u ($1):($4) title "weighted"       w l lt 5\
,"ANALYSIS/t5_weighted.ANALYSIS_partial_0_0.pcf"  u ($1):($5) title "reference"      w l lt 3\
,"ANALYSIS/t5_weighted.ANALYSIS_partial_0_0.pcf"  u ($1):($6) title "normalization"  w l lt 4\


set output "plot4.eps"
plot [:10][:] \
 "ANALYSIS/t5_weighted.ANALYSIS_partial_0_0.pcf"  u ($1):($2) title "histogram"      w l lt 1\
,"ANALYSIS/t5_weighted.ANALYSIS_partial_0_0.pcf"  u ($1):($3) title "unewighted"     w l lt 2\
,"ANALYSIS/t5_weighted.ANALYSIS_partial_0_0.pcf"  u ($1):($4) title "weighted"       w l lt 5\
,"ANALYSIS/t5_weighted.ANALYSIS_partial_0_0.pcf"  u ($1):($5) title "reference"      w l lt 3\
,"ANALYSIS/t5_weighted.ANALYSIS_partial_0_0.pcf"  u ($1):($6) title "normalization"  w l lt 4\


set output "plot5.eps"
plot [:][:] \
 "ANALYSIS/t5_weighted.ANALYSIS.bas"  u ($1):($2) title "unweighted"     w l lt 2\
,"ANALYSIS/t5_weighted.ANALYSIS.bas"  u ($1):($3) title "weighted"       w l lt 5\
,"ANALYSIS/t5_weighted.ANALYSIS.bas"  u ($1):($4) title "reference"      w l lt 3\


set output "plot6.eps"
plot [:][:] \
 "ANALYSIS/t5_chunk0.ANALYSIS.bas"  u ($1):($2) title "chunk0"       w l lt 1\
,"ANALYSIS/t5_chunk1.ANALYSIS.bas"  u ($1):($2) title "chunk1"       w l lt 2\
,"ANALYSIS/t5_chunk2.ANALYSIS.bas"  u ($1):($2) title "chunk2"       w l lt 4\
,"ANALYSIS/t5_chunk3.ANALYSIS.bas"  u ($1):($2) title "chunk3"       w l lt 5\
,"ANALYSIS/t5_chunk4.ANALYSIS.bas"  u ($1):($2) title "chunk4"       w l lt 6\
,"ANALYSIS/t5_chunk5.ANALYSIS.bas"  u ($1):($2) title "chunk5"       w l lt 9\
,"ANALYSIS/t5_chunk0.ANALYSIS.bas"  u ($1):($4) title "reference"    w l lt 7 lw 3\
