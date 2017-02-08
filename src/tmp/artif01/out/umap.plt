#set size ratio -1
#set view equal xyz
#set view 70,30,1.3

#set ticslevel 0.0

#set style line 2 pt 2
#splot "umap.dat" using 1:2:3 w pm3d title "U" 
set grid
#set pm3d
unset hidden3d
splot "umap.dat" using 1:2:3 w l title "Utility function" 


#splot "out.dat" using 1:(log($2+1)):(log($3+1)) w p pt 13 ps 0.3

pause 1000
