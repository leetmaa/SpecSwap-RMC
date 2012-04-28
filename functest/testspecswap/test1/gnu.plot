set terminal postscript eps enhanced color solid "Times-Roman" 26

set grid

set output "plot1.eps"
plot [:][:] \
 "results/start.sample_curvedata0.bas"  u ($1):($2) title "start"     w l lw 3 lt 1\
,"results/stop.sample_curvedata0.bas"   u ($1):($2) title "stop"      w l lw 3 lt 3\
,"results/start.sample_curvedata0.bas"  u ($1):($3) title "reference" w l lw 3 lt 7\
