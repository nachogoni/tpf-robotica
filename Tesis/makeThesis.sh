#!/bin/bash

rm tesis.aux tesis.idx tesis.log tesis.out tesis.toc tesis.lof tesis.lot tesis.idx tesis.bbl tesis.blg

cat hardware/references.bib > references.bib
cat comportamientos/references.bib >> references.bib
cat vision/references.bib >> references.bib

pdflatex tesis.tex

bibtex tesis

pdflatex tesis.tex
pdflatex tesis.tex

#evince tesis.pdf &
