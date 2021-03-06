#!/bin/bash

rm -fr ORIG RICKER
##clean
rm -rf dfile hspec96.dat hspec96.grn hspec96.out simple.mod
#####
# MODEL 1 , dr pujol, date 23 Aug 2011, from paper 
# "shear wave velocity profiling, at sites with high 
# freqeuncy stiffness contrasts: a comparison between 
# invasive and non-invasive methods", TABLE 1
#
# Create the model using mkmod96                              CREATE THE VELOCITY MODEL USING mkmod96
#####                                                         THIS IS COMMENTED SINCE THE MODEL IS GIVEN BELOW
#mkmod96 << EOF
#simple.mod
#Simple Crustal Model
#0
#0.0050 1.100 0.300 1.6 20 20 0 0 1 1
#000000 1.800 0.400 2.0 20 20 0 0 1 1
#EOF

echo Start >> time_run.txt
date >> time_run.txt
cat > simple.mod << EOF
MODEL.01
Simple Crustal Model
ISOTROPIC
KGS
FLAT EARTH
1-D
CONSTANT VELOCITY
LINE08
LINE09
LINE10
LINE11
  H(KM) VP(KM/S) VS(KM/S) RHO(GM/CC)  QP   QS  ETAP  ETAS  FREFP  FREFS 
  0.0050  1.1000  0.3000  2.5000 20.0  20.0  0.00  0.00  1.00  1.00 
  0.0080  1.8000  0.4000  2.6000 20.0  20.0  0.00  0.00  1.00  1.00 
  0.0050  2.0000  1.1500  2.8000 20.0  20.0  0.00  0.00  1.00  1.00 
EOF


for dis in `seq 30`
do
cat >> dfile << EOF
$dis   0.0005  1024     0   0                           
EOF
done
hprep96 -M simple.mod -d dfile -HS 0.001 -HR 0 -EXF
hspec96 > hspec96.out
hpulse96 -p -V -l 1  |  f96tosac -B                       
mkdir EXPL
mv *.sac EXPL

##clean
rm -rf dfile hspec96.dat hspec96.grn hspec96.out simple.mod

echo Completed! >> time_run.txt
date >> time_run.txt