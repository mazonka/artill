set samples 300

a(x) = 0.1
b(x) = x>1?0.3:0

plot [0:3] [0:0.5] 0.005+0.3/(1+exp(-25*(x-1))) w l lw 2 title "1/(1+exp(-x))"
#plot [0:3] [0:0.5] 0.005+0.3/(1+exp(-25*(x-1))) w l lw 2 title "a+b/x"

pause 1000
