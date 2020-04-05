#ifndef _H_SUB_
#define _H_SUB_

#include <dirent.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"

struct string_node{
  struct string_node * next;
  char str[MAX_CHAR];
};


//static struct string_node * head_history=NULL;

void help(struct command*);
void dir(struct command*);
void history(struct command*);

void add_history(struct command*);


#endif
