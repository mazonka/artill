set samples 301

ef(x,m,s,b,a)=b+a*0.5*(erf((x-m)/sqrt(s*s))+1)
ex(x,m,s,a)=a*exp(-(x-m)**2/(s*s))

aa              = 0.0169925
ab              = 0.130726
am              = 1.14692
as              = 0.0840112
ba              = 0.281808
bs              = 0.05023
bm              = 0.960386

a(x)=ef(x,am,as,ab,aa)
b(x)=ef(x,bm,bs,0,ba)
a2(x)=ex(x,am,as,aa)
b2(x)=ex(x,bm,bs,ba)

c(x)=a(x)+b(x)/x

plot [0:3] c(x) lw 2

set table "taylor.tmp"
plot [0:3] c(x) lw 2
unset table

replot "taylor.tmp"

c0 = c(3)
c1              = -0.0914778
c2              = -0.0125015
c3              = 0.0134551
c4 = 0.001
c5 = 0.001
c6 = 0.0001
c7 = 0.0001
c8 = 0
f(x) = c0 + c1*(x-3)+c2*(x-3)**2+c3*(x-3)**3+c4*(x-3)**4+c5*(x-3)**5+c6*(x-3)**6+c7*(x-3)**7

replot f(x)

pause 1
fit f(x) "taylor.tmp" via c1,c2,c3,c4,c5,c6,c7

replot

pause 1000
