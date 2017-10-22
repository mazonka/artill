set samples 300

s1(x)=x
s2(x)=3*x**2-2*x**3
s3(x)=6*x**5-15*x**4+10*x**3
s4(x)=-20*x**7+70*x**6-84*x**5+35*x**4
s(x)=s4(x)

tr_int(x,x1,x2,y1,y2)=y1+(y2-y1)*s((x-x1)/(x2-x1))
tr(x,x1,x2,y1,y2)=(x<x1?y1:(x>x2?y2:tr_int(x,x1,x2,y1,y2)))

a4              = 0.148804
a3              = 0.130948
a2              = 1.28322
a1              = 0.979995
b4              = 0.279681
b2              = 1.0605
b1              = 0.859423

#a0 = 0.13
#am = 1.1
#as = 0.02
#ah = 0.02
#bm = 0.95
#bs = 0.02
#bh = 0.28
a0              = 0.130357
am              = 1.16576
as              = 25
ah              = 0.0160684
bm              = 0.960856
bs              = 47
bh              = 0.284374

a(x)=a0+ah/(1+exp(-as*(x-am)))
b(x)=bh/(1+exp(-bs*(x-bm)))

ao(x)=tr(x,a1,a2,a3,a4)
bo(x)=tr(x,b1,b2,0,b4)

c(x)=a(x)+b(x)/x

plot "a.dat" w l lw 2
replot c(x) lw 2, a(x) lw 2, b(x) lw 2
replot "a.dat" using 1:($1*($2-a($1))) w l lw 2 t "d"
#replot "a.dat" using 1:($1*($2)) w l lw 2 t "d"

#replot ao(x), bo(x)

pause 1
#fit c(x) "a.dat" via a4,a3,a2,a1,b4,b2,b1
fit c(x) "a.dat" via a0,am,as,ah,bm,bs,bh

replot

pause 1000
