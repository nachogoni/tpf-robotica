#!/bin/bash

rm all.aux all.idx all.log all.out all.toc

cd ..

pdflatex -shell-escape -output-directory vision vision/all.tex 

cd vision

bibtex all

cd ..

pdflatex -shell-escape -output-directory vision vision/all.tex 
pdflatex -shell-escape -output-directory vision vision/all.tex 

cd vision

# evince all.pdf &
