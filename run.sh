#!/bin/bash

BIBLIOTEKARZE=100

if [ $# -eq 0 ]
then
    echo "Nie podano liczby bibliotekrzy - ustawiono domyślną wartość $BIBLIOTEKARZE"
else
    BIBLIOTEKARZE=$1
fi

mpirun -np $BIBLIOTEKARZE debug/CrazyLibrarians -p 10
