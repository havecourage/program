#!/bin/sh
#This example computes P-wave receiverfunctions for a single layer overahalfspace.These synthetic data sets were created for the single layer overahalfspace model
cat > model.mod <<EOF
MODEL
TEST MODEL
ISOTROPIC
KGS
FLAT EARTH
1-D
CONSTANT VELOCITY
LINE08
LINE09
LINE10
LINE11
HR  VP  VS  RHO QP  QS  ETAPETAS FREFP FREFS
40  63.5 2.5 200.0 100.0 0.0    0.0  1.0    1.0
 0     8 4.7 3.3   900.0 500.0  0.0   0.0  1.0  1.0
EOF

p=0.1
for CALP in 05 10 25 50
do
case $CALP in 
        05) ALP=0.5 ;;
        10) ALP=1.0 ;;
        25) ALP=2.5 ;;
        50) ALP=5.0 ;;
esac
hrftn96 -P -ALP $ALP -DT 0.1 -D 10. -RAYP $p -M model.mod -2
mv hrftn96.sac ${CALP}.rfn
done


#想要顺利进行后续步骤，当前文件夹中还需要robs.d文件、
#初始的地球模型,频散文件以及接收函数文件

cat > robs.d <<EOF
1 0 0 0 0 0 0 0 1 0
EOF


