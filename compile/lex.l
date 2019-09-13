%{
	# include "ast.h"  // 这个include放在define前
	# define YYSTYPE TreeNode*
	# include <stdio.h>
	# include "y.tab.h"

extern "C"
{
	int yywrap(void);
	int yylex();
}

int yywrap(void)
{
	return 1;
}

char *token_value = NULL;
%}

digit 					[0-9]+
identifier 				[a-zA-Z_][a-zA-Z0-9_]*
chars 					[a-zA-Z0-9_\\]*

%%

"if"					{return IF;}
"else"					{return ELSE;}
"def"					{return DEF;}
"for"					{return FOR;}
"in"					{return IN;}
"break" 				{return BREAK;}
"continue" 				{return CONTINUE;}
"return"  				{return RETURN;}
"+"						{return ADD;}
"-"						{return MINUS;}
"*"						{return TIMES;}
"/"						{return DIV;}
"%"						{return MOD;}
{digit}					{token_value=strdup(yytext); return NUMBERS;}
{identifier}			{token_value=strdup(yytext); return ID;}
{chars} 				{token_value=strdup(yytext); return STRING;}
["]						{return QUOTE_MARK;}
","						{return COMMA;}
"=" 					{return ASSIGN;}
"==" 					{return EQ;}
">"  					{return GREATER;}
"<"  					{return LESS;}
"(" 					{return LE_S_BRACKET;}
")" 					{return RG_S_BRACKET;}
"["  					{return LE_M_BRACKET;}
"]" 					{return RG_M_BRACKET;}
"{" 					{return LE_B_BRACKET;}
"}"  					{return RG_B_BRACKET;}
":"						{return COLON;}
[.]						{return DOT;}
[\t] 					;
[\n]					{return EOL;}
[ ]						;	

%%

int get_token()
{
	return yylex();
}
