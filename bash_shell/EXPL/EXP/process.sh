#/bin/sh
for name in `ls *ZEX*`
do
sac2col ${name} | tail -n 1024 > Vz${name:1:3}.txt
done


for name in `ls *REX*`
do
sac2col ${name} | tail -n 1024 > Vr${name:1:3}.txt
done