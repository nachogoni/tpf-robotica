#!/bin/bash

echo "# Created by Octave 3.2.3, Fri Jul 02 11:25:11 2010 ART <gcampelo@pibe>" > odometryErrorOct.txt
echo "# name: vars" >> odometryErrorOct.txt
echo "# type: matrix" >> odometryErrorOct.txt
a=`wc -l odometryError.txt`
echo $a
echo "# rows: 1" >> odometryErrorOct.txt
echo "# columns: 6" >> odometryErrorOct.txt
cat odometryError.txt >> odometryErrorOct.txt
