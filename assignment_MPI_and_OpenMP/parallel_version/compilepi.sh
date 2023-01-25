#!/bin/bash

module load 2020
module load GCC
gcc -fopenmp -o simple_try simple_try.c -lm