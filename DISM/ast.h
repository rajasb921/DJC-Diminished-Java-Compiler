/* File ast.h
   Abstract-syntax-tree data structure for DISM
*/

#ifndef AST_H
#define AST_H

/* define types of AST nodes */
typedef enum {
  INSTR_LIST, LABELED_INSTR, ADD_AST, SUB_AST, MUL_AST,
  MOV_AST, LOD_AST, STR_AST, JMP_AST, BEQ_AST, BLT_AST,
  RDN_AST, PTN_AST, HLT_AST, LABEL_AST, INT_AST
} ASTNodeType;

/* define a list of AST nodes */
typedef struct astlistnode {
  struct astnode *data;
  struct astlistnode *next;
} ASTList;

/* define the actual AST nodes */
typedef struct astnode {
  ASTNodeType typ;
  /* list of children nodes: */
  ASTList *childrenHead;
  ASTList *childrenTail;
  /* node attribute: */
  char *text;
} ASTree;

/* METHODS TO CREATE AND MANIPULATE THE AST */

/* Create a new AST node of type t */
ASTree *newAST(ASTNodeType t, ASTree *child1, 
  ASTree *child2, ASTree *child3, char *attribute);

/* Append a new child AST node onto a parent's list of children */
ASTree *appendAST(ASTree *parent, ASTree *newChild);

/* Print the AST to stdout with indentations marking tree depth. */
void printAST(ASTree *t);

#endif

