#include "assemble.h"

struct opnode *optable[SIZE_HASH];

int insert_symbol(char* label_,int locctr_){
  struct symbol * cur_node;
  struct symbol * tmp_node;

  // init
  if(symtab == NULL){
    cur_node = (struct symbol*)malloc(sizeof(struct symbol));
    strcpy(cur_node->label,label_);
    cur_node->locctr=locctr_;
    cur_node->next=NULL;
    symtab = cur_node;
    // alphabet order insertion
  }else{
    cur_node=symtab;
    tmp_node = (struct symbol*)malloc(sizeof(struct symbol));
    strcpy(tmp_node->label,label_);
    tmp_node->locctr=locctr_;
    tmp_node->next=NULL;

    //check symtab
    if(strcmp(cur_node->label,tmp_node->label)>0){
      tmp_node->next=cur_node;
      symtab = tmp_node;
      return 0;
    }
   
    //check next of symtab
   if(cur_node->next==NULL){
      cur_node->next = tmp_node;
      tmp_node->next = NULL;
      return 0;
    }

    // Nth
    while( strcmp(cur_node->next->label,tmp_node->label)<0) {
     cur_node=cur_node->next;
     if(cur_node->next==NULL)
       break;
    }
    if(cur_node->next==NULL){
      cur_node->next = tmp_node;
      tmp_node->next = NULL;
    }else{
      tmp_node->next = cur_node->next;
      cur_node->next = tmp_node;
    }
      

    // Duplciated label
    if(strcmp(cur_node->label,tmp_node->label)==0)
      return -1;
  }
return 0;
}

void symbol(struct command*cmd){
  struct symbol * cur_node;
  if(cmd->num_arg>2){
    printf("ERROR::Invalid arguments\n");
    cmd->state=BAD_INPUT_TOO_MANY_ARGS;
    return;
  }
  cmd->state=GOOD_INPUT;
  cur_node = symtab;
  while(cur_node!=NULL){
    printf("       %-7s %04X\n",cur_node->label,cur_node->locctr);
    cur_node=cur_node->next;
  }
}

int get_symbol(char* str){
  struct symbol * cur_node;
  cur_node = symtab;
  while(cur_node!=NULL&&strcmp(str,cur_node->label)){
    cur_node=cur_node->next;
  }
  if(cur_node==NULL)
    return -1;
  return cur_node->locctr;
}

void assemble(struct command*cmd){
  FILE * lst,*obj,*input_file;
  char * str_lst,*str_obj; 
  int len;
  int size;
  int error;
    if(cmd->num_arg>2){
    printf("ERROR::Invalid arguments\n");
    cmd->state=BAD_INPUT_TOO_MANY_ARGS;
    return;
  }
  cmd->state=GOOD_INPUT;

  // file check
  input_file = fopen(cmd->arg[1],"r");
  if(!input_file){
    printf("ERROR::can't find %s\n",cmd->arg[1]);
    return;
  }else
    fclose(input_file);

  // *.lst *.obj
  str_lst = strdup(cmd->arg[1]);
  str_obj = strdup(cmd->arg[1]);
  len = strlen(cmd->arg[1]);

  str_lst[len-3]='l';
  str_lst[len-2]='s';
  str_lst[len-1]='t';
  
  lst = fopen(str_lst,"w");

  str_obj[len-3]='o';
  str_obj[len-2]='b';
  str_obj[len-1]='j';

  printf("PASS 1\n");
  error = assemble_sub(cmd->arg[1],1,NULL,&size);
  if(error !=-1){
    printf("ERROR occured at line %d in pass 1\n",error);
  }else{
    printf("PASS 2\n");
    error = assemble_sub(cmd->arg[1],2,lst,&size);
    if(error !=-1){
      printf("ERROR occured at line %d in pass 2\n",error);
    }
  }

  fclose(lst);

  if(error == -1){
  printf("OBJ\n");
  obj = fopen(str_obj,"w");
  lst = fopen(str_lst,"r");
  generate_obj(lst,obj,size);

  fclose(lst);
  fclose(obj);
  }
  else{
    remove(str_lst);
  }
}


int assemble_sub(char* file,int pass,FILE*lst,int*size){
  int lineno =0;
  int errorno=-1;
  int locctr=0;
  FILE * input;
  char buf[MAX_CHAR];
  char args[MAX_CHAR];
  char*arg[MAX_ASM_ARG];

  char *tmp_str;
  char tmp_str2[MAX_CHAR];

  int cnt;
  int base;
  int objcode;

  char*operate;
  char*operand_1;
  char*operand_2;
  int val_operand;

  int format;
  int extended=0;

  int i;

  input =fopen(file,"r");
  
  // check start
  fgets(buf, MAX_CHAR, input);
  strcpy(args,buf);
  buf[strlen(buf)-1]='\0';
  cnt = 0;
  arg[0]=strtok(args," ,\t\n");
  tmp_str = arg[0];
  while(tmp_str){
    cnt++;
    arg[cnt]=strtok(NULL," ,\t\n");
    tmp_str = arg[cnt];
  }
  if(strcmp(arg[1],"START")){
    return 0;
  }
  lineno+=5;
  if(pass==2){
       fprintf(lst,"%3d %04X %-24s\n",lineno,locctr,buf);
  }

  lineno+=5;
  while(fgets(buf, MAX_CHAR, input)!=NULL){

    operate=NULL;
    operand_1=NULL;
    operand_2=NULL;

    strcpy(args,buf);

    // elim next line char'\n'
    buf[strlen(buf)-1]='\0';
    // do not tokenizing comment
    if(buf[0]!='.'){
    // tokenizing
    cnt = 0;
    arg[0]=strtok(args," ,\t\n");
    tmp_str = arg[0];
    while(tmp_str){
      cnt++;
      arg[cnt]=strtok(NULL," ,\t\n");
      tmp_str = arg[cnt];
    }
    //execption for empty input at EOF
    if(cnt==0)
      continue;
    }
    
    if(strncmp(buf,"      ", 6)) { 
      // label exists
      operate = arg[1];
      operand_1 = arg[2];
      operand_2 = arg[3];
    }else{
      operate = arg[0];
      operand_1 = arg[1];
      operand_2 = arg[2];
    }
   /////////////////////////////////////////////////////////

   if(pass==1){
     //comment
     if(buf[0]=='.'){
       continue;
     }
     else if(strncmp(buf,"      ", 6)) { // label
       if(insert_symbol(arg[0],locctr)){
         //printf("ERROR:duplicated label\n");
         errorno = lineno;
         return errorno;
       }
     }
   }
   
   ////////////////////////////////////////////////////
   if(pass==2){
     extended =0; // for format 4 check
      //comment
     if(buf[0]=='.'){
       buf[0]=' ';
       fprintf(lst,"%3d      %-24s\n",lineno,buf);
       lineno+=5;
       continue;
     }
     else if(!strcmp(operate,"END")){ 
       fprintf(lst,"%3d      %-24s",lineno,buf);
     }else if(strcmp(operate,"BASE") == 0) { // if base
       fprintf(lst,"%3d      %-24s",lineno,buf);
       base =get_symbol(operand_1);
       if(base == -1) {
         //printf("ERROR::symbol can't be found\n");
         errorno = lineno;
         return errorno;
       }
     }
     else if(strcmp(operate, "RESB") == 0 || strcmp(operate, "RESW") == 0){
       fprintf(lst,"%3d %04X %-24s",lineno,locctr,buf);
       ;
     }else if(strcmp(operate, "BYTE") == 0 || strcmp(operate, "WORD") == 0) {
       fprintf(lst,"%3d %04X %-24s",lineno,locctr,buf);
       ;
       // if byte or word
       //HEX
       if(operand_1[0] == 'X') { // hexademical data
         if(strcmp(operate, "BYTE") == 0) { // byte
           strncpy(tmp_str2, buf+16, 2);
           tmp_str2[2] = '\0';
           objcode = (int)strtol(tmp_str2,NULL,16);
           fprintf(lst,"%02X", objcode);
         }
         //WORD
         else {
           strncpy(tmp_str2, buf+16, 6);
           tmp_str2[6] = '\0';
           objcode = (int)strtol(tmp_str2,NULL,16);
           fprintf(lst,"%06X", objcode);
         }
       } else if(operand_1[0] == 'C') { // char data
         for(i = 2; i < strlen(operand_1)-1; i++) {
           fprintf(lst,"%02X",operand_1[i]);
         }
       }
     }else { // if opcode
       fprintf(lst,"%3d %04X %-24s",lineno,locctr,buf);
       if(operate[0] == '+') {
         extended=1;
         ++operate;
       }
       if(operand_1&& (operand_1[0] == '#' || operand_1[0] == '@')) {
         ++operand_1;
       }					
       objcode = get_opcode(operate);
       format = find_opcode_format(operate);

       if(objcode == -1) {
         //printf("ERROR::can't find opcode of (%s)\n",operate);
         errorno = lineno;
         return errorno;
       }
       if(format==1) { // format 1
         fprintf(lst, "%02X", objcode);
       } else if(format==2) { // format 2
         objcode <<= 4;
         if(strcmp(operand_1, "A") == 0) {
           objcode += 0;
         }else if(strcmp(operand_1, "X") == 0) {
           objcode += 1;
         }else if(strcmp(operand_1, "L") == 0) {
           objcode += 2;
         }else if(strcmp(operand_1, "PC") == 0) {
           objcode += 8;
         }else if(strcmp(operand_1, "SW") == 0) {
           objcode += 9;
         }else if(strcmp(operand_1, "B") == 0) {
           objcode += 3;
         }else if(strcmp(operand_1, "S") == 0) {
           objcode += 4;
         }else if(strcmp(operand_1, "T") == 0) {
           objcode += 5;
         }else if(strcmp(operand_1, "F") == 0) {
           objcode += 6;
         }
         objcode <<= 4;
         if(operand_2 != NULL) { // two operand
           if(strcmp(operand_2, "A") == 0) {
             objcode += 0;
           }else if(strcmp(operand_2, "X") == 0) {
             objcode += 1;
           }else if(strcmp(operand_2, "L") == 0) {
             objcode += 2;
           }else if(strcmp(operand_2, "PC") == 0) {
             objcode += 8;
           }else if(strcmp(operand_2, "SW") == 0) {
             objcode += 9;
           }else if(strcmp(operand_2, "B") == 0) {
             objcode += 3;
           }else if(strcmp(operand_2, "S") == 0) {
             objcode += 4;
           }else if(strcmp(operand_2, "T") == 0) {
             objcode += 5;
           }else if(strcmp(operand_2, "F") == 0) {
             objcode += 6;
           }
         }						
         fprintf(lst, "%04X", objcode);
       } else { // format 3/4
         if(buf[14] == '@') { // indirect
           objcode |= 0x02;
         }else if(buf[14] == '#') { // immediate
           objcode |= 0x01;
         }else { // simple
           objcode |= 0x03;
         }
         objcode <<= 4;
          
         // no operand : RSUB
         if(operand_1 == NULL) {
           objcode <<= 12;
           fprintf(lst,"%06X",objcode);
         } else {
           // indexed
           if(operand_2 != NULL) {
								if(strcmp(operand_2,"X") == 0) {
									objcode |= 0x008;
								}
							}
           //constant
					if(buf[14] == '#' && '0' <= buf[15] && buf[15] <= '9') { 
								val_operand = strtol(operand_1, NULL, 10);
				  } 
          else {
								val_operand = get_symbol(operand_1);
                  if(val_operand == -1) {
                  //printf("ERROR::no symbol(%s)\n",operand_1);
									errorno = lineno;
                  return errorno;
								}
							}
              // format 4
							if(buf[7] == '+') { 
								objcode |= 0x01;
								objcode <<= 20;
								objcode += val_operand;
								fprintf(lst,"%08X",objcode);
							} else { // format 3
                // DIRECCT
								if(buf[14] == '#' && '0' <= buf[15] && buf[15] <= '9') { 
									objcode |= 0x0;
                  //PC
								} else if(locctr+3 - 2048 <= val_operand && val_operand <= locctr+3 + 2047) {
									objcode |= 0x2;
									val_operand -= (locctr+3);
                  // BASE
								} else if(base <= val_operand && val_operand <= base + 4095) {
									objcode |= 0x4;
									val_operand -= base;
								} else { // out of range
									//printf("error : out of range! use format 4!\n");
									errorno= lineno;
                  return errorno;
								}
                // shift for disp
								objcode <<= 12;
								val_operand &= 0x000FFF;
								objcode += val_operand;
                //printf("PC : %s %06X %04X\n",operate,objcode,val_operand);
								fprintf(lst,"%06X",objcode);
							}
						}
					}
				}
				fprintf(lst,"\n");
   }
   ////////////////////////////////////////////////////
   //LOCCTR
   
   if(!strcmp(operate,"RESW")){
     locctr += 3*strtol(operand_1,NULL,10);//word
   }else if(!strcmp(operate,"RESB")){
     locctr += strtol(operand_1,NULL,10);//byte
   }else if(strcmp(operate, "BYTE") == 0){
     if(operand_1[0] == 'X') { // hex
       locctr += 1;
     }else if(operand_1[0] == 'C') { //char
       locctr += strlen(operand_1)-3; // - C'<CHAR>'
     }
     // no locctr change
   }else if(!strcmp(operate, "BASE")|| !strcmp(operate, "END")){
     ;
     // format 4
   }else if(operate[0] == '+'||extended){ 
     locctr += 4;
   }else {
     format = find_opcode_format(operate);
     if(format == -1) {
       //printf("ERROR::Can't find such opcode\n");
       errorno= lineno;
       return errorno;
     }
     else{
       locctr += format;
     }
   }
   lineno+=5;
  }
  fclose(input);


  if(pass==2){
    *size = locctr;
  }
  return errorno;
}


void generate_obj(FILE*lst,FILE*obj,int size){
	int len_byte;
	char buf[MAX_CHAR], buf_source[61] = {0, };
	char locctr[5] = {0,}, locctr_keep[5]={0,},label[7] = {0,}, opcode[7] = {0,}, operand[10]={0,},source[21] = {0,};
  FILE *f_modi;

  // create temp file for modification
  f_modi = fopen(".modification.tmp","w");

  //START
	if(fgets(buf, MAX_CHAR, lst) != NULL) {
		strncpy(label, buf+9, 6);
		fprintf(obj, "H%6s000000%06X\n", label, size);
	}

	memset(buf_source,'\0',31);
	len_byte = 0;

	while (fgets(buf, MAX_CHAR, lst) != NULL){

		buf[strlen(buf)-1] = '\0';
		if(len_byte == 0) 
			strncpy(locctr, buf+4, 4);
	  strncpy(locctr_keep,buf+4, 4);
		strncpy(opcode, buf+16, 3);
    strncpy(operand,buf+23,9);
		strcpy(source, buf+33);

    //skip comment,base (no locctr),execpt END
    if(buf[4]==' ' && strcmp(opcode,"END"))
      continue;
    
    //format 4
    if(opcode[0]=='+'&&!(operand[0]=='@'||operand[0]=='#')){
        fprintf(f_modi,"M00%04X05\n",(int)strtol(locctr_keep,NULL,16)+0x1);
    }
    //end
		if(!strcmp(opcode, "END")) {
			fprintf(obj, "T00%4s%02X%s\n", locctr, len_byte, buf_source);
			break;
    }
    //variable
    else if(!strcmp(opcode, "RES")) {
      //alread new line
			if(len_byte == 0) {
				continue;
			}
      // new line for RES
      else { 
				fprintf(obj, "T00%4s%02X%s\n", locctr, len_byte, buf_source);
				memset(buf_source,'\0',31);
				len_byte = 0;
			}
		}else if(len_byte + (strlen(source)/2) > 0x1E) { // byte length > 0x1E
			fprintf(obj, "T00%4s%02X%s\n", locctr, len_byte, buf_source);
			memset(buf_source,'\0',31);
			strcat(buf_source, source);
			len_byte = (strlen(source)/2);
			strncpy(locctr, buf+4, 4);
		} else { 
			strcat(buf_source, source);
			len_byte += (strlen(source)/2);
		}
	}
  /* Modification */
  fclose(f_modi);
  f_modi = fopen(".modification.tmp","r");
	while (fgets(buf, MAX_CHAR, f_modi) != NULL){
    fprintf(obj,"%s",buf);
  }
  fclose(f_modi);
  // remove tmp file
  remove(".modification.tmp");
	fprintf(obj, "E%06X\n",0);
}
