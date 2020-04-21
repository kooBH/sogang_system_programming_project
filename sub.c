#include "sub.h"

struct string_node *head_history=NULL;
struct string_node *tail_history=NULL;


void help(struct command*cmd){
  if(cmd->num_arg>1){
    printf("ERROR::Invalid arguments\n");
    cmd->state=BAD_INPUT_TOO_MANY_ARGS;
    return;
  }
  cmd->state=GOOD_INPUT;
  printf("h[elp]\n");
  printf("d[ir]\n");
  printf("q[uit]\n");
  printf("hi[story]\n");
  printf("du[mp] [start, end]\n");
  printf("e[dit] address, value\n");
  printf("f[ill] start, end, value\n");
  printf("reset\n");
  printf("opcode mnemonic\n");
  printf("opcodelist\n");
  printf("assemble filename\n");
  printf("type filename\n");
  printf("symbol\n");
}

void dir(struct command*cmd){
  DIR *cur_dir = opendir(".");
  struct dirent *dp;
  int cnt_line =0;

  if(cmd->num_arg>1){
    printf("ERROR::Invalid arguments\n");
    cmd->state=BAD_INPUT_TOO_MANY_ARGS;
    return;
  }
  cmd->state=GOOD_INPUT;

  for (dp = readdir(cur_dir); dp != NULL; dp = readdir(cur_dir)){
    struct stat info;
    stat(dp->d_name,&info);
    
    // Is directory?
    if(S_ISDIR(info.st_mode))
      printf("\t%s/",dp->d_name);
    // +x?
    else if(info.st_mode & S_IXUSR)
      printf("\t%s*",dp->d_name);
    else
      printf("\t%s",dp->d_name);

    cnt_line++;
    if(cnt_line > 4){
      printf("\n");
      cnt_line=0;
    }
  }
  if(cnt_line!=0)
      printf("\n");

  closedir(cur_dir);
}

void history(struct command*cmd){
  int cnt = 1;
  struct string_node * cur_node;
  if(cmd->num_arg>1){
    printf("ERROR::Invalid arguments\n");
    cmd->state=BAD_INPUT_TOO_MANY_ARGS;
    return;
  }
  cur_node = head_history;
  while(cur_node){
    printf("\t%3d %s\n",cnt++,cur_node->str);
    cur_node = cur_node->next;
  }
  cmd->state = GOOD_INPUT;

}

void type(struct command *cmd){
	char buf[MAX_CHAR];
  if(cmd->num_arg>2){
    printf("ERROR::Invalid arguments\n");
    cmd->state=BAD_INPUT_TOO_MANY_ARGS;
    return;
  }
  FILE *fp;

	fp = fopen(cmd->arg[1], "r");
	if(fp != NULL) {
		while(fgets(buf, 81, fp) != NULL){
			printf("%s",buf);
		}
		fclose(fp);
	}
  else
    printf("ERROR::Can't find file '%s'",cmd->arg[1]);
  // can't find file
  cmd->state = GOOD_INPUT;
}

void add_history(struct command * cmd){
    struct string_node * cur_node;
  //init
  if(!head_history){
    head_history = (struct string_node*)malloc(sizeof(struct string_node));
    tail_history = head_history;
    head_history->next = NULL;
    strcpy(head_history->str,cmd->args);
  }else{
    cur_node = (struct string_node*)malloc(sizeof(struct string_node));
    tail_history->next = cur_node;
    cur_node->next = NULL;
    strcpy(cur_node->str,cmd->args);
    tail_history = cur_node;
  }
}

