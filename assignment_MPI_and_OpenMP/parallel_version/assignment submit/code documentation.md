# Documentation

## `int is_border(int i,int j)`

 * **Parameters:**
   * `i` — int row value
   * `j` — int column value
 * **Returns:** int -1 if the given postition is not a border and 1 if it is a border

## `int count_live_neighbour_cell(int *a, int r, int c)`

 * **Parameters:**
   * `a` — the board of the game
   * `r` — int row value# Documentation

## `int is_border(int i,int j)`

 * **Parameters:**
   * `i` — int row value
   * `j` — int column value
 * **Returns:** int -1 if the given postition is not a border and 1 if it is a border

## `int count_live_neighbour_cell(int *a, int r, int c)`

 * **Parameters:**
   * `a` — the board of the game
   * `r` — int row value
   * `c` — int column value
 * **Returns:** int, amount of alive neighbouring cells

## `int count_live_cell(int *a, int size)`

 * **Parameters:**
   * `a` — board of the game
   * `size` — amount of cells in the game
 * **Returns:** int, total amount of alive cells on the board

## `void init_board(int *a, int size)`

 * **Parameters:**
   * `a` — board to be initalized
   * `size` — number of cells in the board

## `void calculate_next_generation(int *a, int *b, int rows_per_proc)`

 * **Parameters:**
   * `a` — current board generation
   * `b` — next board generation
   * `rows_per_proc` — the number of rows to calculate

## `void insert_pattern(uint8_t pattern[][PATTERN_COL], int *board, int pattern_row, int pattern_col, int start_row, int start_col)`

 * **Parameters:**
   * `pattern` — uint8_t pattern to be insertet in the board
   * `board` — 
   * `pattern_row` — number of row in the pattern
   * `pattern_col` — number of columns in the pattern
   * `start_row` — row value of where the pattern should be inserted
   * `start_col` — column value of where the pattern should be inserted

## `void test_count_live_cell(int *a, int generation)`

 * **Parameters:**
   * `a` — game board
   * `generation` — 
t
   * `c` — int column value
 * **Returns:** int, amount of alive neighbouring cells

## `int count_live_cell(int *a, int size)`

 * **Parameters:**
   * `a` — board of the game
   * `size` — amount of cells in the game
 * **Returns:** int, total amount of alive cells on the board

## `void init_board(int *a, int size)`

 * **Parameters:**
   * `a` — board to be initalized
   * `size` — number of cells in the board

## `void calculate_next_generation(int *a, int *b, int rows_per_proc)`

 * **Parameters:**
   * `a` — current board generation
   * `b` — next board generation
   * `rows_per_proc` — the number of rows to calculate

## `void insert_pattern(uint8_t pattern[][PATTERN_COL], int *board, int pattern_row, int pattern_col, int start_row, int start_col)`

 * **Parameters:**
   * `pattern` — uint8_t pattern to be insertet in the board
   * `board` — 
   * `pattern_row` — number of row in the pattern
   * `pattern_col` — number of columns in the pattern
   * `start_row` — row value of where the pattern should be inserted
   * `start_col` — column value of where the pattern should be inserted

## `void test_count_live_cell(int *a, int generation)`

 * **Parameters:**
   * `a` — game board
   * `generation` — 
