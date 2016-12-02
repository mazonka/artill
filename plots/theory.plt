set samples 300

a(x) = 0.1
b(x) = x>1?0.3:0

plot [0:3] [0:0.5] a(x)+b(x)/x w l lw 2 title "Cd"
replot "155mm/a.dat" w l lw 2 t "M107"

pause 1000
