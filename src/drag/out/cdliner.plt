set samples 300

s1(x)=x
s2(x)=3*x**2-2*x**3
s3(x)=6*x**5-15*x**4+10*x**3
s4(x)=-20*x**7+70*x**6-84*x**5+35*x**4
s(x)=s4(x)


load 'cdliner.dat'

c0 = bc0 * shape;
c1 = bc1 * shape;
c2 = bc2 * shape;

a4(x) = c1 + (c2-c1)* s( (x-bd2)/(bd3-bd2) )
a3(x) = ( x<bd3 ? a4(x) : c2 )
a2(x) = c0 + (c1-c0)* s( (x-bd1)/(bd2-bd1) )
a1(x) = ( x<bd2 ? a2(x) : a3(x) )
a(x) = ( x<bd1 ? c0 : a1(x) )

plot [0:6] a(x) lw 2

pause 1000
