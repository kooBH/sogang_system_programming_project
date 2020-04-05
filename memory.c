#include "memory.h"


unsigned char memory[MAX_MEM];
unsigned int last_address=0;

void dump(struct command*cmd){
  unsigned int start,end;
  unsigned int start_offset,end_offset;
  int iter_offset;
  int i;
  if(!(cmd->num_arg<4 )){
    printf("ERROR::Invalid arguments\n");
    cmd->state=BAD_INPUT_TOO_MANY_ARGS;
    return;
  }
  /* arguments  */ 
  //dump
  if(cmd->num_arg==1){
    start = last_address;
    end   = start + 159;
  //dump start
  }else if(cmd->num_arg==2){
    start = strtol(cmd->arg[1],NULL,16);
    end = start + 159;
  }
  //dump start end
  else{
    start = strtol(cmd->arg[1],NULL,16);
    end = strtol(cmd->arg[2],NULL,16);
  }
  if(start > end ){
    printf("ERROR::Invalid arguments\n");
    cmd->state=BAD_INPUT_INVALID_ARGS;
    return;
  }
  // Out of memory boundary
  if(start > MAX_MEM || end > MAX_MEM){
    printf("ERROR::Invalid arguments\n");
    cmd->state=BAD_INPUT_INVALID_ARGS;
    return;
  }

  //
  /* display 
   * <hex address 16unit interval> <hex>*16 <ascii>*16
   * ... till end
   * */
  
  start_offset = (start/16) * 16;
  end_offset   = (end+16)/16 * 16;

  // out of memory boundary
  if(end_offset > MAX_MEM)
    end_offset = MAX_MEM;

  for(iter_offset=start_offset;
      iter_offset<end_offset;
      iter_offset+=16 ){

    printf("%05X ",iter_offset);

    for(i=0;i<16;i++){
      if(iter_offset+i<start)
        printf("   ");
      else if(iter_offset+i>end)
        printf("   ");
      else
        printf("%02X ",memory[start_offset+iter_offset+i]);
    }

    printf("; ");
    for(i=0;i<16;i++){
      if(memory[start_offset+iter_offset+i]>=0x20 
          && memory[start_offset+iter_offset+i]<=0x7E)
        printf("%c",memory[start_offset+iter_offset+i]);
      else
        printf(".");
    }
    printf("\n");
  }
  // remember last address 
  last_address = end+1;
  if(last_address == (1024*1024))
    last_address = 0;

  cmd->state=GOOD_INPUT;
}
void edit(struct command*cmd){
  unsigned int address,value;
  // edit address value
  if(!(cmd->num_arg==3 )){
    printf("ERROR::Invalid arguments\n");
    cmd->state=BAD_INPUT_TOO_MANY_ARGS;
    return;
  }

  address= strtol(cmd->arg[1],NULL,16);
  value= strtol(cmd->arg[2],NULL,16);
  //address boundary
  if(address <0 || address > MAX_MEM){
    printf("ERROR::Invalid arguments\n");
    cmd->state=BAD_INPUT_INVALID_ARGS;
    return;
  }
  //value boundary
  if(value<0 || value > 255){
    printf("ERROR::Invalid arguments\n");
    cmd->state=BAD_INPUT_INVALID_ARGS;
    return;
  }
  memory[address]=value;

  cmd->state= GOOD_INPUT;
}
void fill(struct command*cmd){
  unsigned int start,end,value;
  // fill start end value
  if(!(cmd->num_arg=4 )){
    printf("ERROR::Invalid arguments\n");
    cmd->state=BAD_INPUT_TOO_MANY_ARGS;
    return;
  }
  start= strtol(cmd->arg[1],NULL,16);
  end= strtol(cmd->arg[2],NULL,16);
  value= strtol(cmd->arg[3],NULL,16);
  //address boundary
  if(start<0 || start > MAX_MEM){
    printf("ERROR::Invalid arguments\n");
    cmd->state=BAD_INPUT_INVALID_ARGS;
    return;
  }
  if(end<0 || end > MAX_MEM){
    printf("ERROR::Invalid arguments\n");
    cmd->state=BAD_INPUT_INVALID_ARGS;
    return;
  }
  if(start > end ){
    printf("ERROR::Invalid arguments\n");
    cmd->state=BAD_INPUT_INVALID_ARGS;
    return;
  }
  //value boundary
  if(value<0 || value > 255){
    printf("ERROR::Invalid arguments\n");
    cmd->state=BAD_INPUT_INVALID_ARGS;
    return;
  }
  memset(memory + start,value,sizeof(unsigned char)*(end-start));


}
void reset(struct command*cmd){
  if(!(cmd->num_arg==1 )){
    printf("ERROR::Invalid arguments\n");
    cmd->state=BAD_INPUT_TOO_MANY_ARGS;
    return;
  }
  memset(memory,0,sizeof(unsigned char)*MAX_MEM);
  cmd->state=GOOD_INPUT;
}

