#ifndef _H_OPCODE_
#define _H_OPCODE_

#include <stdlib.h>
#include <stdio.h>

#include "command.h"

#define SIZE_HASH 20
#define MAX_OPCODE 6
#define MAX_TYPE 4

struct opnode{
  unsigned int value; 
  char name[MAX_OPCODE];
  struct opnode *next;
};

struct opnode *optable[SIZE_HASH];

void create_optable();
void opcode(struct command*);
void opcodelist(struct command*);
unsigned int hash(char*);

#endif
