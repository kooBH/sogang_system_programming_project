#ifndef _H_ASSEMBLE_
#define _H_ASSEMBLE_

#include "command.h"
#include "opcode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ASM_ARG 4

enum{INST,INST_W_LABEL,BASE,COMMENT,VAR,CONST };

struct asm_node{
  int type;
  int format;
  int operate;
  char operand[MAX_CHAR];
  char *arg[MAX_ASM_ARG];
  char args[MAX_CHAR];
  int num_arg;
};

struct symbol{
  char label[MAX_CHAR];
  int locctr;
  struct symbol * prev;
  struct symbol * next;
};

struct symbol * symtab;

int insert_symbol(char*,int);
int get_symbol(char*);
void symbol(struct command*cmd);

void ams_token(char*input,struct asm_node*);

void assemble(struct command*);
int assemble_sub(char*file,int pass,FILE*lst,int*size);
void generate_obj(FILE*lst,FILE*obj,int size);



#endif
