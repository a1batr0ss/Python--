# ifndef AST_H
# define AST_H

# include <vector>
# include <string>
# include "code_object.h"

using namespace std;

enum NodeType
{ Stmt, Expr, Root};  // 对应下面两种枚举(无root)

enum StmtType 
{ If, For, Assign, Call, Break, Continue, Return, Others};

enum ExprType
{ Void=8, Integer, String, Op, Array, Dict, Method, Function, Define, Id, Elem, Comp, Decself, Incself};

enum OpType
{ Add, Minus, Times, Div, Mod, Equal, Greater, Less};

class TreeNode
{
	public:
		vector<TreeNode*> child;
		NodeType type;
		union 
		{
			StmtType stmt;
			ExprType expr;
		} kind;
		union
		{
			OpType op_type;
			// string string_value;
			char *string_value;  // 这里不能使用string
			int num_value;
		} value;
};

TreeNode* get_stmt_node(StmtType);
TreeNode* get_expr_node(ExprType);
TreeNode* get_root_node();
CodeObject* compile_ast(TreeNode*);
CodeObject* compile_ast_file(TreeNode*);

# endif

