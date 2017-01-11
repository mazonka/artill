#set size ratio -1

plot "trajs.dat" using 2:3 smooth bezier w l lw 2 title "Trajectory"
replot "earth.dat" w l lw 2 title "Earth"
#replot "art.dat" using 2:3

#R=6378137
#set parametric
#set trange [0:2*pi]
#fx(t) = R*cos(t)
#fy(t) = R*sin(t)-R
#replot fx(t),fy(t) lw 2

pause 1000
