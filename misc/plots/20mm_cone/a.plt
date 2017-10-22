set samples 300

s1(x)=x
s2(x)=3*x**2-2*x**3
s3(x)=6*x**5-15*x**4+10*x**3
s4(x)=-20*x**7+70*x**6-84*x**5+35*x**4
s(x)=s4(x)

tr_int(x,x1,x2,y1,y2)=y1+(y2-y1)*s((x-x1)/(x2-x1))
tr(x,x1,x2,y1,y2)=(x<x1?y1:(x>x2?y2:tr_int(x,x1,x2,y1,y2)))

a4              = 0.0883352
a3 = 0.2129
b4              = 0.393003
b1              = 0.8

a2              = 0.891274
a1              = 0.797985
b2              = 1.06799

a(x)=tr(x,a1,a2,a3,a4)
b(x)=tr(x,b1,b2,0,b4)

c(x)=a(x)+b(x)/x

plot "a.dat" w l lw 2
replot c(x) lw 2, a(x) lw 2, b(x) lw 2
replot "a.dat" using 1:($1*($2-a($1))) w l lw 2 t "d"

pause 1
fit c(x) "a.dat" via a2,a1,b2

replot

pause 1000
