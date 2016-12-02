# f - pressure
# g - Cd

cd1=1

g=1.4
gm1=g-1
ggm1=g/(g-1)
gp1=1+g

f1a(x)=(1+cd1*gm1*x**2/2)**ggm1
f1(x)=(x>1.3?0/0:f1a(x))

f2a(x)=(gp1*x)**2/(4*x**2*g-2*gm1)
f2b(x)=f2a(x)**ggm1*(1-g+2*g*x**2)/gp1
f2(x)=(x<0.8?0/0:f2b(x))

g1(x) = (f1(x)-1)/x**2
g2(x) = (f2(x)-1)/x**2

plot [0:3] g1(x) lw 2
replot g2(x) lw 2
pause 100
