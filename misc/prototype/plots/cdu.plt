set samples 2000

s2(x)=3*x**2-2*x**3
s3(x)=6*x**5-15*x**4+10*x**3
s4(x)=-20*x**7+70*x**6-84*x**5+35*x**4

s(x)=s4(x)

uk(x,k)=1-exp(-x**k)


u1 = 0.80
u2 = 1.2
S = 0.3
l(v) = s((v-u1)/(u2-u1))
#u(v) = (v<u1?0: (v>u2?1: l(v)) )
A = 0.2
#f(v) = A + S*u(v)/v
#plot [0:4] [0:]  f(x) w l lw 2 title ""

f(v,k) = A + S*uk(v,k)/v
plot [0:4] [0:]  f(x,2) w l lw 2 title "2"
replot  f(x,3) w l lw 2 title "3"
replot  f(x,5) w l lw 2 title "5"
replot  f(x,8) w l lw 2 title "8"
replot  f(x,20) w l lw 2 title "20"
replot  f(x,1.3) w l lw 2 title "1.1"
replot  f(x,1.1) w l lw 2 title "1.1"
replot  f(x,1.02) w l lw 2 title "1.02"
replot  uk(x,20)/2 w l lw 2 title "u20"

pause 1000