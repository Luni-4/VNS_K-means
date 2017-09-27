#!/bin/bash

FOLDER=Results
FOLDERO=Wilcoxon
INPUT=$FOLDER/
OUTPUT=$FOLDERO/
SUFFIX=".txt"
VALUE="_Wilcoxon"
ALL="*"

rm -rf $OUTPUT$ALL

for i in `ls -v $INPUT`;
do
        o=${i%$SUFFIX} 
        d=$o$VALUE$SUFFIX
        perl SRTest.pl $INPUT$i > $OUTPUT$d           
done
