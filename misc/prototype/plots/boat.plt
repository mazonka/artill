# t == tan(beta)
# s == sin(beta)
# r=R-h t
# A1 = pi R^2 - A2
# A2 = pi r^2 = pi( R - h t )^2
# C = A1 sinb + A2 = pi R^2 + 2 pi R H (ts-t) + h^2 ( t^2 - t^2 s^2 )

pi=acos(-1)
R=1
f(s,t,h) = pi*R**2+2*pi*R*h*(s*t-s) +h*h*(t**2-s**2*t**2)
g1(x,h) = f(sin(x),tan(x),h)
g2(x,h) = g1(x/180*pi,h)

plot [0:40] g2(x,1)
replot g2(x,2)
replot g2(x,0.5)
pause 100
