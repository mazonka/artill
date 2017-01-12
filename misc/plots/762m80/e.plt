set samples 300

ef(x,m,s,b,a)=b+a*0.5*(erf((x-m)/sqrt(2*s*s))+1)

am              = 1
bm              = 1

aa              = -0.0440758
ab              = 0.148633
ba              = 0.448983
bs              = 0.0690775
as              = 0.332612

a(x)=ef(x,am,as,ab,aa)
b(x)=ef(x,bm,bs,0,ba)

c(x)=a(x)+b(x)/x

plot "a.dat" w l lw 2
replot c(x) lw 2, a(x) lw 2, b(x) lw 2
replot "a.dat" using 1:($1*($2-a($1))) w l lw 2 t "d"

pause 1
#fit c(x) "a.dat" via aa,ab,am,as,ba,bs,bm
fit c(x) "a.dat" via aa,ab,ba,bs,as

replot

pause 1000
