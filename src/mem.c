#include "mem.h"

#define MINIMUM_SIZE 8

typedef enum { FREE=0, SPLIT, FULL } NodeState;

typedef struct {
  NodeState state;
  uint32_t size;
  uint32_t loc;

  MemoryNode* parent;
  MemoryNode* left;
  MemoryNode* right;

  uint8_t* mem;
} MemoryNode;

typedef enum { INSUFFICIENT_SPACE=1 } AllocError;

MemoryNode* _create_node(uint32_t size, uint32_t loc, MemoryNode* parent);
int _delete_node(MemoryNode* node);
uint32_t _dyn_alloc(MemoryNode* node, uint32_t size);

MemoryNode* root;

int mem_init(){
  root = _create_node(32,0);
  return 0;
}

int mem_cleanup(){
  //No cleanup should really be necessary
  return 0;
}

uint32_t mem_dynamic_alloc(uint32_t size){
  for(uint32_t bit_size=0; size != 0; bit_size++, size >>= 1); //TESTME
  if(bit_size == 0){
    return 0;
  }
  return _dyn_alloc(root,bit_size);
}

int mem_static_alloc(uint32_t size, uint32_t loc){}
int mem_free(uint32_t loc){}

MemoryNode* _create_node(uint32_t size, uint32_t loc, MemoryNode* parent){
  MemoryNode* node = (MemoryNode*)malloc(sizeof(MemoryNode));
  memset(node,0,sizeof(MemoryNode));
  node->size = size;
  node->loc = loc;
  node->parent = parent;
  return node;
}

int _delete_node(MemoryNode* node){
  if(node->left != 0){
    _delete_node(node->left);
  }
  if(node->right != 0){
    _delete_node(node->right);
  }
  if(node->mem != 0){
    free(node->mem);
  }
  free(node);
}

uint32_t _dyn_alloc(MemoryNode* node, uint32_t size){
  uint32_t res;
  switch(node->status){
  case FREE:
    if(node->size == size || node->size == MINIMUM_SIZE){
      node->status = FULL;
      node->mem = malloc(1 << (size-1));
      return node->loc;
    }else{
      node->state = SPLIT;
      node->left = _create_node(node->size-1,node->loc,node);
      return _dyn_alloc(node->left,size);
    }

  case SPLIT:
    if(node->left == 0){
      node->left = _create_node(node->size-1,node->loc,node);
      return _dyn_alloc(node->left,size);
    }else{
      res = _dyn_alloc(node->left,size);
      if(res == INSUFFICIENT_SPACE){
	if(node->right == 0){
	  node->right = _create_node(node->size-1,node->loc+(1<<(node->size-1)),node); //TESTME
	  return _dyn_alloc(node->right,size);
	}else{
	  return _dyn_alloc(node->right,size);
	}      
      }else{
	return res;
      }
    }

  case FULL:
    return INSUFFICIENT_SPACE;
  }
  return 0;
}
