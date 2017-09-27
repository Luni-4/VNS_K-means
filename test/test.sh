#!/bin/bash

FOLDER=Instances
OUTPUT=Results/
INSTANCES=$FOLDER/*
PREFIX=$FOLDER/
SUFFIX=".tsp"
SUFFIXO=".txt"
ALL="*"

MAXIT=( 10 30 50 )
K=( 5 8 10 )
K_LENGHT=${#K[@]}
N=$((${#MAXIT[@]} * K_LENGHT))
 
j=0
z=0

rm -rf $OUTPUT$ALL

for ((m=0; m<N; m++)); do 
              
        d=$OUTPUT"k"${K[$j]}"m"${MAXIT[$z]}

        for i in `ls -v $INSTANCES`; do                                                               
                ./kmeans $i 10 ${MAXIT[$z]} ${K[$j]} 30 1 >> $d$SUFFIXO                         
                ./kmeans++ $i 10 ${MAXIT[$z]} ${K[$j]} 30 1 >> $d$SUFFIXO                                                    
        done
        
        z=$(((m+1) % K_LENGHT)) 
        j=$(((m+1) / K_LENGHT))                                                               
done
