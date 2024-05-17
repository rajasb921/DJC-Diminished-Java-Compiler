/* File ast.c
   Implementation of DISM ASTs for sim-dism
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"

void printError(char *reason) {
  printf("AST Error: %s\n", reason);
  exit(-1);
}

/* Create a new AST node of type t */
ASTree *newAST(ASTNodeType t, ASTree *child1,
       ASTree *child2, ASTree *child3, char *attribute) {
  ASTree *toReturn = malloc(sizeof(ASTree));
  if(toReturn==NULL) printError("malloc in newAST()");
  toReturn->typ = t;
  // create a linked list of children
  ASTList *childList = malloc(sizeof(ASTList));
  if(childList==NULL) printError("malloc in newAST()");
  childList->data = child1;
  childList->next = NULL;
  toReturn->childrenHead = childList;
  toReturn->childrenTail = childList;
  if(child2!=NULL) appendAST(toReturn, child2);
  if(child3!=NULL) appendAST(toReturn, child3);
  // finally set this node's attribute
  if(attribute==NULL) toReturn->text=NULL;
  else {
    char *copyStr = malloc(strlen(attribute)+1);
    if(copyStr==NULL) printError("malloc in newAST()");
    strcpy(copyStr, attribute);
    toReturn->text = copyStr;
  }
  return toReturn;
}

/* Append a new child AST node onto a parent's list of children */
ASTree *appendAST(ASTree *parent, ASTree *newChild) {
  if(parent==NULL) printError("append called with null parent");
  if(parent->childrenHead==NULL || parent->childrenTail==NULL)
    printError("append called with bad parent");
  if(newChild==NULL) printError("append called with null newChild");
  if(parent->childrenTail->data==NULL) //replace empty tail with new child
    parent->childrenTail->data = newChild;
  else { //tail of children is currently nonempty; append new child to list
    ASTList *newList = malloc(sizeof(ASTList));
    if(newList==NULL) printError("malloc in appendAST()");
    newList->data = newChild;
    newList->next = NULL;
    parent->childrenTail->next = newList;
    parent->childrenTail = newList;
  }
  return parent;
}

/* Print the type of this node and any node attributes */
void printNodeTypeAndAttribute(ASTree *t) {
  if(t==NULL) return;
  switch(t->typ) {
    case INSTR_LIST: printf("INSTR_LIST"); break;
    case LABELED_INSTR: printf("LABELED_INSTR"); break;
    case ADD_AST: printf("ADD_AST"); break;
    case SUB_AST: printf("SUB_AST"); break;
    case MUL_AST: printf("MUL_AST"); break;
    case MOV_AST: printf("MOV_AST"); break;
    case LOD_AST: printf("LOD_AST"); break;
    case STR_AST: printf("STR_AST"); break;
    case JMP_AST: printf("JMP_AST"); break;
    case BEQ_AST: printf("BEQ_AST"); break;
    case BLT_AST: printf("BLT_AST"); break;
    case RDN_AST: printf("RDN_AST"); break;
    case PTN_AST: printf("PTN_AST"); break;
    case HLT_AST: printf("HLT_AST"); break;
    case LABEL_AST: printf("LABEL_AST(%s)",t->text); break;
    case INT_AST: printf("INT_AST(%s)",t->text); break;
    default: printError("unknown node type in printNodeTypeAndAttribute()");
  }  
}

/* print tree in preorder */
void printASTree(ASTree *t, int depth) {
  if(t==NULL) return;
  printf("%d:",depth);
  int i=0;
  for(; i<depth; i++) printf("  ");
  printNodeTypeAndAttribute(t);
  printf("\n");
  //recursively print all children
  ASTList *childListIterator = t->childrenHead;
  while(childListIterator!=NULL) {
    printASTree(childListIterator->data, depth+1);
    childListIterator = childListIterator->next;
  }
}

/* Print the AST to stdout with indentations marking tree depth. */
void printAST(ASTree *t) { printASTree(t, 0); }

