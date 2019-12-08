set style data lines
set ylabel '{/Symbol r}({/Symbol t})'
set xlabel '{/Symbol t}'
set term postscript eps enhanced color font "Helvetica,24"
set output 'pp-feaspop.gnu.eps'
set key right bottom
set xrange [1:28.30297297297297]
set yrange [0:1.01]
plot 'out_PM_pp-feaspop.gnu.dat' u ($1):($2)  title 'PM' lw 20 lt 0 , 'out_SAMFS_pp-feaspop.gnu.dat' u ($1):($2)  title 'SAMFS' lw 20 lt 1
set output 'pp-feaspop.gnu_bar.eps'
unset key
set ylabel 'Area'
set xlabel 'Algorithm'
set autoscale
unset logscale x
set boxwidth 0.8
set style fill solid border -1
set xrange [1:5]
set yrange [0.5:1.005]
set xtics ('PM' 2, 'SAMFS' 4)
plot 'out_pp-feaspop.gnu.dat' usi (2):1 title 'PM' w boxes lt 0, 'out_pp-feaspop.gnu.dat' usi (4):2 title 'SAMFS' w boxes lt 1
