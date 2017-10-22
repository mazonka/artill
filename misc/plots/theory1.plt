set samples 300

a(x) = 0.1
b(x) = x>1?0.3:0

plot [0:3] [-0.05:0.5] 0.005+a(x)+b(x+0.01)/x w l lw 2 title "Cd"
replot a(x) lw 2
replot b(x) lw 2

pause 1000
