c0a=0.2
c1a=0.5
c2a=0.2
d1a=0.7
d2a=1.3
d3a=3.5

c0b=0.121
c1b=0.391
c2b=0.26
d1b=0.85
d2b=1.1
d3b=2.5

plot [0:5] [0:1]  "cd.dat" w l lw 2 title ""
replot "cd.dat" w p lt 1 ps 1 pt 6 title ""

s2(x)=3*x**2-2*x**3
s3(x)=6*x**5-15*x**4+10*x**3
s4(x)=-20*x**7+70*x**6-84*x**5+35*x**4

s(x)=s2(x)

g5a(x)=c0a+(c1a-c0a)*s((x-d1a)/(d2a-d1a))
g4a(x)=c1a+(c2a-c1a)*s((x-d2a)/(d3a-d2a))
g3a(x)=( x>d2a ? g4a(x):g5a(x) )
g2a(x)=( x>d3a ? c2a   :g3a(x) )
g1a(x)=( x>d1a ? g2a(x):c0a    )

g5b(x)=c0b+(c1b-c0b)*s((x-d1b)/(d2b-d1b))
g4b(x)=c1b+(c2b-c1b)*s((x-d2b)/(d3b-d2b))
g3b(x)=( x>d2b ? g4b(x):g5b(x) )
g2b(x)=( x>d3b ? c2b   :g3b(x) )
g1b(x)=( x>d1b ? g2b(x):c0b    )

replot g1a(x) lw 3 title ""
#replot g1b(x) lw 3 lt 2 title ""

pause 1000
