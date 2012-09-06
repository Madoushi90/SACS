#include "asm.h"
#include "asm_impl.h"
#include "list.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 256

int asm_init(){return 0;}
int asm_cleanup(){return 0;}

struct asm_binary* asm_parse_file(const char* file){
  int i, toklen;
  char buf[MAX_LINE_LENGTH];
  char* token;

  FILE* fp = fopen(file,"r");
  //TODO: Check for NULL

  uint32_t loc = 0;
  asm_label label;
  struct list* label_list = create_list(16,sizeof(struct asm_label));

  while(!feof(fp)){
    fgets(buf,MAX_LINE_LENGTH,fp);
    
    //Strip Comments
    for(i=0;i<MAX_LINE_LENGTH;i++){
      if(buf[i] == '#'){
	buf[i] = 0;
	break;
      }
    }
    
    token = strtok(buf," \t\n\v\f\r");
    while(token != NULL){
      toklen = strlen(token);
      if(token[toklen-2] == ':'){
	token[toklen-2] = 0;
	label.loc = loc;
	strcpy(label.name,token);
	list_add(label_list,&label);
      }else if(token[0]='.'){
	token++;
	//TODO: Handle data/segment markers
      }else{
	//TODO: Handle instruction
      }
      token = strtok(NULL," \t\n\v\f\r");
    }
  }
}

int asm_free_binary(struct asm_binary* bin){
  return _delete_binary(bin);
}

struct asm_binary* _create_binary(){
  struct asm_binary* bin = (struct asm_binary*)malloc(sizeof(struct asm_binary));
  memset(bin,0,sizeof(struct asm_binary));
  return bin;
}

int _delete_binary(struct asm_binary* bin){
  if(bin->binary != 0){
    free(bin->binary);
  }
  free(bin);
}

struct asm_instr* _create_instr(uint8_t max_argc){
  struct asm_instr* instr = (struct asm_instr*)malloc(sizeof(struct asm_instr));
  memset(instr,0,sizeof(struct asm_instr));
  instr->argv = (struct asm_arg*)malloc(max_argc*sizeof(struct asm_arg));
  return instr;
}

int _delete_instr(struct asm_instr* instr){
  if(instr->argc != 0){
    for(int i=0; i < instr->argc; i++){
      _delete_arg(instr->argv[i]);
    }
  }
  if(instr->argv != 0){
    free(instr->argv);
  }
  free(instr);
  return 0;
}

struct asm_arg* _create_arg(){}
int _delete_arg(struct asm_arg* arg){}
