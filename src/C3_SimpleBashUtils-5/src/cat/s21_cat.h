#ifndef S21_CAT_H_
#define S21_CAT_H_

#include <getopt.h>
#include <stdio.h>
#include <string.h>

typedef struct Countflag {
  int n;
  int b;
  int E;
  int e;
  int s;
  int t;
  int T;
  int v;
} Countflag;

void open(int argc, char **argv);
int count_flag(int flag, Countflag *fflag);
void open_file(char *argv[], Countflag *fflag);
void print(FILE *myfile, int element, int count_s, int count_b,
           int *previous_character, Countflag *fflag);

#endif  //  S21_CAT_H_
