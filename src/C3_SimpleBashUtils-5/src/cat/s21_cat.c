#include "s21_cat.h"

int main(int argc, char **argv) {
  if (argc > 1) {
    open(argc, argv);
  } else {
    fprintf(stderr, "error");
  }
  return 0;
}

void open(int argc, char **argv) {
  int err = 0;
  struct option long_options[] = {
      {"number-nonblank", 0, NULL, 'b'}, {"number", 0, NULL, 'n'},
      {"squeeze-blank", 0, NULL, 's'},   {"show-ends", 0, NULL, 'E'},
      {"show-tabs", 0, NULL, 'T'},       {NULL, 0, NULL, 0}};
  Countflag fflag = {0};
  while (optind < argc && err != 1) {
    int flag = 0;
    while ((flag = getopt_long(argc, argv, "+benstvTE", long_options, NULL)) !=
           EOF) {
      if (count_flag(flag, &fflag) == 1) err = 1;
    }
    open_file(argv, &fflag);
    optind++;
  }
}

int count_flag(int flag, Countflag *fflag) {
  int err = 0;
  if (flag == 'n')
    fflag->n = 1;
  else if (flag == 'b')
    fflag->b = 1;
  else if (flag == 'e') {
    fflag->e = 1;
    fflag->v = 1;
  } else if (flag == 'E')
    fflag->e = 1;
  else if (flag == 's')
    fflag->s = 1;
  else if (flag == 't') {
    fflag->v = 1;
    fflag->t = 1;
  } else if (flag == 'T')
    fflag->t = 1;
  else if (flag == 'v')
    fflag->v = 1;
  else {
    fprintf(stderr,
            "cat: illegal option -- %c\nusage: cat [-benstuv] [file ...]",
            flag);
    err = 1;
  }
  return err;
}
