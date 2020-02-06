// https://open.kattis.com/problems/mayhem

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// reads input and allocates buffers
void start();

// frees buffers
void end();

// returns the total number of boms on the map minus the number of unique bomb networks
unsigned defuseAll();

// these two methods return the number of bombs in a bomb network starting a given row or column
// when bombs are found they are removed
unsigned defuseRow(unsigned r);
unsigned defuseCol(unsigned c);

int main() {
  start();
  printf("%u\n", defuseAll());
  end();
  return 0;
}

unsigned row_count, col_count;
bool *row_defused_flags, *col_defused_flags; 
char *input_buffer;

void start() {
  if (scanf("%u %u\n", &row_count, &col_count) != 2)
    exit(-1);
  unsigned input_size = row_count*(col_count+1);
  input_buffer = malloc(input_size);
  if (fread(input_buffer, 1, input_size, stdin) != input_size)
    exit(-2);
  row_defused_flags = calloc(row_count, sizeof(bool));
  col_defused_flags = calloc(col_count, sizeof(bool));;
}

void end() {
  free(input_buffer);
  free(row_defused_flags);
  free(col_defused_flags);
}

unsigned defuseAll() {
  unsigned sum = 0;
  for (unsigned r = 0; r < row_count; r++) {
    unsigned partial_sum = defuseRow(r);
    sum += partial_sum ? partial_sum-1 : 0;
  }
  return sum;
}

unsigned defuseRow(unsigned r) {
  if (row_defused_flags[r])
    return 0;
  else
    row_defused_flags[r] = true;
  unsigned sum = 0;
  for (unsigned c = 0; c < col_count; c++) {
    char *ptr = &input_buffer[c+r*(col_count+1)];
    if (*ptr == 'x') {
      *ptr = '.';
      sum += defuseCol(c)+1;
    }
  }
  return sum;
}

unsigned defuseCol(unsigned c) {
  if (col_defused_flags[c])
    return 0;
  else
    col_defused_flags[c] = true;
  unsigned sum = 0;
  for (unsigned r = 0; r < row_count; r++) {
    char *ptr = &input_buffer[c+r*(col_count+1)];
    if (*ptr == 'x') {
      *ptr = '.';
      sum += defuseRow(r)+1;
    }
  }
  return sum;
}
