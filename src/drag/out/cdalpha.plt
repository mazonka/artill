set samples 300

#a0=                  0.1521807262648475767
#a1=                  0.0027375134048963018288
#am=                  1.2011934780286186442
#aw=                  25.030464396073657696
#b1=                  0.13730679486511601639
#bm=                  1.0211581881813656114
#bw=                  46.923691404154396878

load 'cdalpha.dat'

a(x)=a0+a1/(1+exp(-aw*(x-am)))
b(x)=b1/(1+exp(-bw*(x-bm)))

c(x)=a(x)+b(x)/x

#plot "a.dat" w l lw 2
plot [0.5:3] c(x) lw 2, a(x) lw 2, b(x) lw 2
replot "a.dat" using 1:($1*($2-a($1))) w l lw 2 t "d"
#replot "a.dat" using 1:($1*($2)) w l lw 2 t "d"

#replot ao(x), bo(x)

pause 1
#fit c(x) "a.dat" via a4,a3,a2,a1,b4,b2,b1
#fit c(x) "a.dat" via a0,am,as,ah,bm,bs,bh

#replot

pause 1000
