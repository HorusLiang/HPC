#include <stdio.h>
#include "./headers/beehive.h"
#include "./headers/glider.h"
#include "./headers/grower.h"
 
// change row and column value to set the canvas size
const int ROW = 900;
const int COL = 900;

int board [ROW][COL];

// const int PATTERN_ROW=GLIDER_HEIGHT;
// const int PATTERN_COL=GLIDER_WIDTH;

// const int PATTERN_ROW=BEEHIVE_HEIGHT;
// const int PATTERN_COL=BEEHIVE_WIDTH;

const int PATTERN_ROW=GROWER_HEIGHT;
const int PATTERN_COL=GROWER_WIDTH;


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
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (a[i][j] == 1) {
                count++;
            }
        }
    }
    return count;
}

void init_canvas(int a[][COL]){
    int i, j;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            a[i][j]=0;  
        }
    }
}
void calculate_next_generation(int a[][COL]) {
    int neighbour_live_cell;
    int new_b[ROW][COL];
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            neighbour_live_cell = count_live_neighbour_cell(a, i, j);
            if (a[i][j] == 1 && (neighbour_live_cell == 2 || neighbour_live_cell == 3)) {
                if (is_border(i, j) == -1) {
                    new_b[i][j] = 1;
                }
            } else if (a[i][j] == 0 && neighbour_live_cell == 3) {
                if (is_border(i, j) == -1) {
                    new_b[i][j] = 1;
                }
            } else {
                new_b[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < ROW; i++){
        for (int j = 0; j < COL; j++){
            board[i][j] = new_b[i][j];
        }
    }
}

void calculate_n_generation(int a[][COL],int time){
    for (int i = 0; i < time; i ++){
        calculate_next_generation(board);
    }
}


void insert_pattern(uint8_t pattern[][PATTERN_COL], int canvas[][COL], int pattern_row, int pattern_col, int start_row, int start_col) {
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
/*
void glider_test(int generation,int expected){
     init_canvas(board);
     insert_pattern(glider,board,PATTERN_ROW,PATTERN_COL,1,1);

     calculate_n_generation(board,generation);
     test_count_live_cell(board,expected);
 }
*/

/*
void beehive_test(int generation,int expected){
    init_canvas(board);
    insert_pattern(beehive,board,PATTERN_ROW,PATTERN_COL,1,1);

    calculate_n_generation(board,generation);
    test_count_live_cell(board,expected);
}
*/

void grower_test(int generation,int expected){
    
    init_canvas(board);
    insert_pattern(grower,board,PATTERN_ROW,PATTERN_COL,100,100);

    calculate_n_generation(board,generation);
    test_count_live_cell(board, expected);
    }

int main(){   
    // glider_test(50,5);
    // beehive_test(10,6);
    
    // grower_test(100,138);
    grower_test(0,20);
    grower_test(1,23);
    grower_test(2,24);
    grower_test(3,28);
    grower_test(4,31);
    grower_test(5,35);
    grower_test(6,35);
    grower_test(7,35);
    grower_test(8,43);
    grower_test(9,41);
    grower_test(10,49);
    return 0;
}