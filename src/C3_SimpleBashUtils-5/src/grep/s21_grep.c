#include "s21_grep.h"

int main(int argc, char *argv[]) {
  if (argc > 1) {
    Countflag fflag = {0};
    search_opshion(argc, argv, &fflag);
    open_file(argc, argv, &fflag);
  } else {
    fprintf(
        stderr,
        "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
        "[-C[num]]\n\t[-e pattern] [-f file] [--binary-files=value] "
        "[--color=when]\n\t[--context[=num]] [--directories=action] [--label] "
        "[--line-buffered]\n\t[--null] [pattern] [file ...]\n");
  }
  return 0;
}

void search_opshion(int argc, char *argv[], Countflag *fflag) {
  char flag;
  while ((flag = getopt_long(argc, argv, "e:ivclnhsf:o?+", NULL, NULL)) !=
         EOF) {
    count_flags(flag, fflag);
  }
  if (fflag->f == 0 && fflag->e == 0) {
    snprintf(search_element, sizeof(search_element), "%s", argv[optind]);
    optind++;
  } else if (argc < optind) {
    fprintf(stderr,
            "grep: option requires an argument -- %c\nusage: grep "
            "[-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
            "[-C[num]]\n\t[-e "
            "pattern] [-f file] [--binary-files=value] "
            "[--color=when]\n\t[--context[=num]] [--directories=action] "
            "[--label] "
            "[--line-buffered]\n\t[--null] [pattern] [file ...]\n",
            flag);
  }
}

void count_flags(int flag, Countflag *fflag) {
  if (flag == 'e') {
    fflag->e = 1;
    element_entry(flag, fflag);
  } else if (flag == 'i')
    fflag->i = 1;
  else if (flag == 'v')
    fflag->v = 1;
  else if (flag == 'c')
    fflag->c = 1;
  else if (flag == 'l')
    fflag->l = 1;
  else if (flag == 'n')
    fflag->n = 1;
  else if (flag == 'h')
    fflag->h = 1;
  else if (flag == 's')
    fflag->s = 1;
  else if (flag == 'f') {
    fflag->f = 1;
    element_entry(flag, fflag);
  } else if (flag == 'o')
    fflag->o = 1;
  else {
    if (fflag->s != 1)
      fprintf(stderr,
              "grep: illegal option -- %c\nusage: cat [-benstuv] [file ...]\n",
              flag);
  }
}

void element_entry(char flag, Countflag *fflag) {
  if (flag == 'f') {
    open_file_f(optarg, fflag);
  } else if (flag == 'e') {
    template_entry(optarg);
  }
}

void open_file(int argc, char *argv[], Countflag *fflag) {
  for (int i = optind; i < argc; i++) {
    myfile = fopen(argv[i], "r");
    if (myfile == NULL) {
      if (fflag->s == 0)
        fprintf(stderr, "grep: %s: No such file or directory\n", argv[i]);
    } else {
      flag_handling(argv[i], fflag, argc);
      fclose(myfile);
    }
  }
}

void open_file_f(char *element_file, Countflag *fflag) {
  FILE *term = fopen(element_file, "r");
  if (term == NULL) {
    if (fflag->s == 0)
      fprintf(stderr, "grep: %s: No such file or directory\n", element_file);
  } else {
    char file[4096];
    while (fgets(file, 4096, term) != NULL) {
      int size = strlen(file) - 1;
      if (file[size] == '\n' && size > 0) file[size] = '\0';
      template_entry(file);
    }
    fclose(term);
  }
}

void template_entry(char *element) {
  if (search_element[0] == 0) {
    snprintf(search_element, sizeof(search_element), "%s", element);
  } else {
    char str[513];
    snprintf(str, sizeof(str), "|%s", element);
    strcat(search_element, str);
  }
}

void flag_handling(char *namefile, Countflag *fflag, int argc) {
  int totalfile = argc - optind;
  char file_element[4096];
  int count_flag_c = 0;
  for (int count_element = 1; fgets(file_element, 4096, myfile) != NULL;
       count_element++) {
    int bool = 0;
    if (fflag->o == 1)
      bool = flag_o(file_element, fflag, count_element, namefile, totalfile);
    else
      bool = regex(file_element, fflag);
    bool = flag_v(bool, fflag);
    if (bool == 0) count_flag_c++;
    if (bool == 0 && fflag->c != 1 && fflag->l != 1 &&
        (fflag->o != 1 || fflag->v == 1))
      print(count_element, namefile, fflag, file_element, totalfile);
  }
  if (fflag->c == 1 || fflag->l == 1 || fflag->o == 1) {
    if (fflag->l == 1 && fflag->c == 1 && count_flag_c != 0) count_flag_c = 1;
    if (totalfile >= 2 && fflag->h != 1 && fflag->c == 1)
      printf("%s:", namefile);
    if (fflag->c == 1) printf("%d\n", count_flag_c);
    if (fflag->l == 1 && count_flag_c > 0) printf("%s\n", namefile);
  }
}

void print(int count_element, char *txt, Countflag *fflag, char *file_element,
           int argc) {
  if (argc >= 2 && fflag->h != 1) printf("%s:", txt);
  if (fflag->n == 1) printf("%d:", count_element);
  enter(file_element);
}

void enter(char *file_element) {
  int bool = 1;
  for (int i = 0; i < (int)strlen(file_element); i++)
    if (file_element[i] == '\n') bool = 0;
  if (bool == 0)
    printf("%s", file_element);
  else
    printf("%s\n", file_element);
}

// Печатает только совпадающие (непустые) части совпавшей строки
int flag_o(char *file_element, Countflag *fflag, int count_element, char *txt,
           int argc) {
  int REG = fflag->i == 1 ? REG_ICASE : REG_EXTENDED;
  int bool = 1;
  regex_t search;
  int err;
  // должна скомпилировать регулярное выражение search_element
  err = regcomp(&search, search_element, REG);
  if (err == 0) {
    size_t nmatch = 2;
    regmatch_t pmatch[2];
    char *file = file_element;
    for (unsigned int l = 0; l != strlen(file_element) && err != -1; l++) {
      // сравнивает строку с скомпилированным регулярным выражением
      if (regexec(&search, file, nmatch, pmatch, 0) != 1) {
        int end = 0;
        for (size_t size = 0; size <= nmatch && pmatch[size].rm_so != -1;
             size++) {  // nmatch и pmatch используются для получения информации
                        // о местонахождении любых совпадени
          if (size == 0) end = pmatch[size].rm_eo;
          char buffer[strlen(file) + 1];
          strcpy(buffer, file);
          buffer[pmatch[size].rm_eo] = 0;
          if (fflag->c != 1 && fflag->l != 1 && fflag->v != 1) {
            if (argc >= 2 && fflag->h != 1) printf("%s:", txt);
            if (fflag->n == 1 && l == 0) printf("%d:", count_element);
            enter(buffer + pmatch[size].rm_so);
          }
          bool = 0;
        }
        file += end;
      } else
        err = -1;
    }
  }
  regfree(&search);
  return bool;
}

int regex(char *file_element, Countflag *fflag) {
  regex_t preg;
  regmatch_t pm;
  int err = 0;
  if (fflag->i == 1)
    err = regcomp(&preg, search_element, REG_ICASE);
  else
    err = regcomp(&preg, search_element, REG_EXTENDED);
  if (err != 0) {
    char buff[512];
    err = regerror(err, &preg, buff, sizeof(buff));
    if (fflag->s == 0) fprintf(stderr, "grep: %s\n", buff);
    err = -1;
  } else {
    err = regexec(&preg, file_element, 0, &pm, 0);
  }
  regfree(&preg);
  return err;
}

int flag_v(int bool, Countflag *fflag) {
  if (fflag->v == 1) {
    if (bool == 1)
      bool = 0;
    else if (bool == 0)
      bool = 1;
  }
  return bool;
}
