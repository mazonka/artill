set samples 300

a = -1
b =-2
c = 3

f(x)=a*x*x+b*x+c

m1 = -0.5*b/a

x1=-5
x2=3
x3=5
y1=f(x1)
y2=f(x2)
y3=f(x3)+1.0
#bug ?
#y3=f(x3)+1

denom = (x1 - x2)*(x1 - x3)*(x2 - x3)
A = (x3 * (y2 - y1) + x2 * (y1 - y3) + x1 * (y3 - y2)) / denom
B = (x3**2 * (y1 - y2) + x2**2 * (y3 - y1) + x1**2 * (y2 - y3)) / denom
C = (x2 * x3 * (x2 - x3) * y1 + x3 * x1 * (x3 - x1) * y2 + x1 * x2 * (x1 - x2) * y3) / denom

#m2= -0.5*B/A

g(x)=A*x*x+B*x+C

plot [-5:5] f(x), g(x)

pause 1000

#denom = (x1 - x2)(x1 - x3)(x2 - x3)
#A = (x3 * (y2 - y1) + x2 * (y1 - y3) + x1 * (y3 - y2)) / denom
#B = (x3**2 * (y1 - y2) + x2**2 * (y3 - y1) + x1**2 * (y2 - y3)) / denom
#C = (x2 * x3 * (x2 - x3) * y1 + x3 * x1 * (x3 - x1) * y2 + x1 * x2 * (x1 - x2) * y3) / denom
