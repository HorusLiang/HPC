#include <iostream>
#include "./headers/beehive.h"

using namespace std;
 
// change row and column value to set the canvas size
const int row = 5;
const int col = 4;
 
// creates row boundary
int row_line()
{
    cout << endl;
    for (int i = 0; i < col; i++) {
        cout << " -----";
    }
    cout << endl;
}

bool is_border(int i,int j){
    if (i == 0 || i == row-1 || j == 0 || j == col-1){
        return true;
    }
    return false;
}
 
// returns the count of alive neighbours
int count_live_neighbour_cell(int a[row][col], int r, int c)
{
    int i, j, count = 0;
    for (i = r - 1; i <= r + 1; i++) {
        for (j = c - 1; j <= c + 1; j++) {
            if ((i == r && j == c) || (i < 0 || j < 0)
                || (i >= row || j >= col)) {
                continue;
            }
            if (a[i][j] == 1) {
                count++;
            }
        }
    }
    return count;
}

void calculate_next_generation(int a[][col], int b[][col]) {
    int neighbour_live_cell;
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
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

int main()
{
    for (int i = 0; i < BEEHIVE_HEIGHT; i++) {
        for (int j = 0; j < BEEHIVE_WIDTH; j++) {
            std::cout << (int)beehive[i][j] << " ";
        }
        std::cout << std::endl;
    }

    int a[row][col], b[row][col];
    int i, j;
 
    // generate matrix canvas with random values (live and
    // dead cells)
    for (i = 0; i < row; i++) {
        for (j = 0; j < col; j++) {
            if (is_border(i,j)) {
                a[i][j] = 0;
            } else {
                a[i][j] = rand() % 2;
            }
        }
    }
 
    // print array matrix
    cout << "Initial Stage:";
    row_line();
    for (i = 0; i < row; i++) {
        cout << ":";
        for (j = 0; j < col; j++) {
            cout << "  " << a[i][j] << "  :";
        }
        row_line();
    }
 
    // next canvas values based on live neighbour count
    calculate_next_generation(a,b);
 
    // print next generation
    cout << "\nNext Generation:";
    row_line();
    for (i = 0; i < row; i++) {
        cout << ":";
        for (j = 0; j < col; j++) {
            cout << "  " << b[i][j] << "  :";
        }
        row_line();
    }
 
    return 0;
}