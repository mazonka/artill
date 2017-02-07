#set size ratio -1

c(x)=1
plot [] [0:2.5] "trajs.dat" using 1:12 w l lw 2 t "Mach vs time"
replot c(x) w l lw 1 lt 3 t ""
replot "trajs.dat" using 1:($3/5000) w l lw 1 lt 2 t "Y/5000"
#replot "trajs.dat" using 1:(10000*$12) smooth bezier w l lw 2
#replot "earth.dat" w l lw 2 title "Earth"
#replot "art.dat" using 2:3

pause 1000
