#!/bin/bash

module load 2020
module load GCC
mpicc -fopenmp -o second_try second_try.c -lm