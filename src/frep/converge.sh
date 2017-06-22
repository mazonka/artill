:> conv1.dat
:> conv0.dat

g()
{
ln=`./frep.w.exe smooth.dat $1 | grep "noise b" | sed -e 's/noise b =//g'`
#echo $ln
printf "%s" $ln >> conv$2.dat
}

f()
{
i=$1
echo -n "$i " >> conv$2.dat
g $1 $2
echo "" >> conv$2.dat
}

for i in {3..25}
do
j=$((i%2))
echo $i $j
f $i $j
done


