#include "command.h"
#include <stdio.h>


int check_input(char* input,struct command* cmd){
  unsigned int cnt=0;
  char* cur_str;

  strcpy(cmd->args,strtok(input,"\n"));
  cmd->arg[cnt]= strtok(input, " ,\n");
  cur_str = cmd->arg[cnt];
  while(cur_str){
    cnt++;
    if(cnt> MAX_ARG)
      return BAD_INPUT_TOO_MANY_ARGS;
    cmd->arg[cnt] = strtok(NULL," ,\n");
    cur_str = cmd->arg[cnt];
  }
  cmd->num_arg = cnt;
  return INPUT_CHECKING;
}


