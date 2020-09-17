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
  0.0050  1.1000  0.3000  1.6000 20.0  20.0  0.00  0.00  1.00  1.00 
  0.0000  1.8000  0.4000  2.0000 20.0  20.0  0.00  0.00  1.00  1.00 
EOF


for dis in 1 2 3 4 5 6 7 8 9 10
do
cat >> dfile << EOF
$dis   0.0005  512     -0.05   0                           
EOF
done
hprep96 -M simple.mod -d dfile -HS 0.002 -HR 0 -EXF
hspec96 > hspec96.out
hpulse96 -p -V -l 1  |  f96tosac -B                       
mkdir ORIG
mv *.sac ORIG

##clean
rm -rf dfile hspec96.dat hspec96.grn hspec96.out simple.mod