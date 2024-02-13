#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Countflag {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
} Countflag;

char search_element[4096];
FILE *myfile = NULL;
void search_opshion(int argc, char *argv[], Countflag *fflag);
void open_file(int argc, char *argv[], Countflag *fflag);
void element_entry(char flag, Countflag *fflag);
void open_file_f(char *element_file, Countflag *fflag);
void template_entry(char *element);
void count_flags(int flag, Countflag *fflag);
void flag_handling(char *namefile, Countflag *fflag, int argc);
void print(int count_element, char *txt, Countflag *fflag, char *file_element,
           int argc);
int flag_o(char *file_element, Countflag *fflag, int count_element, char *txt,
           int argc);
int regex(char *file_element, Countflag *fflag);
int flag_v(int bool, Countflag *fflag);
void enter(char *file_element);

#endif
