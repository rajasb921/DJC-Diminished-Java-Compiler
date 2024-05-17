#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "interp.h"

/* This is a simple though somewhat inefficient implementation of the
   DISM VM (i.e., an interpreter/simulator of DISM instructions).
*/

/* fetch Nth instruction, stripping off label if one exists */
ASTree *getNthInstruction(ASTree *wholeTree, unsigned int n) {
  int count = 0;
  if(wholeTree==NULL) {
    printf("Internal Error: Invalid program\n");
    exit(-1);
  }
  ASTList *iter = wholeTree->childrenHead;
  for(; count<n; count++) {
    if(iter==NULL) {
      printf("Error: invalid instruction number %u sought\n",n);
      exit(-1);
    }
    iter = iter->next;
  }
  if(iter==NULL) {
    printf("Error: invalid instruction number %u sought\n",n);
    exit(-1);
  }
  ASTree *instr = iter->data;
  if(instr==NULL) {
    printf("Error: invalid instruction number %u found\n",n);
    exit(-1);
  }
  if(instr->typ==LABELED_INSTR) {
    if(instr->childrenHead==NULL || instr->childrenHead->next==NULL) {
      printf("Internal Error: invalid labeled instruction at location %u\n",n);
      exit(-1);
    }
    //skip past the label
    instr= instr->childrenHead->next->data;
  }
  return instr;
}

/* return number of first instruction with given label */
int getInstrNum(ASTree *wholeTree, char *labl) {
  int count = 0;
  if(wholeTree==NULL) {
    printf("Internal Error: invalid program\n");
    exit(-1);
  }
  ASTList *iter = wholeTree->childrenHead;
  // iterate through list of children, looking for label
  while(iter!=NULL) {
    if(iter->data==NULL) break;
    if(iter->data->typ==LABELED_INSTR) {
      if(iter->data->childrenHead==NULL || iter->data->childrenHead->data==NULL)
        break;
      if(strcmp(iter->data->childrenHead->data->text,labl)==0) return count;
    }
    count++;
    iter = iter->next;
  }
  printf("Error at PC %u: could not find instruction with label %s\n",PC,labl);
  exit(-1);
}

/* Return the first operand of a DISM instruction */
int getFirstOperand(ASTree *inst, ASTree *wholeTree) {
  if(inst==NULL || inst->childrenHead==NULL || inst->childrenHead->data==NULL) {
    printf("Internal Error: invalid DISM AST structure found at PC %u\n", PC);
    exit(-1);
  }
  if(inst->childrenHead->data->typ==INT_AST)
    return atoi(inst->childrenHead->data->text);
  if(inst->childrenHead->data->typ==LABEL_AST)
    return getInstrNum(wholeTree, inst->childrenHead->data->text);
  printf("Internal Error: unknown DISM AST structure found at PC %u\n", PC);
  exit(-1);
}

/* Return the second operand of a DISM instruction */
int getSecondOperand(ASTree *inst, ASTree *wholeTree) {
  if(inst==NULL || inst->childrenHead==NULL || inst->childrenHead->next==NULL
     || inst->childrenHead->next->data==NULL) {
    printf("Internal Error: invalid DISM AST structure found at PC %u\n", PC);
    exit(-1);
  }
  if(inst->childrenHead->next->data->typ==INT_AST)
    return atoi(inst->childrenHead->next->data->text);
  if(inst->childrenHead->next->data->typ==LABEL_AST)
    return getInstrNum(wholeTree, inst->childrenHead->next->data->text);
  printf("Internal Error: unknown DISM AST structure found at PC %u\n", PC);
  exit(-1);
}

/* Return the third operand of a DISM instruction */
int getThirdOperand(ASTree *inst, ASTree *wholeTree) {
  if(inst==NULL || inst->childrenHead==NULL || inst->childrenHead->next==NULL
     || inst->childrenHead->next->next==NULL
     || inst->childrenHead->next->next->data==NULL) {
    printf("Internal Error: invalid DISM AST structure found at PC %u\n", PC);
    exit(-1);
  }
  if(inst->childrenHead->next->next->data->typ==INT_AST)
    return atoi(inst->childrenHead->next->next->data->text);
  if(inst->childrenHead->next->next->data->typ==LABEL_AST)
    return getInstrNum(wholeTree, inst->childrenHead->next->next->data->text);
  printf("Internal Error: unknown DISM AST structure found at PC %u\n", PC);
  exit(-1);
}

/* interpret a single instruction */
void interpretInst(ASTree *nextInstr, ASTree *wholeTree) {
  if(nextInstr==NULL) {
    printf("Internal Error: invalid instruction reached at PC=%u\n",PC);
    exit(-1);
  }
  int o1, o2, o3, ldAddr, stAddr;
  switch(nextInstr->typ) {
    case ADD_AST:   //addition
      o1 = getFirstOperand(nextInstr, wholeTree);
      o2 = getSecondOperand(nextInstr, wholeTree);
      o3 = getThirdOperand(nextInstr, wholeTree);
      if(o1<0 || o2<0 || o3<0 || o1>=NUM_REGS || o2>=NUM_REGS || o3>=NUM_REGS) {
        printf("Error: Illegal register number at PC=%u\n",PC);
        exit(-1);
      }
      R[o1] = R[o2] + R[o3];
      PC++;
      break;
    case SUB_AST:  //subtraction
      o1 = getFirstOperand(nextInstr, wholeTree);
      o2 = getSecondOperand(nextInstr, wholeTree);
      o3 = getThirdOperand(nextInstr, wholeTree);
      if(o1<0 || o2<0 || o3<0 || o1>=NUM_REGS || o2>=NUM_REGS || o3>=NUM_REGS) {
        printf("Error: Illegal register number at PC=%u\n",PC);
        exit(-1);
      }
      if(R[o3]>R[o2]) R[o1]=0;
      else R[o1] = R[o2] - R[o3];
      PC++;
      break;
    case MUL_AST:  //multiplication
      o1 = getFirstOperand(nextInstr, wholeTree);
      o2 = getSecondOperand(nextInstr, wholeTree);
      o3 = getThirdOperand(nextInstr, wholeTree);
      if(o1<0 || o2<0 || o3<0 || o1>=NUM_REGS || o2>=NUM_REGS || o3>=NUM_REGS) {
        printf("Error: Illegal register number at PC=%u\n",PC);
        exit(-1);
      }
      R[o1] = R[o2] * R[o3];
      PC++;
      break;
    case MOV_AST:  //move immediate
      o1 = getFirstOperand(nextInstr, wholeTree);
      o2 = getSecondOperand(nextInstr, wholeTree);
      if(o1<0 || o2<0 || o1>=NUM_REGS) {
        printf("Error: Illegal operand at PC=%u\n",PC);
        exit(-1);
      }
      R[o1] = o2;
      PC++;
      break;
    case LOD_AST:  //load
      o1 = getFirstOperand(nextInstr, wholeTree);
      o2 = getSecondOperand(nextInstr, wholeTree);
      o3 = getThirdOperand(nextInstr, wholeTree);
      if(o1<0 || o2<0 || o1>=NUM_REGS || o2>=NUM_REGS) {
        printf("Error: Illegal register number at PC=%u\n",PC);
        exit(-1);
      }
      ldAddr = R[o2]+o3;
      if(ldAddr<0 || ldAddr>=DATA_SIZE) {
        printf("Error: out-of-range data-memory address %d at PC %u\n",
                ldAddr, PC);
        exit(-1);
      }
      R[o1] = M[ldAddr];
      PC++;
      break;
    case STR_AST:  //store
      o1 = getFirstOperand(nextInstr, wholeTree);
      o2 = getSecondOperand(nextInstr, wholeTree);
      o3 = getThirdOperand(nextInstr, wholeTree);
      if(o1<0 || o3<0 || o1>=NUM_REGS || o3>=NUM_REGS) {
        printf("Error: Illegal operand at PC=%u\n",PC);
        exit(-1);
      }
      stAddr = R[o1]+o2;
      if(stAddr<0 || stAddr>=DATA_SIZE) {
        printf("Error: out-of-range data-memory address %d at PC %u\n",
               stAddr, PC);
        exit(-1);
      }
      M[R[o1]+o2] = R[o3];
      PC++;
      break;
    case JMP_AST:  //jump
      o1 = getFirstOperand(nextInstr, wholeTree);
      o2 = getSecondOperand(nextInstr, wholeTree);
      if(o1<0 || o1>=NUM_REGS) {
        printf("Error: Illegal register number at PC=%u\n",PC);
        exit(-1);
      }
      if( (int)(R[o1])+o2 < 0) {
        printf("Error: Illegal PC value of %d set at instruction %u\n",
               R[o1]+o2,PC);
        exit(-1);
      }
      PC = R[o1] + o2;
      break;
    case BEQ_AST:  //branch if equal
      o1 = getFirstOperand(nextInstr, wholeTree);
      o2 = getSecondOperand(nextInstr, wholeTree);
      o3 = getThirdOperand(nextInstr, wholeTree);
      if(o1<0 || o2<0 || o3<0 || o1>=NUM_REGS || o2>=NUM_REGS) {
        printf("Error: Illegal operand at PC=%u\n",PC);
        exit(-1);
      }
      if(R[o1]==R[o2]) PC = o3;
      else PC++;
      break;
    case BLT_AST:  //branch if less-than
      o1 = getFirstOperand(nextInstr, wholeTree);
      o2 = getSecondOperand(nextInstr, wholeTree);
      o3 = getThirdOperand(nextInstr, wholeTree);
      if(o1<0 || o2<0 || o3<0 || o1>=NUM_REGS || o2>=NUM_REGS) {
        printf("Error: Illegal operand at PC=%u\n",PC);
        exit(-1);
      }
      if(R[o1] < R[o2]) PC = o3;
      else PC++;
      break;
    case RDN_AST:  //read a natural number
      o1 = getFirstOperand(nextInstr, wholeTree);
      if(o1<0 || o1>=NUM_REGS) {
        printf("Error: Illegal register number at PC=%u\n",PC);
        exit(-1);
      }
      int readIn;
      printf("Enter a natural number: ");
      if(scanf("%d", &readIn)!=1 || readIn<0) {
        printf("Error: only a natural number may be input\n");
        exit(-1);
      }
      R[o1] = readIn;
      PC++;
      break;
    case PTN_AST:  //print a natural number
      o1 = getFirstOperand(nextInstr, wholeTree);
      if(o1<0 || o1>=NUM_REGS) {
        printf("Error: Illegal register number at PC=%u\n",PC);
        exit(-1);
      }
      printf("%u\n",R[o1]);
      PC++;
      break;
    case HLT_AST:  //halt the DISM
      o1 = getFirstOperand(nextInstr, wholeTree);
      if(o1<0 || o1>=NUM_REGS) {
        printf("Error: Illegal register number at PC=%u\n",PC);
        exit(-1);
      }
      printf("Simulation completed with code %u at PC=%u.\n",R[o1],PC);
      exit(0);
    default:
      printf("Internal Error: unknown instruction at PC=%u",PC);
      exit(-1);
  }
}

/* Interpret the AST instructions */
void interpret(ASTree *t) {
  PC = 0;
  unsigned int i=0;
  for(; i<NUM_REGS; i++) R[i]=0;
  for(i=0; i<DATA_SIZE; i++) M[i]=0;
  if(t==NULL || t->typ!=INSTR_LIST) {
    printf("Internal Error: invalid program tree; can't begin to interpret\n");
    exit(-1);
  }
  while(1) {
    ASTree *nextInst = getNthInstruction(t,PC);
    if(debug) {
      printf("******interpreting the following ");
      printf("instruction at location %u: \n",PC);
      printAST(nextInst);
    }
    interpretInst(nextInst, t);
    if(debug) {
      printf("Register contents after executing this instruction:\n");
      printf("  0:%u 1:%u 2:%u 3:%u 4:%u 5:%u 6:%u 7:%u PC:%u\n",
             R[0],R[1],R[2],R[3],R[4],R[5],R[6],R[7],PC);
      printf("Nonzero values currently stored in memory:\n");
      char foundNonzeroMemoryVal = 0;
      for(i=0; i<DATA_SIZE; i++) {
        if(M[i]!=0) {
          foundNonzeroMemoryVal = 1;
          printf("  M[%u] = %u\n", i, M[i]);
        }
      }
      if(foundNonzeroMemoryVal==0) printf("  <none>\n");
      printf("\n");
    }
  }
}

