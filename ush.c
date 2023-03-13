/* Program written by Garrett King */

/* CS 352 -- Micro Shell!
 *
 *   Sept 21, 2000,  Phil Nelson
 *   Modified April 8, 2001
 *   Modified January 6, 2003
 *   Modified January 8, 2017
 *
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include "builtin.h"
/* Constants */
#define LINELEN 1024
/* Prototypes */
void processline (char *line);
char** arg_parse (char *line, int *argcptr);

/* Shell main */
int main (void)
{
  char   buffer [LINELEN];
  int    len;
  while (1) {
    /* prompt and get line */
    fprintf (stderr, "%% ");
    if (fgets (buffer, LINELEN, stdin) != buffer)
      break;
    /* Get rid of \n at end of buffer. */
    len = strlen(buffer);
    if (buffer[len-1] == '\n')
      buffer[len-1] = 0;
    /* Run it ... */
    processline (buffer);
  }
  if (!feof(stdin))
    perror ("read");
  return 0; /* Also known as exit (0); */
}
void processline (char *line)
{
  pid_t  cpid;
  int    status;

  int* argc = malloc(sizeof(int));

    /* Pass line[] and new[] to expand() */
  char new[1024] = "";
  int err = expand(line, new, 1024);
  if (err == 0) {
    printf("error in expand()\n");
    exit(0);
  }

  char** args = arg_parse(new, argc);
  if (*argc == 0) {
    free(args);
    free(argc);
    return;
  }

  /* Check if args[0] is builtin function */
  if (check_builtin_function(args[0])) {
    execute_builtin_function(args[0]);
  }
  else {
    /* Start a new process to do the job. */
    cpid = fork();
    if (cpid < 0) {
      /* Fork wasn't successful */
      perror ("fork");
      return;
    }

    /* Check for who we are! */
    if (cpid == 0) {
      /* We are the child! */
      execvp (args[0], &args[0]);
      /* execlp reurned, wasn't successful */
      perror ("exec");
      fclose(stdin);  // avoid a linux stdio bug
      exit (127);
    }

    /* Have the parent wait for child to complete */
    if (wait (&status) < 0) {
      /* Wait wasn't successful */
      perror ("wait");
    }
  }
}

char ** arg_parse(char *line, int *argcptr) {
    *argcptr = 0;
    int i = 0;
    while (i < strlen(line)) {
        //if we encounter an opening (")
        if (i < strlen(line) && line[i] == '"') {
            i++;
            //continue until closing (") is found or line ends
            while (i < strlen(line) && line[i] != '"') {
                i++;
            }
            //if the closing (") we found is followed by a space
            //VALID ARG
            //if the closing (") we found is the final char
            //VALID ARG
            if (line[i] == '"' && (line[i+1] == ' ' || i+1 == strlen(line))) {
              (*argcptr)++;
              i++;
            }
        }

        //if the current char is a space, continue to next char
        while (i < strlen(line) && line[i] == ' ') {
            i++;
        }

        //if the current char is not a space, we have found a new arg
        //increment argc and continue to end of this argument
        if (i < strlen(line) && line[i] != '"') {
            (*argcptr)++;
            //skip to end of this arg
            while (i < strlen(line) && line[i] != ' ') {
                i++;
                //if we find an opening (") in the middle of our arg,
                //we want to skip all characters between here and the close (")
                if (line[i] == '"') {
                    i++;
                    while (i < strlen(line) && line[i] != '"') {
                        i++;
                    }
                }
            }
        }
    }

    char **args = malloc(sizeof(char *) * (*argcptr+1));
    args[*argcptr] = NULL;

    for (int j = 0; j < *argcptr; j++) {
        int s = 0;
        while (line[0] == ' ') {
            line++;
        }
        while (line[s] != ' ' && line[s] != '\0') {
            s++;
        }
        *(args+j) = malloc(sizeof(char)*(s+1));
        strcpy(*(args+j), line);
        *(*(args+j)+s) = '\0';
        line += s;
    }
    return args;
}
