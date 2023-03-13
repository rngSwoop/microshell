#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include "builtin.h"

int check_builtin_function(char *argZero)
{
  //initialize array of builtin functions
  //use: str-cmp(..., ...) in a loop to see if arg[0] is in our list of built in functions
  //if yes return true, no return false

  char *funcs[4] = {"cd", "exit", "envset", "envunset"};
  int i = 0;
  int numElements = sizeof(funcs)/sizeof(funcs[0]);
  bool found = false;

  while (i < numElements && found == false) {
    if (strcmp(argZero, funcs[i]) == 0) {
      found = true;
    }
    else {
      i++;
    }
  }

  return found;
}


int execute_builtin_function(char *argZero)
{
  //assignment description says function pointers are really nice to use here with
  //having each builtin command in its own function

  if (strcmp(argZero, "cd") == 0) {
    //future code to execute cd
    printf("execute cd\n");
  }
  else if (strcmp(argZero, "exit") == 0) {
    exit(0);
  }
  else if (strcmp(argZero, "envset") == 0) {
    //future code to execute envset
    printf("execute envset\n");
  }
  else if (strcmp(argZero, "envunset") == 0) {
    //future code to execute envunset
    printf("execute envunset\n");
  }

  return 0;
}
