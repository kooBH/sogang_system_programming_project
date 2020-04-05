#include "opcode.h"

void create_optable(){
  FILE * fp;
  unsigned int value;
  int hash_index;
  char op[MAX_OPCODE];
  char type[MAX_TYPE];
  struct opnode* t1_node;
  struct opnode* t2_node;
  int i;

  fp = fopen("opcode.txt","r");

  for(i=0;i<SIZE_HASH;i++)
    optable[i]=NULL;

  while(fscanf(fp,"%X %s %s",&value,op,type)!=EOF){
    hash_index = hash(op);
    t1_node =optable[hash_index];
    //first bucket
    if(t1_node==NULL){
      t1_node = (struct opnode*)malloc(sizeof(struct opnode));
      t1_node->value = value;
      strcpy(t1_node->name,op);
      t1_node->next = NULL;
      optable[hash_index]=t1_node;
    }
    //collision 
    else{
      while(t1_node->next!=NULL)
        t1_node=t1_node->next;
      t2_node = (struct opnode*)malloc(sizeof(struct opnode));
      t2_node->value = value;
      strcpy(t2_node->name,op);
      t2_node->next = NULL;
      t1_node->next = t2_node;
    }
  }
  fclose(fp);
}

void opcode(struct command*cmd){
  int hash_index;
  struct opnode * t_node;
  if(cmd->num_arg>2){
    printf("ERROR::Invalid arguments\n");
    cmd->state=BAD_INPUT_TOO_MANY_ARGS;
    return;
  }
  hash_index = hash(cmd->arg[1]);
  t_node = optable[hash_index];
  while(strcmp(t_node->name,cmd->arg[1])){
    t_node=t_node->next;
    if(t_node ==NULL){
      printf("ERROR::Can't find such mnemonic\n");
      cmd->state=BAD_INPUT_INVALID_ARGS;
      return;
    }
  }
  printf("opcode is %X\n",t_node->value);

  
  cmd->state=GOOD_INPUT;
}

void opcodelist(struct command*cmd){
  int i;
  struct opnode *t1_node;
  if(cmd->num_arg>1){
    printf("ERROR::Invalid arguments\n");
    cmd->state=BAD_INPUT_TOO_MANY_ARGS;
    return;
  }
  for(i=0;i<SIZE_HASH;i++){
    t1_node = optable[i];
    printf("%d : ",i);
    while(t1_node!=NULL){
      printf("[%s,%X] ",t1_node->name,t1_node->value);
      t1_node = t1_node->next;
      if(t1_node!=NULL)
        printf("-> ");
    }
    printf("\n");
  }
  cmd->state=GOOD_INPUT;
}


unsigned int hash(char* str){
  //djb2 by Dan Bernstein. 
  unsigned int hash_value = 5381;
  while(*(str++))
    hash_value =((hash_value<<5)+hash_value)+*str;
  return hash_value % SIZE_HASH;
}
