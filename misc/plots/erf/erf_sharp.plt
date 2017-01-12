set samples 300

xa=1
xs=3
xm=4
ex(x,m,s,a)=a*exp(-(x-m)**2/(2*s*s))
ef(x,m,s,a)=a*0.5*(erf((x-m)/sqrt(2*s*s))+1)


#plot ef(x,xm,xs,xa),ex(x,xm,xs,xa)

a0=0.2

ma=1
sa=0.0001
aa=0

mb=1
sb=0.0001
ab=0.3

f(x)=a0+ef(x,ma,sa,aa)+ef(x,mb,sb,ab)/x
plot [0:3] f(x),ex(x,ma,sa,aa),ex(x,mb,sb,ab)
replot a0+ef(x,ma,sa,aa),ef(x,mb,sb,ab)

pause 1000
