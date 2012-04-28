set terminal postscript eps enhanced color solid "Times-Roman" 26

set grid

set output "plot1.eps"
plot [532.0:548.0][:] \
 "ANALYSIS/analysis_run_ice4_weighted.ANALYSIS.XAS.DKSfwhm"  u ($1):($2) title "un-weighted" w l lw 3 lt 1\
,"ANALYSIS/analysis_run_ice4_weighted.ANALYSIS.XAS.DKSfwhm"  u ($1):($3) title "weighted"    w l lw 3 lt 2\
,"ANALYSIS/analysis_run_ice4_weighted.ANALYSIS.XAS.DKSfwhm"  u ($1):($4) title "reference"   w l lw 3 lt 5\


set output "plot2.eps"
plot [532.0:548.0][:] \
 "ANALYSIS/analysis_run_ice4_chunk0.ANALYSIS.XAS.DKSfwhm"    u ($1):($4)  title "reference"  w l lw 3 lt 7\
,"ANALYSIS/analysis_run_ice4_chunk0.ANALYSIS.XAS.DKSfwhm"    u ($1):($2)  title "chunk0"  w l lw 1 lt 1\
,"ANALYSIS/analysis_run_ice4_chunk1.ANALYSIS.XAS.DKSfwhm"    u ($1):($2)  title "chunk1"  w l lw 1 lt 2\
,"ANALYSIS/analysis_run_ice4_chunk2.ANALYSIS.XAS.DKSfwhm"    u ($1):($2)  title "chunk2"  w l lw 1 lt 3\
,"ANALYSIS/analysis_run_ice4_chunk3.ANALYSIS.XAS.DKSfwhm"    u ($1):($2)  title "chunk3"  w l lw 1 lt 4\
,"ANALYSIS/analysis_run_ice4_chunk4.ANALYSIS.XAS.DKSfwhm"    u ($1):($2)  title "chunk4"  w l lw 1 lt 5\
,"ANALYSIS/analysis_run_ice4_chunk5.ANALYSIS.XAS.DKSfwhm"    u ($1):($2)  title "chunk5"  w l lw 1 lt 6\
,"ANALYSIS/analysis_run_ice4_chunk6.ANALYSIS.XAS.DKSfwhm"    u ($1):($2)  title "chunk6"  w l lw 1 lt 7\
,"ANALYSIS/analysis_run_ice4_chunk7.ANALYSIS.XAS.DKSfwhm"    u ($1):($2)  title "chunk7"  w l lw 1 lt 9\
,"ANALYSIS/analysis_run_ice4_chunk8.ANALYSIS.XAS.DKSfwhm"    u ($1):($2)  title "chunk8"  w l lw 1 lt 11\
,"ANALYSIS/analysis_run_ice4_chunk9.ANALYSIS.XAS.DKSfwhm"    u ($1):($2)  title "chunk9"  w l lw 1 lt 12\

