#include <stdio.h>
#include <omp.h>
#include <mpi.h>
#include <math.h>
#include "./headers/beehive.h"
#include "./headers/glider.h"
#include "./headers/grower.h"
 
// change row and column value to set the canvas size
const int ROW = 3000;
const int COL = 3000;
// const int PATTERN_ROW=GLIDER_HEIGHT;
// const int PATTERN_COL=GLIDER_WIDTH;

// const int PATTERN_ROW=BEEHIVE_HEIGHT;
// const int PATTERN_COL=BEEHIVE_WIDTH;

const int PATTERN_ROW=GROWER_HEIGHT;
const int PATTERN_COL=GROWER_WIDTH;

// Initialize MPI
int rank, num_procs;


// Divide loop iterations among processes
int rows_per_proc;
int start_row ;
int end_row;

int is_border(int i,int j){
    if (i == 0 || i == ROW-1 || j == 0 || j == COL-1){
        return 1;
    }
    return -1;
}
 
// returns the count of alive neighbours
int count_live_neighbour_cell(int a[ROW][COL], int r, int c)
{
    int i, j, count = 0;
    for (i = r - 1; i <= r + 1; i++) {
        for (j = c - 1; j <= c + 1; j++) {
            if ((i == r && j == c) || (i < 0 || j < 0)
                || (i >= ROW || j >= COL)) {
                continue;
            }
            if (a[i][j] == 1) {
                count++;
            }
        }
    }
    return count;
}
int count_live_cell(int a[][COL]) {
    int count = 0;
    #pragma omp parallel for
    for  (int i = start_row; i < end_row; i++)  {
        for (int j = 0; j < COL; j++) {
            if (a[i][j] == 1) {
                count++;
            }
        }
    }
    // Gather all the results from each process to the root process
    MPI_Gather(a[start_row], rows_per_proc*COL, MPI_INT, a, rows_per_proc*COL, MPI_INT, 0, MPI_COMM_WORLD);
    return count;
}

void init_canvas(int a[][COL]){
    int i, j;

    #pragma omp parallel for
    for (i = start_row; i < end_row; i++) {
        for (j = 0; j < COL; j++) {
            a[i][j]=0;  
        }
    }
    // Gather all the results from each process to the root process
    MPI_Gather(a[start_row], rows_per_proc*COL, MPI_INT, a, rows_per_proc*COL, MPI_INT, 0, MPI_COMM_WORLD);
}
void calculate_next_generation(int a[][COL], int b[][COL]) {
    init_canvas(b);
    int neighbour_live_cell;

    
    #pragma omp parallel for private(neighbour_live_cell)
    for (int i = start_row; i < end_row; i++) {
        for (int j = 0; j < COL; j++) {
            neighbour_live_cell = count_live_neighbour_cell(a, i, j);
            if (a[i][j] == 1 && (neighbour_live_cell == 2 || neighbour_live_cell == 3)) {
                if (is_border(i, j) == -1) {
                    b[i][j] = 1;
                }
            } else if (a[i][j] == 0 && neighbour_live_cell == 3) {
                if (is_border(i, j) == -1) {
                    b[i][j] = 1;
                }
            } else {
                b[i][j] = 0;
            }
        }
    }
    // Gather all the results from each process to the root process
    MPI_Gather(b[start_row], rows_per_proc*COL, MPI_INT, b, rows_per_proc*COL, MPI_INT, 0, MPI_COMM_WORLD);
    
}

void calculate_n_generation(int a[][COL], int b[][COL],int time){
    // int res[ROW][COL];
    // init_canvas(res);

    

    for (int i = 0; i < time; i++) {
        if (i % 2 == 0) {
            calculate_next_generation(a, b);
            // init_canvas(a);
            // print_2d_array(b);
        } else {
            calculate_next_generation(b, a);
            // init_canvas(b);
            // print_2d_array(a);
        }
    }
}


void insert_pattern(uint8_t pattern[][PATTERN_COL], int canvas[][COL], int pattern_row, int pattern_col, int canvas_row, int canvas_col, int start_row, int start_col) {
    for (int i = 0; i < pattern_row; i++) {
        for (int j = 0; j < pattern_col; j++) {
            canvas[i + start_row][j + start_col] = pattern[i][j];
        }
    }
}



void test_count_live_cell(int a[][COL],int expected_output) {
        int output = count_live_cell(a);
        if (output == expected_output) {
            printf("Test passed: count_live_cell(a) returned %d\n", output);
        } else {
            printf("Test failed: count_live_cell(a) returned %d, expected %d\n", output, expected_output);
        }
}
// void glider_test(int generation,int expected){
//     const int PATTERN_ROW=GLIDER_HEIGHT;
//     const int PATTERN_COL=GLIDER_WIDTH;

//     int a[ROW][COL], b[ROW][COL];
//     init_canvas(a);
//     insert_pattern(glider,a,PATTERN_ROW,PATTERN_COL,ROW,COL,1,1);

//     calculate_n_generation(a,b,generation);
//     test_count_live_cell(b,expected);
// }


// void beehive_test(int generation,int expected){
    
//     int a[ROW][COL], b[ROW][COL];
//     init_canvas(a);
//     insert_pattern(beehive,a,PATTERN_ROW,PATTERN_COL,ROW,COL,1,1);

//     calculate_n_generation(a,b,generation);
//     test_count_live_cell(b,expected);
// }

void grower_test(int generation,int expected){
    
    int a[ROW][COL], b[ROW][COL];
    init_canvas(a);
    init_canvas(b);
    insert_pattern(grower,a,PATTERN_ROW,PATTERN_COL,ROW,COL,1500,1500);

    calculate_n_generation(a,b,generation);
    if(generation%2==0){
        test_count_live_cell(a,expected);
    }else{
        test_count_live_cell(b,expected);
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
    
    
    double start,end;
    start=omp_get_wtime();
    // glider_test(50,5);
    // beehive_test(10,6);
    
    // grower_test(100,138);
    // grower_test(0,20);
    // grower_test(1,23);
    // grower_test(2,24);
    // grower_test(3,28);
    // grower_test(4,31);
    // grower_test(5,35);
    // grower_test(6,35);
    // grower_test(7,35);
    // grower_test(8,43);
    // grower_test(9,41);
    grower_test(10,49);
    // grower_test(5000,3647);
    end=omp_get_wtime();
    // Print result
    printf("Obtained in %f seconds\n",end - start);

    // Finalize MPI
    MPI_Finalize();
    return 0;
}