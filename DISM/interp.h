/* File interp.h
   Implementation of DISM VM
   (i.e., interpreter for DISM programs)
*/
#ifndef INTERP_H
#define INTERP_H

#include "ast.h"

/* Interpret the AST instructions */
void interpret(ASTree *t);

/* GLOBAL INTERPRETER STATE */
#define NUM_REGS 8
#define DATA_SIZE 65536
unsigned int PC;  // program counter
unsigned int R[NUM_REGS]; // registers
unsigned int M[DATA_SIZE]; // data memory

/* GLOBAL DEBUG-MODE FLAG */
int debug;

#endif

