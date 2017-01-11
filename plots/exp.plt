set samples 300

a0=                  0.20747199999999998976
ah=                  -0.017224500000000000227
am=                  0.99363800000000002122
as=                  1.9991699999999998916
bh=                  0.066475500000000006806
bm=                  0.94922099999999998143
bs=                  21.001000000000001222


a(x)=a0+ah/(1+exp(-as*(x-am)))
b(x)=bh/(1+exp(-bs*(x-bm)))

ao(x)=tr(x,a1,a2,a3,a4)
bo(x)=tr(x,b1,b2,0,b4)

c(x)=a(x)+b(x)/x

#plot "a.dat" w l lw 2
plot c(x) lw 2, a(x) lw 2, b(x) lw 2
replot "a.dat" using 1:($1*($2-a($1))) w l lw 2 t "d"
#replot "a.dat" using 1:($1*($2)) w l lw 2 t "d"

#replot ao(x), bo(x)

pause 1
#fit c(x) "a.dat" via a4,a3,a2,a1,b4,b2,b1
#fit c(x) "a.dat" via a0,am,as,ah,bm,bs,bh

#replot

pause 1000
