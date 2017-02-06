set samples 300

#set title "155mm, m=35.42, v0=791"

load 'cdalpha.dat'

#a(x)=a0+a1/(1+exp(-aw*(x-am)))
#b(x)=b1/(1+exp(-bw*(x-bm)))
a(x)=a0+a1/(1+exp(-(x-am)/aw))
b(x)=b1/(1+exp(-(x-bm)/bw))

c(x)=a(x)+b(x)/x

#plot "a.dat" w l lw 2
plot [0.5:3] c(x) lw 2 t "Cd"
#replot c(x) lw 2
#replot "a.dat" using 1:($1*($2-a($1))) w l lw 2 t "d"
#replot "a.dat" using 1:($1*($2)) w l lw 2 t "d"

#replot ao(x), bo(x)

pause 1
#fit c(x) "a.dat" via a4,a3,a2,a1,b4,b2,b1
#fit c(x) "a.dat" via a0,am,as,ah,bm,bs,bh

#replot

pause 1000
