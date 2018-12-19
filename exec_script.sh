#!/bin/bash

iteraciones=(1 2 3 4 5)
matrix_sizes=(2050 4098 8194)

for matrix_size in "${matrix_sizes[@]}"
do
    echo "-------------------------------"
    echo "|  Matrix size $matrix_size  |"
    echo "-------------------------------"

    for iteracion in "${iteraciones[@]}"
    do
        echo "-------------------------------"
        echo "| Iteracion $iteracion  |"
        echo "-------------------------------"
	./gs_openmp $matrix_size

    done
done
