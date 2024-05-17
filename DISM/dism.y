/* DISM PARSER */

%code provides {
  #include <stdio.h>
  #include "lex.yy.c"
  #include "ast.h"
  #include "interp.h"

  /* Symbols in this grammar are represented as ASTs */ 
  #define YYSTYPE ASTree *

  /* Declare global AST for entire program */
  ASTree *pgmAST;
  
  /* Function for printing generic syntax-error messages */
  void yyerror(const char *str) {
    printf("Syntax error on line %d at token %s\n",yylineno,yytext);
    printf("Exiting simulator.\n");
    exit(-1);
  }
}

%token ADD SUB MUL MOV LOD STR JMP BEQ BLT RDN PTN HLT INT LABEL COLON
%token ENDOFFILE

%start pgm

%%

pgm : instlist ENDOFFILE 
      { pgmAST = $1;  return 0; }
    ;

instlist : instlist label COLON inst 
           { $$ = appendAST($1, newAST(LABELED_INSTR,$2,$4,NULL,NULL)); }
         | instlist inst 
           { $$ = appendAST($1, $2); }
         | 
           { $$ = newAST(INSTR_LIST,NULL,NULL,NULL,NULL); }
         ;

inst : ADD intt intt intt 
       { $$ = newAST(ADD_AST,$2,$3,$4,NULL); }
     | SUB intt intt intt 
       { $$ = newAST(SUB_AST,$2,$3,$4,NULL); }
     | MUL intt intt intt 
       { $$ = newAST(MUL_AST,$2,$3,$4,NULL); }
     | MOV intt intt 
       { $$ = newAST(MOV_AST,$2,$3,NULL,NULL); }
     | LOD intt intt intt 
       { $$ = newAST(LOD_AST,$2,$3,$4,NULL); }
     | STR intt intt intt 
       { $$ = newAST(STR_AST,$2,$3,$4,NULL); }
     | JMP intt intt 
       { $$ = newAST(JMP_AST,$2,$3,NULL,NULL); }
     | BEQ intt intt intt 
       { $$ = newAST(BEQ_AST,$2,$3,$4,NULL); }
     | BLT intt intt intt 
       { $$ = newAST(BLT_AST,$2,$3,$4,NULL); }
     | RDN intt 
       { $$ = newAST(RDN_AST,$2,NULL,NULL,NULL); }
     | PTN intt 
       { $$ = newAST(PTN_AST,$2,NULL,NULL,NULL); }
     | HLT intt
       { $$ = newAST(HLT_AST,$2,NULL,NULL,NULL); }
     ;

label : LABEL 
	{ $$ = newAST(LABEL_AST,NULL,NULL,NULL,yytext); }
      ;

intt : INT 
       { $$ = newAST(INT_AST,NULL,NULL,NULL,yytext); }
     | label 
       { $$=$1; }
     ;

%%

int main(int argc, char **argv) {
  if(argc!=2 && argc!=3) {
    printf("Usage: sim-dism filename [mode]\n");
    printf("* If nothing is given for mode, the simulator runs as normal.\n");
    printf("* If anything is given for mode, the simulator prints useful\n");
    printf("  debugging information.\n");
    exit(-1);
  }
  yyin = fopen(argv[1],"r");
  if(yyin==NULL) {
    printf("ERROR: could not open file %s\n",argv[1]);
    exit(-1);
  }
  debug=0;
  if(argc==3) debug=1; 

  /* parse and interpret the program */
  yyparse();
  if(debug) {
    printf("****** begin abstract syntax tree for DISM program ********\n");
    printAST(pgmAST);
    printf("******* end abstract syntax tree for DISM program *********\n\n");
  }
  interpret(pgmAST);
  return 0;
}

