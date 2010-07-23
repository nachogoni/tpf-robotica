#!/bin/bash

echo "# Created by Octave 3.2.3, Fri Jul 02 11:25:11 2010 ART <gcampelo@pibe>" > simStatsOct.txt
echo "# name: vars" >> simStatsOct.txt
echo "# type: matrix" >> simStatsOct.txt
a=`wc -l simStats.txt`
echo $a
echo "# rows: 1" >> simStatsOct.txt
echo "# columns: 8" >> simStatsOct.txt
cat simStats.txt >> simStatsOct.txt
