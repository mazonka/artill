set samples 300

s1(x)=x
s2(x)=3*x**2-2*x**3
s3(x)=6*x**5-15*x**4+10*x**3
s4(x)=-20*x**7+70*x**6-84*x**5+35*x**4
s(x)=s4(x)

tr_int(x,x1,x2,y1,y2)=y1+(y2-y1)*s((x-x1)/(x2-x1))
tr(x,x1,x2,y1,y2)=(x<x1?y1:(x>x2?y2:tr_int(x,x1,x2,y1,y2)))

a4              = 0.105032
b4              = 0.458613
a3 = 0.1388

a2              = 1.12598
a1              = 0.787926
b2              = 1.12779
b1              = 0.832699

a(x)=tr(x,a1,a2,a3,a4)
b(x)=tr(x,b1,b2,0,b4)

c(x)=a(x)+b(x)/x

plot "a.dat" w l lw 2
replot c(x) lw 2, a(x) lw 2, b(x) lw 2
replot "a.dat" using 1:($1*($2-a($1))) w l lw 2 t "d"

pause 2
fit c(x) "a.dat" via a2,a1,b2,b1

replot

pause 1000
