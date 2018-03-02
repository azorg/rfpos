#!/bin/sh

gnuplot4-qt -p -c gnuplot.gp

#gnuplot <<EOF
#set grid
##unset border
#plot "data.txt" using (\$1) title "x" with lines, \
#     "data.txt" using (\$2) title "v" with lines, \
#     "data.txt" using (\$3) title "a" with lines, \
#     "data.txt" using (\$4) title "X" with lines, \
#     "data.txt" using (\$5) title "V" with lines
#pause -1
#EOF

