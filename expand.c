#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include "builtin.h"

int expand(char *orig, char *new, int newsize) {
  int i = 0; //tracking orig line
  int j = 0; //tracking envvar
  int n = 0; //tracking new line
  int err = 1;
  char append;

  //printf("ORIGINAL LINE: %s\n", orig);

  while (i < strlen(orig) && err != 0) {

    //begin by copying contents of orig to new
    if (orig[i] != '$') {
      append = orig[i];
      strcpy(new + n, &append);
    }

    //if we encounter a ${ start reading into envvar
    if (orig[i] == '$' && orig[i+1] == '{') {
      i += 2;
      j = 0;
      char envvar[32] = "";
      char value[1024] = "";

      //copy everything in ${...} to envvar
      while (orig[i] != '}' && orig[i] != '\0') {
        envvar[j] = orig[i];
        i++;
        j++;
      }
      envvar[j] = '\0';

      if (orig[i] == '\0') {
        err = 0;
        printf("error in processline, no '}' was found\n");
      }

      //printf("envvar: %s\n", envvar);
      //may need an error check here
      strcpy(value, getenv(envvar));

      //copy value into new starting at where we left off
      strcat(new, value);
      n += strlen(value);

    }

    i++; //advance orig line
    n++; //advance new line
    //printf("i = %d\n", i);
    //printf("new: %s\n", new);
  }

  char null[] = "\0";
  strcat(new, null);
  //printf("new[] contains: %s\n", new);

  return err;
}
