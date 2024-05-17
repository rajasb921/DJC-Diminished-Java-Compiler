/* LEXER for DISM */

%option yylineno
%option noyywrap

%{
  #define DEBUG 0
  typedef YYTOKENTYPE Token;
  Token scanned(Token t);
%}

digit	[0-9]
label	"#"[a-zA-Z0-9]+
wspace	[ \t\r\n]+

%%

add		{return scanned(ADD);}
sub		{return scanned(SUB);} 
mul		{return scanned(MUL);}
mov		{return scanned(MOV);}
lod		{return scanned(LOD);}
str		{return scanned(STR);}
jmp		{return scanned(JMP);}
beq		{return scanned(BEQ);}
blt		{return scanned(BLT);}
rdn		{return scanned(RDN);}
ptn		{return scanned(PTN);}
hlt		{return scanned(HLT);}
"-"?{digit}+    {return scanned(INT);}
{label}    	{return scanned(LABEL);}
":"		{return scanned(COLON);}
";"        { char c = input();
             while(c!='\n' && c!=EOF) c=input();
	   }
{wspace}   { /*skip whitespace*/ }
<<EOF>>    {return scanned(ENDOFFILE);}
.          { if(DEBUG) printf("\n");
             printf("Lex error on line %d: Illegal character %s\n",
                    yylineno,yytext);
             exit(-1);
           }
%%

/* process and return scanned token t */
Token scanned(Token t) {
  if(DEBUG==0) return t;
  switch(t) {
    case ADD: printf("ADD "); return t;
    case SUB: printf("SUB "); return t;
    case MUL: printf("MUL "); return t;
    case MOV: printf("MOV "); return t;
    case LOD: printf("LOD "); return t;
    case STR: printf("STR "); return t;
    case JMP: printf("JMP "); return t;
    case BEQ: printf("BEQ "); return t;
    case BLT: printf("BLT "); return t;
    case RDN: printf("RDN "); return t;
    case PTN: printf("PTN "); return t;
    case HLT: printf("HLT "); return t;
    case INT: printf("INT(%s) ",yytext); return t;
    case LABEL: printf("LABEL "); return t;
    case COLON: printf("COLON "); return t;
    case ENDOFFILE: printf("ENDOFFILE\n"); return t;
    default: printf("ERROR: invalid token in scanned().\n"); exit(-1);
  }
}

