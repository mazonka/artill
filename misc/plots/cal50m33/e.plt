set samples 300

ef(x,m,s,b,a)=b+a*0.5*(erf((x-m)/sqrt(s*s))+1)
ex(x,m,s,a)=a*exp(-(x-m)**2/(s*s))

aa              = 0.0851404
ab              = 0.11548
am              = 1.05637
as              = 0.155552
ba              = 0.205083
bs              = 0.0337094
bm              = 0.967252

a(x)=ef(x,am,as,ab,aa)
b(x)=ef(x,bm,bs,0,ba)
a2(x)=ex(x,am,as,aa)
b2(x)=ex(x,bm,bs,ba)

c(x)=a(x)+b(x)/x

plot "a.dat" w l lw 2
replot c(x) lw 2, a(x) lw 2, b(x) lw 2
replot "a.dat" using 1:($1*($2-a($1))) w l lw 2 t "d"
replot a2(x) lw 2, b2(x) lw 2

pause 1
fit c(x) "a.dat" via aa,ab,am,as,ba,bs,bm
#fit c(x) "a.dat" via aa,ab,ba,bs

replot

pause 1000
