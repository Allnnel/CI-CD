#include "s21_cat.h"

void open_file(char *argv[], Countflag *fflag) {
  FILE *myfile = fopen(argv[optind], "r");
  if (myfile == NULL) {
    fprintf(stderr, "cat: No such file or directory");
  } else {
    int element = 0;
    int count_s = 1, count_b = 1;
    int previous_character = 0;
    print(myfile, element, count_s, count_b, &previous_character, fflag);
    fclose(myfile);
  }
}

void print(FILE *myfile, int element, int count_s, int count_b,
           int *previous_character, Countflag *fflag) {
  while ((element = getc(myfile)) != EOF) {
    if (fflag->s == 1) {
      if (element == '\n' && *previous_character == '\n') count_s += 1;
      if (element != '\n') count_s = 0;
    }
    if (count_s < 2) {
      if (fflag->b == 1) {
        fflag->n = 0;
        if (element != '\n' && (*previous_character == '\n' || count_b == 1))
          printf("%6d\t", count_b++);
      }
      if (fflag->n == 1) {
        if (count_b == 1 || *previous_character == '\n')
          printf("%6d\t", count_b++);
        *previous_character = element;
      }
      if (fflag->e == 1) {
        if (element == '\n') printf("$");
      }
      if (fflag->t == 1) {
        if (element == '\t') printf("^I");
      }
      if (fflag->v == 1) {
        if (element != 10 && element != 9 && element <= 31) {
          element = element + 64;
          printf("^%c", element);
        } else if (element == 127) {
          element = '?';
          printf("^?");
        } else if (fflag->t == 1 && element != '\t')
          printf("%c", element);
        else if (fflag->t == 0)
          printf("%c", element);
      }
      if ((element != '\t' || fflag->t == 0) && fflag->v == 0)
        printf("%c", element);
    }
    *previous_character = element;
  }
}
