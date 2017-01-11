#set size ratio -1
#set view equal xyz
set view 70,30,1.3

set ticslevel 0.0

#splot "art.dat" using 2:4:3 w l lw 2 title ""
splot [] [] [] "earth.dat" using 1:3:2 w l lt 2 lw 2 title "Earth" 
replot "trajs.dat" using 2:4:3 w l lt 3 lw 2 title "Trajectory" 

pause 1000
