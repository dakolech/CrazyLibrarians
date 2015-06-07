#!/bin/bash

BIBLIOTEKARZE=100
MPC=10

if [ $# -eq 0 ]
then
    echo "Nie podano liczby bibliotekrzy - ustawiono domyślną wartość $BIBLIOTEKARZE"
    echo "Nie podano liczby MPC - ustawiono domyślną wartość $MPC"
else
    if [ $# -eq 1 ]
    then
        BIBLIOTEKARZE=$1
        echo "Nie podano liczby MPC - ustawiono domyślną wartość $MPC"
    else
        BIBLIOTEKARZE=$1
        MPC=$2
    fi
fi

mpirun -np $BIBLIOTEKARZE debug/CrazyLibrarians -p $MPC
