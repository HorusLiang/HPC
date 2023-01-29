#include <stdio.h>
#include <omp.h>
#include <mpi.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "./headers/beehive.h"
#include "./headers/glider.h"
#include "./headers/grower.h"

#define N 3000
// change row and column value to set the canvas size
const int ROW = N;
const int COL = N;
#define INDEX(i, j) (((i)*COL)+(j))

#define BOARD_SIZE (ROW*COL)
#define GENERATION (100)
const int PATTERN_ROW=GROWER_HEIGHT;
const int PATTERN_COL=GROWER_WIDTH;

// Initialize MPI
int rank, num_procs;

// Divide loop iterations among processes
int rows_per_proc;
int start_row ;
int end_row;
int *A;

int is_border(int i,int j){
    if (i == 0 || i == ROW-1 || j == 0 || j == COL-1){
        return 1;
    }
    return -1;
}
int count_live_neighbour_cell(int *sub_data, int r, int c)
{
    int i, j, count = 0;
    for (i = r - 1; i <= r + 1; i++) {
        for (j = c - 1; j <= c + 1; j++) {
            if ((i == r && j == c) || (i < 0 || j < 0)
                || (i >= ROW || j >= COL)) {
                continue;
            }
            if (sub_data[INDEX(i,j)] == 1) {
                count++;
            }
        }
    }
    return count;
}
void calculate_next_generation(int *sub_data) {
    
    int * result=(int*)malloc(sizeof(int) * (rows_per_proc+2) * COL);
    memset(result, 0, (rows_per_proc+2) * COL* sizeof(int));
    int neighbour_live_cell;

    #pragma omp parallel for private(neighbour_live_cell)
    for (int i =1; i < rows_per_proc+1; i++) { // maybe a hidden bug in the row bound ???
        for (int j = 0; j < COL; j++) {
            neighbour_live_cell = count_live_neighbour_cell(sub_data, i, j);
            if (sub_data[INDEX(i,j)] == 1 && (neighbour_live_cell == 2 || neighbour_live_cell == 3)) {
                if (is_border(i, j) == -1) {
                    result[INDEX(i,j)]= 1;
                }
            } else if (sub_data[INDEX(i,j)] == 0  && neighbour_live_cell == 3) {
                if (is_border(i, j) == -1) {
                    result[INDEX(i,j)] = 1;
                }
            } else {
                result[INDEX(i,j)] = 0;
            }
        }
    }
    // Gather all the results from each process to the root process
    MPI_Gather(&result[1], rows_per_proc*COL, MPI_INT, A, rows_per_proc*COL, MPI_INT, 0, MPI_COMM_WORLD);
}
void insert_pattern(uint8_t pattern[][PATTERN_COL], int *canvas, int pattern_row, int pattern_col, int canvas_row, int canvas_col, int start_row, int start_col) {
    for (int i = 0; i < pattern_row; i++) {
        for (int j = 0; j < pattern_col; j++) {
            canvas[INDEX(i + start_row,j + start_col)] = pattern[i][j];
        }
    }
}

int count_live_cell(int *a, int size) {
    int count = 0;
    #pragma omp parallel for
    for  (int i = 0; i < size; i++)  {
        if (a[i] == 1) {
            count++;
        }
    }
    return count;
}
void test_count_live_cell(int *a, int generation) {
    int final_output = count_live_cell(a,BOARD_SIZE);
	int expected_output;
	if(generation==10){
	    expected_output = 49;
	}
	if(generation==100){
	    expected_output = 138;
	}
    if(generation==1){
        expected_output=23;
    }
    if(generation==2){
        expected_output=24;
    }
	if (final_output == expected_output) {
	    printf("Test passed: count_live_cell(a) returned %d, # of cell alive in the %dth generation\n", final_output,generation);
        } else {
	    printf("Test failed: count_live_cell(a) returned %d, expected %d\n", final_output, expected_output);
	}
}
int main()
{   
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    rows_per_proc = (int)ceil((double)ROW / num_procs);
    start_row = rank * rows_per_proc;
    end_row = start_row + rows_per_proc;
    if(end_row > ROW) end_row = ROW;
    
    

    if (rank == 0) {
        // initialize the data on rank 0
        A=(int*)malloc(BOARD_SIZE*sizeof(int));
        memset(A, 0, BOARD_SIZE * sizeof(int));
        insert_pattern(grower,A,PATTERN_ROW,PATTERN_COL,ROW,COL,1500,1500);
        // printf("A is initialized successfully! rank=%d\n",rank);
    }
    
    


    double start,end;
    start=omp_get_wtime();
     // Gameplay
    for (int i = 0; i < GENERATION; i++){
        int * sub_data = (int*)malloc(sizeof(int) * (rows_per_proc+2) * COL);
        memset(sub_data, 0, sizeof(int) * (rows_per_proc + 2) * COL);
        MPI_Scatter(A, (rows_per_proc) * COL, MPI_INT, &sub_data[ROW], (rows_per_proc) * COL, MPI_INT, 0, MPI_COMM_WORLD);
        // printf("A is scatterred successfully! rank=%d\n",rank);

        int top_neighbour = rank-1;
        int bot_neighbour = rank+1;
        if (top_neighbour >= 0){    
            MPI_Sendrecv(&sub_data[ROW], ROW, MPI_INT, top_neighbour, 0,sub_data, ROW, MPI_INT, top_neighbour, MPI_ANY_TAG,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        if (bot_neighbour < num_procs){
            MPI_Sendrecv(&sub_data[(rows_per_proc)*ROW], ROW, MPI_INT, bot_neighbour, 0,&sub_data[(rows_per_proc+1)*ROW], ROW, MPI_INT, bot_neighbour, MPI_ANY_TAG,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        // printf("sub_data is initialized successfully! rank=%d\n",rank);
        calculate_next_generation(sub_data);
    }
    if(rank==0){
        // printf("test part has reached!\n");
        test_count_live_cell(A,GENERATION);
    }
    

    end=omp_get_wtime();
    // Print result
    if(rank==0){
        printf("Obtained in %f seconds\n",end - start);
    }

   

    

    
    // grower_test(10,49);
    // grower_test(5000,3647);
    

    
    // Finalize MPI
    MPI_Finalize();
    return 0;
}