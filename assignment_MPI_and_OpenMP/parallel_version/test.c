#include <stdio.h>
#include <omp.h>
#include <mpi.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "./headers/beehive.h"
#include "./headers/glider.h"
#include "./headers/grower.h"

#ifndef N
#define N 3000
#endif

#define INDEX(i, j) (((i)*N)+(j))

#define ROW (N)
#define COL (N)
#define GENERATION (100)
#define BOARD_SIZE (N*N)

const int PATTERN_ROW=GROWER_HEIGHT;
const int PATTERN_COL=GROWER_WIDTH;

int is_border(int i,int j){
    if (i == 0 || i == ROW-1 || j == 0 || j == COL-1){
        return 1;
    }
    return -1;
}
 
// returns the count of alive neighbours
int count_live_neighbour_cell(int *a, int r, int c)
{
    int i, j, count = 0;
    for (i = r - 1; i <= r + 1; i++) {
        for (j = c - 1; j <= c + 1; j++) {
            if ((i == r && j == c) || (i < 0 || j < 0)
                || (i >= ROW || j >= COL)) {
                continue;
            }
            if (a[INDEX(i,j)] == 1) {
                count++;
            }
        }
    }
    return count;
}
int count_live_cell(int *a, int size) {
    int count = 0;
    #pragma omp parallel for
    for  (int i = 0; i < size; i++)  {
        if (a[i] == 1) {
            count++;
        }
    }
    // Gather all the results from each process to the root process
    //MPI_Gather(a[start_row], rows_per_proc*COL, MPI_INT, a, rows_per_proc*COL, MPI_INT, 0, MPI_COMM_WORLD);
    return count;
}


void init_board(int *a, int size){
    for (int i = 0; i < size; i++)
    {
        a[i] = 0;
    }
    
}

void calculate_next_generation(int *a, int *b, int rows_per_proc) {
    //init_canvas(b);
    int neighbour_live_cell;
    
    #pragma omp parallel for private(neighbour_live_cell)
    for (int i = 1; i < rows_per_proc+1; i++) {
        for (int j = 0; j < COL; j++) {
            neighbour_live_cell = count_live_neighbour_cell(a, i, j);
            if (a[INDEX(i,j)] == 1 && (neighbour_live_cell == 2 || neighbour_live_cell == 3)) {
                if (is_border(i, j) == -1) {
                    b[INDEX(i,j)] = 1;
                }
            } else if (a[INDEX(i,j)] == 0 && neighbour_live_cell == 3) {
                if (is_border(i, j) == -1) {
                    b[INDEX(i,j)] = 1;
                }
            } else {
                b[INDEX(i,j)] = 0;
            }
        }
    }
    // Gather all the results from each process to the root process
    //MPI_Gather(b[start_row], rows_per_proc*COL, MPI_INT, b, rows_per_proc*COL, MPI_INT, 0, MPI_COMM_WORLD);
    
}



void insert_pattern(uint8_t pattern[][PATTERN_COL], int *canvas, int pattern_row, int pattern_col, int canvas_row, int canvas_col, int start_row, int start_col) {
    for (int i = 0; i < pattern_row; i++) {
        for (int j = 0; j < pattern_col; j++) {
            canvas[INDEX(i + start_row,j + start_col)] = pattern[i][j];
        }
    }
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
        //MPI_Reduce(&output, &final_output, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (final_output == expected_output) {
	    printf("Test passed: count_live_cell(a) returned %d\n", final_output);
        } else {
	    printf("Test failed: count_live_cell(a) returned %d, expected %d\n", final_output, expected_output);
	}
}


void display(int *matrix){
    printf("%10c", ' ');
    printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");
    for (int i = 0; i < BOARD_SIZE; i++){
        printf("\n%10c", ' ');
        if (matrix[i] == 1)
            printf("+");
        else
            printf("-");
    }
    printf("\n%10c\n", ' ');
}

int main()
{   
    // Initialize MPI
    int rank, num_procs;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    //printf("procs %d is running\n",rank);

    //error handling
    int error = MPI_Comm_set_errhandler(MPI_COMM_WORLD, MPI_ERRORS_RETURN);
    printf("error:\n")
    printf(error)
    
    int rows_per_proc = (int)ceil((double)ROW / num_procs);//ver
    int row_size = (rows_per_proc+2) * ROW;
    int *A = NULL; 
    int *B = NULL; 
    int *C = NULL;
    int *D = NULL;
    A = (int*)malloc(BOARD_SIZE*sizeof(int));//global
    B = (int*)malloc(BOARD_SIZE*sizeof(int));
    C = (int*)malloc(row_size*sizeof(int));//local
    D = (int*)malloc(row_size*sizeof(int));
    init_board(C,row_size);
    init_board(D,row_size);
    
    //INI
    if (rank == 0) {
        int i;
        init_board(A,BOARD_SIZE);
        //init_board(B,BOARD_SIZE);
        insert_pattern(grower,A,PATTERN_ROW,PATTERN_COL,ROW,COL,1500,1500);
 
        memcpy(&C[N], &A[0], rows_per_proc * ROW * sizeof(int));//??
        for (int proc = 1; proc < num_procs; proc++) {
            //MPI_Send(&global_matrix[dir*ver*MAX_N], ver * MAX_N, MPI_INT, dir, 1, MPI_COMM_WORLD);
            MPI_Send(&A[proc*rows_per_proc*ROW], rows_per_proc * ROW, MPI_INT, proc, 1, MPI_COMM_WORLD);
        }
 
        //display(A);
    } else {
        //For each processor, there is a local matrix.
        MPI_Recv(&C[ROW], rows_per_proc * ROW, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    double start,end;
    
    start=MPI_Wtime();

    int top_neighbour = rank-1;
    int bot_neighbour = rank+1;
    
    for (int i = 0; i < GENERATION; i++)
    {
        // top
        if (top_neighbour >= 0){
            
            MPI_Sendrecv(&C[ROW], ROW, MPI_INT, top_neighbour, 0,C, ROW, MPI_INT, top_neighbour, MPI_ANY_TAG,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        // bottom
        if (bot_neighbour < num_procs)
        {
            MPI_Sendrecv(&C[(rows_per_proc)*ROW], ROW, MPI_INT, bot_neighbour, 0,&C[(rows_per_proc+1)*ROW], ROW, MPI_INT, bot_neighbour, MPI_ANY_TAG,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        //game
        calculate_next_generation(C,D,rows_per_proc);
        {
            // Swap the role of the two arrays.
            int *tmp = C;
            C = D;
            D = tmp;
        }
    }
    
    
    if (rank == 0) {
        memcpy(A, &C[ROW], rows_per_proc * ROW * sizeof(int));
        for (int proc = 1; proc < num_procs; proc++) {
            MPI_Recv(&A[proc*rows_per_proc*ROW], rows_per_proc*ROW, MPI_INT, proc, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        //display(A);
        //test
	if(GENERATION!=5000){
            test_count_live_cell(A,GENERATION);
	}
	else{
	    printf("# of cell alive in the 5000th generation: %d\n",count_live_cell(A,BOARD_SIZE));
	}
    }
    else {
        MPI_Send(&C[ROW], rows_per_proc*ROW, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }
            
    end=MPI_Wtime();
    // Print result
    if (rank==0){
        printf("Obtained in %f seconds, rank=%d\n",end - start,rank);
    }
    
    free(A);
    free(B);
    free(C);
    free(D);

    // Finalize MPI
    MPI_Finalize();
    return 0;
}
