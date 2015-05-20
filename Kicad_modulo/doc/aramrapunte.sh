#!/bin/bash
#Arma el apunte con los archivos .ps de kicad (trazar).

DIR=apunte
PNAME=cartel1

mkdir $DIR || true

#Esquematico

ps2pdf ${PNAME}.ps
mv ${PNAME}.pdf $DIR

#Cobre
ps2pdf ${PNAME}-Back.ps
mv ${PNAME}-Back.pdf $DIR

#Puentes
ps2pdf ${PNAME}-Front.ps
mv ${PNAME}-Front.pdf $DIR

#Serigrafia
ps2pdf ${PNAME}-SilkS_Front.ps
mv ${PNAME}-SilkS_Front.pdf $DIR

#Drawings
ps2pdf ${PNAME}-Drawings.ps
mv ${PNAME}-Drawings.pdf $DIR

#Armado
cd $DIR

pdftk  ${PNAME}-SilkS_Front.pdf stamp ${PNAME}-Front.pdf output ${PNAME}-Armadotmp.pdf
pdftk  ${PNAME}-Armadotmp.pdf  stamp ${PNAME}-Drawings.pdf output ${PNAME}-Armado.pdf
rm ${PNAME}-Armadotmp.pdf

#Armar apunte

pdftk ${PNAME}.pdf ${PNAME}-Armado.pdf ${PNAME}-Front.pdf  ${PNAME}-SilkS_Front.pdf   ${PNAME}-Back.pdf cat output ${PNAME}-APUNTE.pdf

xpdf ${PNAME}-APUNTE.pdf
