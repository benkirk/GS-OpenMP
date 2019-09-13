#!/bin/bash


for pow in $(seq 12 14); do

    ms=$((2 ** ${pow}))
    nt=2
    mt=57

    while [ ${nt} -lt ${mt} ]; do
        echo "n=${ms}, n_threads=${nt}"

        for iter in $(seq 1 5); do
            times=$(./gs_openmp ${ms} ${nt})
            echo " " $times ", iter="$iter
        done

        nt=$((${nt} + 2))
    done

    echo && echo && echo
done
