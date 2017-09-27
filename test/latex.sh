#!/bin/bash

FOLDER=Results
FOLDERO=Latex
INPUT=$FOLDER/
OUTPUT=$FOLDERO/
SUFFIX=".txt"
MINUS="-"
VALUE="_Latex"
ALL="*"

NDIST=(198 493 657 724 1655 1817 2152 2319 3038 3795)

rm -rf $OUTPUT$ALL

for i in `ls -v $INPUT`;
do
        o=${i%$SUFFIX} 
        d=$o$VALUE$SUFFIX
        b=0
        while IFS='' read -r line || [[ -n "$line" ]]; do                
                
                stringarray=($line)                
                v=$(awk '{print $1-$2}' <<< "${stringarray[0]} ${stringarray[1]}")
                v=${v#$MINUS}               
                echo ${NDIST[b]}" & "${stringarray[0]}" & "${stringarray[1]}" & "$v" //" >> $OUTPUT$d
                ((b++))                       
        done < $INPUT$i         
done
