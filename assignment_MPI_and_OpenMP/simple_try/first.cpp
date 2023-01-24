#include <iostream>
#include "./headers/beehive.h"
#include "./headers/glider.h"
#include "./headers/grower.h"
using namespace std;
 
// change row and column value to set the canvas size
const int ROW = 1000;
const int COL = 1000;
// const int PATTERN_ROW=GLIDER_HEIGHT;
// const int PATTERN_COL=GLIDER_WIDTH;

// const int PATTERN_ROW=BEEHIVE_HEIGHT;
// const int PATTERN_COL=BEEHIVE_WIDTH;

const int PATTERN_ROW=GROWER_HEIGHT;
const int PATTERN_COL=GROWER_WIDTH;



// creates row boundary
void row_line()
{
    cout << endl;
    for (int i = 0; i < COL; i++) {
        cout << " -----";
    }
    cout << endl;
}

bool is_border(int i,int j){
    if (i == 0 || i == ROW-1 || j == 0 || j == COL-1){
        return true;
    }
    return false;
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
void print_2d_array(int a[][COL]) {
    row_line();
    for (int i = 0; i < ROW; i++) {
        cout << ":";
        for (int j = 0; j < COL; j++) {
            cout << "  " << a[i][j] << "  :";
        }
        row_line();
    }
}

void calculate_next_generation(int a[][COL], int b[][COL]) {
    int neighbour_live_cell;
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            neighbour_live_cell = count_live_neighbour_cell(a, i, j);
            if (a[i][j] == 1 && (neighbour_live_cell == 2 || neighbour_live_cell == 3)) {
                if (!is_border(i, j)) {
                    b[i][j] = 1;
                }
            } else if (a[i][j] == 0 && neighbour_live_cell == 3) {
                if (!is_border(i, j)) {
                    b[i][j] = 1;
                }
            } else {
                b[i][j] = 0;
            }
        }
    }
}
void init_canvas(int a[][COL]){
    int i, j;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
            a[i][j]=0;  
        }
    }
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
    insert_pattern(grower,a,PATTERN_ROW,PATTERN_COL,ROW,COL,300,300);

    calculate_n_generation(a,b,generation);
    test_count_live_cell(b,expected);
}

int main()
{   
    // glider_test(50,5);
    // beehive_test(10,6);
    
    // grower_test(100,138);
    grower_test(0,0);
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