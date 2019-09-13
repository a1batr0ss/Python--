%{

# include <stdio.h>
# include "ast.h"
# include "parse.h"

# define YYSTYPE TreeNode*

extern char *token_value;

extern "C"
{
	void yyerror(const char*);
	extern int yylex();
	int yyparse();
}

TreeNode *tree = get_root_node();
int tree_pointer = 0;
int is_exec_immediately = 1;
int is_interaction = 1;
int parse_status = 0;

void yyerror(const char *str)
{
	parse_status = 1;
	printf("Error occured: %s\n", str);
}

int yywrap()
{
	return 1;
}

%}

%token IF FOR IN DEF ELSE BREAK CONTINUE RETURN
%token NUMBERS ID CHARS STRING
%token DOT LE_S_BRACKET RG_S_BRACKET LE_M_BRACKET RG_M_BRACKET LE_B_BRACKET RG_B_BRACKET COMMA COLON QUOTE_MARK
%token ADD MINUS TIMES DIV MOD ASSIGN GREATER LESS EQ
%token EOL

%%

sentence: | sentence stmt

stmt:empty
	| stmt_not_empty
	{
		tree->child.push_back($1);
		tree->value.num_value++;  // 注意把main分离出语法分析器后, 这里的值加1会对compile_ast造成影响, 在comiple_ast中应该减一
		if (is_interaction && is_exec_immediately)
		{
			return 1;  // 立即执行字节码
		}
	}


stmt_not_empty: assign EOL
	{
		$$ = $1;
	} 
	| if EOL
	{
		// if的换行不能省，否则不能判断第二个if的右括号
		/* printf("It is a if statement.\n"); */
		$$ = $1;
	}
	| for EOL  {$$ = $1;}
	| call EOL  {$$ = $1;}
	| loop_op EOL {$$ = $1;}
	| others EOL  {$$ = $1;}

others: dec_self  
	  	{
			$$ = get_stmt_node(Others);
			$$->child.push_back($1);
		}
	  | inc_self  
		{
			$$ = get_stmt_node(Others);
			$$->child.push_back($1);
		}

dec_self: id ADD ADD
		{
			$$ = get_expr_node(Incself);
			$$->child.push_back($1);
		}

inc_self: id MINUS MINUS
		{
			$$ = get_expr_node(Decself);
			$$->child.push_back($1);
		}

loop_op: BREAK
	   {
			$$ = get_stmt_node(Break);
		}
	   | CONTINUE
		{
			$$ = get_stmt_node(Continue);
		}
		| RETURN factor
		{
			$$ = get_stmt_node(Return);
			$$->child.push_back($2);
		}

body_stmt: 
		{ 
			// 第一句时，空 stmt, 会进入这里执行
			$$ = get_root_node();
		}
		| body_stmt stmt_not_empty
		 {
			/* printf("In tab body_stmt\n"); */
			TreeNode *tmp = $1;
			tmp->child.push_back($2);
		 }

if: IF LE_S_BRACKET expr RG_S_BRACKET LE_B_BRACKET EOL body_stmt RG_B_BRACKET
  	{
		/* printf("In if\n"); */
		$$ = get_stmt_node(If);
		$$->child.push_back($3);
		$$->child.push_back($7);
	}
  | IF LE_S_BRACKET expr RG_S_BRACKET LE_B_BRACKET EOL body_stmt RG_B_BRACKET ELSE LE_B_BRACKET EOL body_stmt RG_B_BRACKET
	{
		$$ = get_stmt_node(If);
		$$->child.push_back($3);
		$$->child.push_back($7);
		$$->child.push_back($12);
	}

for: FOR LE_S_BRACKET id IN expr RG_S_BRACKET LE_B_BRACKET EOL body_stmt RG_B_BRACKET
	{
		$$ = get_stmt_node(For);
		$$->child.push_back($3);
		$$->child.push_back($5);
		$$->child.push_back($9);
	}

call: method
	{
		$$ = get_stmt_node(Call);
		$$->child.push_back($1);
	}
	| function 
	{
		$$ = get_stmt_node(Call);
		$$->child.push_back($1);
	}

method: id DOT id LE_S_BRACKET not_end_array end_array  RG_S_BRACKET
	  {
			$$ = get_expr_node(Method);
			$$->child.push_back($1);
			$$->child.push_back($3);
			$$->child.push_back($5);
			$$->child.push_back($6);
	  }

function: id LE_S_BRACKET not_end_array end_array RG_S_BRACKET
		{
			$$ = get_expr_node(Function);
			$$->child.push_back($1);
			$$->child.push_back($3);
			$$->child.push_back($4);
		}

empty: EOL
	
assign: id ASSIGN expr
	  {
			$$ = get_stmt_node(Assign);
			$$->child.push_back($1);
			$$->child.push_back($3);
		}
		| define
		{
			$$ = get_stmt_node(Assign);
			$$->child.push_back($1);
		}

define: DEF id LE_S_BRACKET not_end_array end_array RG_S_BRACKET LE_B_BRACKET EOL body_stmt RG_B_BRACKET 
	  {
			$$ = get_expr_node(Define);
			$$->child.push_back($2);
			$$->child.push_back($4);
			$$->child.push_back($5);
			$$->child.push_back($9);
	  }

id: ID
  {
		$$ = get_expr_node(Id);
		$$->value.string_value = token_value;
	}

expr: simple_expr
	{
		$$ = $1;
	}
	| simple_expr EQ simple_expr
	{
		$$ = get_expr_node(Comp);
		$$->child.push_back($1);
		$$->child.push_back($3);
		$$->value.op_type = Equal;
	}
	| simple_expr LESS simple_expr
	{
		$$ = get_expr_node(Comp);
		$$->child.push_back($1);
		$$->child.push_back($3);
		$$->value.op_type = Less;
	}
	| simple_expr GREATER simple_expr
	{
		$$ = get_expr_node(Comp);
		$$->child.push_back($1);
		$$->child.push_back($3);
		$$->value.op_type = Greater;
	}
	| id LE_M_BRACKET factor RG_M_BRACKET
	{
		$$ = get_expr_node(Elem);
		$$->child.push_back($1);
		$$->child.push_back($3);
	}
	| call  {$$ = $1;}

simple_expr: simple_expr ADD term
		   {
				$$ = get_expr_node(Op);
				$$->child.push_back($1);
				$$->child.push_back($3);
				$$->value.op_type = Add;
			}
		| simple_expr MINUS term
		{
			$$ = get_expr_node(Op);
			$$->child.push_back($1);
			$$->child.push_back($3);
			$$->value.op_type = Minus;
		}
		| simple_expr MOD simple_expr
		{
			$$ = get_expr_node(Op);
			$$->child.push_back($1);
			$$->child.push_back($3);
			$$->value.op_type = Mod;
		}
		| term  {$$ = $1;}

term: term TIMES factor
	{
		/* printf("Times in yacc.\n"); */
		$$ = get_expr_node(Op);
		$$->child.push_back($1);
		$$->child.push_back($3);
		$$->value.op_type = Times;
	}
	| term DIV factor
		{
			$$ = get_expr_node(Op);
			$$->child.push_back($1);
			$$->child.push_back($3);
			$$->value.op_type = Div;
		}
	| LE_M_BRACKET not_end_array end_array RG_M_BRACKET
	{
		$$ = get_expr_node(Array);
		$$->child.push_back($2);
		$$->child.push_back($3);
	}
	| LE_B_BRACKET not_end_dict end_dict  RG_B_BRACKET
	{
		$$ = get_expr_node(Dict);
		$$->child.push_back($2);
		$$->child.push_back($3);
	}
	| factor {$$ = $1;}
		   
factor: LE_S_BRACKET expr RG_S_BRACKET 
	  {
			$$ = $2;
		}
	| NUMBERS
	 {
			$$ = get_expr_node(Integer);
			$$->value.num_value = atoi(token_value);
	}
	 | QUOTE_MARK ID QUOTE_MARK
		{
			$$ = get_expr_node(String);
			$$->value.string_value = token_value;
		}
	| QUOTE_MARK STRING QUOTE_MARK
	{
		$$ = get_expr_node(String);
		$$->value.string_value = token_value;
	}
	| ID
	{
			$$ = get_expr_node(Id);
			$$->value.string_value = token_value;
	}
not_end_array:
			{
				$$ = get_expr_node(Void);
			} 
			 | not_end_array factor COMMA
			{
				TreeNode *tmp = $1;
				tmp->child.push_back($2);
			}

end_array: factor  {$$ = $1;}
			|
			{
				$$ = NULL;
			}

not_end_dict: 
			{
				$$ = get_expr_node(Void);
			}
			| not_end_dict factor COLON factor COMMA
			{
				TreeNode *tmp = $1;
				tmp->child.push_back($2);
				tmp->child.push_back($4);
			}

end_dict: factor COLON factor
		{
			$$ = get_expr_node(Void);
			$$->child.push_back($1);
			$$->child.push_back($3);	
		}
		|
		{
			$$ = NULL;
		}

%%

TreeNode* generate_ast()
{
	parse_status = 0;
	int status = yyparse();
	tree->kind.expr = (ExprType)parse_status;  // 使用tree的kind域作语法分析状态标志
	return tree;
}


