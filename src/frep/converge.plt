#set logscale x
set logscale y
plot [5:26] "conv0.dat" w l lw 2 t "even"
replot "conv1.dat" w l lw 2 t "odd"

pause 1000
