set style data lines
set ylabel '{/Symbol r}({/Symbol t})'
set xlabel '{/Symbol t}'
set term postscript eps enhanced color font "Helvetica,24"
set output 'pp-randpop.gnu.eps'
set key right bottom
set xrange [1:1.2550243902439022]
set yrange [0:1.01]
plot 'out_SAM_pp-randpop.gnu.dat' u ($1):($2)  title 'SAM' lw 20 lt 0 , 'out_SAMGAM_pp-randpop.gnu.dat' u ($1):($2)  title 'SAMGAM' lw 20 lt 1
set output 'pp-randpop.gnu_bar.eps'
unset key
set ylabel 'Area'
set xlabel 'Algorithm'
set autoscale
unset logscale x
set boxwidth 0.8
set style fill solid border -1
set xrange [1:5]
set yrange [0.5:1.005]
set xtics ('SAMGAM' 2, 'SAM' 4)
plot 'out_pp-randpop.gnu.dat' usi (2):2 title 'SAMGAM' w boxes lt 1, 'out_pp-randpop.gnu.dat' usi (4):1 title 'SAM' w boxes lt 0
