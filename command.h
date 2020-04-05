#ifndef _H_COMMAND_
#define _H_COMMAND_

#include <string.h>

#define MAX_CHAR 256
#define MAX_ARG 5

struct command{
  unsigned int num_arg;
  char *arg[MAX_ARG];  
  char args[MAX_CHAR];
  int state; //did command performed well?
};

int check_input(char*,struct command*);

enum{
  INPUT_CHECKING,
  GOOD_INPUT,
  BAD_INPUT_TOO_MANY_ARGS,
  BAD_INPUT_UNKOWN,
  BAD_INPUT_INVALID_ARGS
};

#endif
