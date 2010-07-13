#!/bin/bash

rm all.aux all.idx all.log all.out all.toc

cd ..

pdflatex -shell-escape -output-directory comportamientos comportamientos/all.tex 
pdflatex -shell-escape -output-directory comportamientos comportamientos/all.tex 

cd comportamientos

evince all.pdf &
