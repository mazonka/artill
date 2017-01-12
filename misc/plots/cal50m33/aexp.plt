set samples 300
#set terminal , enhanced
#set terminal postscript eps enhanced
set termoption enhanced

s1(x)=x
s2(x)=3*x**2-2*x**3
s3(x)=6*x**5-15*x**4+10*x**3
s4(x)=-20*x**7+70*x**6-84*x**5+35*x**4
s(x)=s4(x)

tr_int(x,x1,x2,y1,y2)=y1+(y2-y1)*s((x-x1)/(x2-x1))
tr(x,x1,x2,y1,y2)=(x<x1?y1:(x>x2?y2:tr_int(x,x1,x2,y1,y2)))

a4              = 0.195875
b4              = 0.214761
a3 = 0.1150

a2              = 1.28708
a1              = 0.867343
b2              = 1.0031
b1              = 0.933439

ao(x)=tr(x,a1,a2,a3,a4)
bo(x)=tr(x,b1,b2,0,b4)

a0              = 0.115207
am              = 1.06362
as              = 15.1677
ah              = 0.0833833
bm              = 0.967739
bs              = 71.1925
bh              = 0.20918

a(x)=a0+ah/(1+exp(-as*(x-am)))
b(x)=bh/(1+exp(-bs*(x-bm)))


c(x)=a(x)+b(x)/x

plot [0:3] [0:0.5] c(x) w l lw 2 t "Cd"
replot "a.dat" w l lw 2 t "M33"
replot a(x) lw 2 t "{/Symbol a}", b(x) lw 2 t "{/Symbol b}"
#replot "a.dat" using 1:($1*($2-a($1))) w l lw 2 t "d"

pause 2
fit c(x) "a.dat" via a0,am,as,ah,bm,bs,bh

replot

pause 1000
