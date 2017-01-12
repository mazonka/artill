set isosamples 30
#set samples 140

a=30
b=0.2

f(x,y)=1-(1+b*x)*exp(-a*(x**2+y**2-1)**2)

splot [-2:2] [-2:2] f(x,y) t "a=30 b=0.2"
pause 1000
