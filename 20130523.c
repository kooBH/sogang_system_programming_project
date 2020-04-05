#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "opcode.h"
#include "sub.h"

int main(){
  int flag_run = 1;
  int exe_history=0;
  char input[MAX_CHAR];

  create_optable();

  while(flag_run){
    printf("sicsim>");
    struct command cmd;
    fgets(input, MAX_CHAR, stdin);
    
    //parsing arguments of input command
    check_input(input,&cmd);
    if(cmd.state == BAD_INPUT_TOO_MANY_ARGS){
      printf("ERROR::invalid args\n");
      continue;
    }

    if(!strcmp(cmd.arg[0],"h")||!(strcmp(cmd.arg[0],"help"))){
      help(&cmd);
    }
    else if(!strcmp(cmd.arg[0],"d")||!(strcmp(cmd.arg[0],"dir"))){
      dir(&cmd);
    }
    else if(!strcmp(cmd.arg[0],"q")||!(strcmp(cmd.arg[0],"quit"))){
      flag_run = 0;
    }
    else if(!strcmp(cmd.arg[0],"hi")||!(strcmp(cmd.arg[0],"history"))){
      cmd.state=GOOD_INPUT;
      exe_history = 1;
    }
    else if(!strcmp(cmd.arg[0],"du")||!(strcmp(cmd.arg[0],"dump"))){
      dump(&cmd);
    }
    else if(!strcmp(cmd.arg[0],"e")||!(strcmp(cmd.arg[0],"edit"))){
      edit(&cmd);
    }
    else if(!strcmp(cmd.arg[0],"f")||!(strcmp(cmd.arg[0],"fill"))){
      fill(&cmd);
    }
    else if(!(strcmp(cmd.arg[0],"reset"))){
      reset(&cmd);
    }
    else if(!(strcmp(cmd.arg[0],"opcode"))){
      opcode(&cmd);
    }
    else if(!(strcmp(cmd.arg[0],"opcodelist"))){
      opcodelist(&cmd);
    }
    else{
      cmd.state=BAD_INPUT_UNKOWN;
    }

    //command well performed
    if(cmd.state==GOOD_INPUT){
      add_history(&cmd);
    }
    // run histroy after add  "history" in history.
    if(exe_history){
      history(&cmd);
      exe_history = 0;
    }
    
  }

  return 0;
}
                        
