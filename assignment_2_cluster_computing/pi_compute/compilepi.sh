#!/bin/bash

module load 2020
module load GCC
gcc -fopenmp -o pi pi.c -lm