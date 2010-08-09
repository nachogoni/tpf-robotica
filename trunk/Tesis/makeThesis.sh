#!/bin/bash

rm all.aux all.idx all.log all.out all.toc

cat hardware/references.bib > references.bib
cat comportamientos/references.bib >> references.bib
cat vision/references.bib >> references.bib

pdflatex tesis.tex

bibtex tesis

pdflatex tesis.tex
pdflatex tesis.tex

#evince tesis.pdf &
