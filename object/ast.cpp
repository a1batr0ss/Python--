# include <iostream>
# include <string>
# include <vector>
# include "ast.h"
# include "init.h"
# include "code_object.h"
# include "exec_bytecode.h"
# include "comp_tools.h"

using namespace std;

TreeNode* get_stmt_node(StmtType stmt_kind)
{
	TreeNode *t = new TreeNode;
	t->type = Stmt;
	t->kind.stmt = stmt_kind;
	return t;
}

TreeNode* get_expr_node(ExprType expr_type)
{
	TreeNode *t = new TreeNode;
	t->type = Expr;
	t->kind.expr = expr_type;
	return t;
}

TreeNode* get_root_node()
{
	TreeNode *t = new TreeNode;
	t->type = Root;
	t->value.num_value = 0;  // 存放tree_pointer, 解析根的哪一个节点
	return t;
}

void process_expr(TreeNode*, CodeObject*);
void process_simple_expr(TreeNode*, CodeObject*);
void process_term(TreeNode*, CodeObject*);
void process_factor(TreeNode*, CodeObject*);
void process_stmt(TreeNode *, CodeObject*);

void process_factor(TreeNode *node, CodeObject *co)
{
	// cout << "In process_factor" << endl;
    if (Integer == node->kind.expr)
	{
		int arg = STORE_VALUES(co, node->value.num_value)
		SET_CODE(co, GET_VAR_VALUE)
		SET_CODE(co, arg)
	}
	else if (String == node->kind.expr)
	{
		int arg = STORE_VALUES(co, node->value.string_value)
		SET_CODE(co, GET_VAR_VALUE)
		SET_CODE(co, arg)
	}
	else if (Id == node->kind.expr)
	{
		int arg = STORE_NAMES(co, node->value.string_value)

		SET_CODE(co, GET_VAR_LOCAL)
		SET_CODE(co, arg)
	}
	else
	{
		process_expr(node, co);
	}
}

void process_simple_expr(TreeNode *node, CodeObject *co)
{
	// cout << "In process_simple_expr: " << (Op == node->kind.expr) << endl;
	if (Op == node->kind.expr)  // 必须进行判断, 否则, union的value.op_type与value.num_value会重合, 只给一个赋值, 另一个也有值
	{
		if (Add == node->value.op_type)
		{
			process_simple_expr(node->child[0], co);
			process_simple_expr(node->child[1], co);
			SET_CODE(co, NUMBER_ADD)
		}
		else if (Minus == node->value.op_type)
		{
			process_simple_expr(node->child[0], co);
			process_simple_expr(node->child[1], co);
			SET_CODE(co, NUMBER_MINUS)
		}
		else if (Mod == node->value.op_type)
		{
			process_simple_expr(node->child[0], co);
			process_simple_expr(node->child[1], co);
			SET_CODE(co, NUMBER_MOD)
		}
		else
		{
			// 必须进入term， 否则乘除无法处理
			process_term(node, co);
		}
	}
	else
	{
		// cout << "Not a op" << endl;
		process_term(node, co);
	}
}

void process_term(TreeNode *node, CodeObject *co)
{
	// cout << "In process_term" << endl;
	if (Op == node->kind.expr)
	{
		if (Times == node->value.op_type)
		{
			process_simple_expr(node->child[0], co);
			process_simple_expr(node->child[1], co);
			SET_CODE(co, NUMBER_TIMES)
		}
		else if (Div == node->value.op_type)
		{
			process_simple_expr(node->child[0], co);
			process_simple_expr(node->child[1], co);
			SET_CODE(co, NUMBER_DIV)
		}
	}
	else
	{
		process_factor(node, co);
	}
}

void process_comp_expr(TreeNode *node, CodeObject *co)
{
	process_simple_expr(node->child[0], co);
	process_simple_expr(node->child[1], co);
	
	int arg = 0;
	string op = "";
	if (Less == node->value.op_type)
	{
		op = "<";
	}
	else if (Greater == node->value.op_type)
	{
		op = ">";
	}
	else if (Equal == node->value.op_type)
	{
		op = "==";
	}
	arg = STORE_NAMES(co, (char*)op.data())  // 此处要强转
	SET_CODE(co, GET_VAR_NAME)
	SET_CODE(co, arg)
	SET_CODE(co, COMPARE_OP)	
}

void process_array(TreeNode *node, CodeObject *co)
{
	int l = 0;
	if (NULL != node->child[0])
	{
		l = node->child[0]->child.size();  // 不要写int l	
	}

	for (int i=0; i<l; i++)
	{
		process_factor(node->child[0]->child[i], co);
	}
	if (NULL != node->child[1])
	{	
		process_factor(node->child[1], co);
	}
	else
	{
		l--;
	}
	int arg = l + 1;
	// cout << "arg is " << arg << endl;
	SET_CODE(co, BUILD_ARRAY)
	SET_CODE(co, arg)
}

void process_dict(TreeNode *node, CodeObject *co)
{
	int l = 0;
	if (NULL != node->child[0])
	{
		l = node->child[0]->child.size();
	}
	// cout << "l is " << l << endl;
	for (int i=0; i<l; i++)
	{
		process_factor(node->child[0]->child[i], co);
	}
	if (NULL != node->child[1])
	{
		process_factor(node->child[1]->child[0], co);
		process_factor(node->child[1]->child[1], co);
	}
	else
	{
		l -= 2;
	}
	
	int arg = l/2 + 1;
	// cout << "arg is " << arg << endl;
	SET_CODE(co, BUILD_DICT)
	SET_CODE(co, arg)
}

void process_elem(TreeNode *node, CodeObject *co)
{
	// cout << "In process_elem" << endl;
	process_factor(node->child[0], co);
	process_factor(node->child[1], co);
	
	SET_CODE(co, GET_ELEM)
}

void process_call_stmt(TreeNode*, CodeObject*);

void process_expr(TreeNode *node, CodeObject *co)
{
	if (Comp == node->kind.expr)
	{
		// process_op
		process_comp_expr(node, co);
	}
	else if (Void == node->kind.expr)
	{
		// process_void
	}
	else if (Array == node->kind.expr)
	{
		// process_array
		process_array(node, co);
	}
	else if (Dict == node->kind.expr)
	{
		// process_dict
		process_dict(node, co);
	}
	else if (Elem == node->kind.expr)
	{
		// process_elem
		process_elem(node, co);
	}
	else if (Call == node->kind.stmt)
	{
		// cout << "In call" << endl;
		process_call_stmt(node, co);
	}
	else
	{
		// process_simple
		process_simple_expr(node, co);
	}
}

void process_define(TreeNode *node, CodeObject *co)
{
	int arg_1 = STORE_NAMES(co, node->child[0]->value.string_value)
	CodeObject *co_func = NULL;
	string code_name = node->child[0]->value.string_value;
	int arg_2 = STORE_CODEOBJECT(co, code_name, &co_func)

	SET_CODE(co, GET_VAR_NAME)
	SET_CODE(co, arg_1)
	SET_CODE(co, GET_VAR_NAME)
	SET_CODE(co, arg_2)
	SET_CODE(co, BUILD_FUNCTION)
	// FunctionObject *func_obj = new FunctionObject(code->child[0]->value.string_value, co, NULL);
	int l = 0;
	if (NULL != node->child[1])
	{
		l = node->child[1]->child.size();  // 不要写int l	
	}
	for (int i=0; i<l; i++)
	{
		STORE_NAMES(co_func, node->child[1]->child[i]->value.string_value)
	}
	if (NULL != node->child[2])
	{	
		STORE_NAMES(co_func, node->child[2]->value.string_value)
	}
	else
	{
		l--;
	}

	SET_CODE(co, l+1)  // argument of BUILD_FUNCTION
	SET_CODE(co, STORE_LOCAL)

	for (int i=0; i<node->child[3]->child.size(); i++)
	{
		process_stmt(node->child[3]->child[i], co_func);
	}
}

void process_assign_stmt(TreeNode *node, CodeObject *co)
{
	if (Define == node->child[0]->kind.expr)
	{
		process_define(node->child[0], co);
		return;
	}

	int arg = STORE_NAMES(co, node->child[0]->value.string_value)
	SET_CODE(co, GET_VAR_NAME)
	SET_CODE(co, arg)
	int node_child = node->child[1]->child.size();
	process_expr(node->child[1], co);  // 不要传入node
	SET_CODE(co, STORE_LOCAL)
}

void process_if_stmt(TreeNode *node, CodeObject *co)
{
	// cout << "start ast" << endl;
	int node_child = node->child.size();

	process_expr(node->child[0], co);
	int arg_1 = NO_CODE(co)
	SET_CODE(co, JUMP_IF_FALSE)
	SET_CODE(co, 0)
	
	SET_CODE(co, POP_TOP)
	for (int i=0; i<node->child[1]->child.size(); i++)
	{
		process_stmt(node->child[1]->child[i], co);
	}
	// 所有语句执行完成后，跳出if
	SET_CODE(co, JUMP_CODE_ABSOLUTE)
	SET_CODE(co, 0)

	int arg_2 = NO_CODE(co)
	SET_CODE_POS(co, arg_1+1, arg_2)
	
	SET_CODE(co, POP_TOP)
	if (3 == node_child)
	{
		for (int i=0; i<node->child[2]->child.size(); i++)
		{
			process_stmt(node->child[2]->child[i], co);
		}
	}
	int arg_3 = NO_CODE(co)
	SET_CODE_POS(co, arg_2-1, arg_3)  // 设置上面JUMP_CODE_ABSOLUTE的参数
	// cout << "end ast" << endl;
}

void process_for_stmt(TreeNode *node, CodeObject *co)
{
	// process_factor(node->child[0], co);
	
	int arg_3 = STORE_NAMES(co, node->child[0]->value.string_value)
	process_factor(node->child[1], co);
	int arg_1 = NO_CODE(co)
	SET_CODE(co, GET_ITER)
	SET_CODE(co, FOR_ITER)
	SET_CODE(co, 0)
	SET_CODE(co, GET_VAR_NAME)
	SET_CODE(co, arg_3)
	SET_CODE(co, SWAP_TOP_TWO)
	SET_CODE(co, STORE_LOCAL)

	// process_stmt(node->child[2], co);
		
	for (int i=0; i<node->child[2]->child.size(); i++)
	{
		process_stmt(node->child[2]->child[i], co);
	}


	SET_CODE(co, JUMP_CODE_ABSOLUTE)
	SET_CODE(co, arg_1+1)
	SET_CODE(co, POP_TOP)

	int arg_2 = NO_CODE(co)
	SET_CODE_POS(co, arg_1+2, arg_2-1)
	set_arg_break(co, arg_2-1);
}

void process_call_method(TreeNode *node, CodeObject *co)
{
	/*
	 * 参数先入栈, 然后变量, 最后函数名
	 */
	// cout << "In call method." << endl;
	// process_factor(node->child[0], co);  // 变量名
	// process_factor(node->child[1], co);  // 函数名
	int l = 0;
	if (NULL != node->child[2])
	{
		l = node->child[2]->child.size();  // 不要写int l	
	}
	// cout << "l is " << l << endl;
	for (int i=0; i<l; i++)
	{
		process_factor(node->child[2]->child[i], co);
	}
	if (NULL != node->child[3])
	{	
		process_factor(node->child[3], co);
	}
	else
	{
		l--;
	}
	int arg_1 = l + 2;  // 还有对象自身
	// process_factor(node->child[0], co);
	int arg_3 = STORE_NAMES(co, node->child[0]->value.string_value)
	// cout << "var_name is" << node->child[0]->value.string_value << endl;
	// 直接处理元素节点, 防止进入factor处理, 产生指定的字节码(不想产生那个字节码, 只适合大多数情况)
	// cout << "function name is " << node->child[1]->value.string_value << endl; 
	int arg_2 = STORE_NAMES(co, node->child[1]->value.string_value)
	// cout << "arg_1 is " << arg_1 << endl;
	SET_CODE(co, GET_VAR_LOCAL)
	SET_CODE(co, arg_3)
	SET_CODE(co, GET_VAR_NAME)
	SET_CODE(co, arg_2)
	SET_CODE(co, GET_ATTR)
	SET_CODE(co, CALL_FUNCTION)
	SET_CODE(co, arg_1)
}

void process_call_funtion(TreeNode *node, CodeObject *co)
{
	// cout << "In call function." << endl;
	// cout << "call_function's child length is " << node->child.size() << endl;
	// process_factor(node->child[0], co);  // 函数名, 本句放在参数入栈之后, 到栈顶, 便于操作
	int l = 0;
	if (NULL != node->child[1])
	{
		l = node->child[1]->child.size();  // 不要写int l	
	}
	// cout << "l is " << l << endl;
	for (int i=0; i<l; i++)
	{
		process_factor(node->child[1]->child[i], co);
	}
	if (NULL != node->child[2])
	{	
		process_factor(node->child[2], co);
	}
	else
	{
		l--;
	}
	
	process_factor(node->child[0], co);  // 函数名

	int arg = l + 1;
	// cout << "arg is " << arg << endl;
	SET_CODE(co, CALL_FUNCTION)
	SET_CODE(co, arg)
}

void process_call_stmt(TreeNode *node, CodeObject *co)
{
	// cout << "In call" << endl;
	if (Method == node->child[0]->kind.expr)
	{
		process_call_method(node->child[0], co);
	}
	else
	{
		process_call_funtion(node->child[0], co);
	}
}

void process_decself_stmt(TreeNode *node, CodeObject *co)
{
	int arg = STORE_NAMES(co, node->value.string_value)
	SET_CODE(co, GET_VAR_NAME)
	SET_CODE(co, arg)
	process_factor(node, co);
	SET_CODE(co, DEC_SELF)
	SET_CODE(co, STORE_LOCAL)
}

void process_incself_stmt(TreeNode *node, CodeObject *co)
{
	int arg = STORE_NAMES(co, node->value.string_value)
	SET_CODE(co, GET_VAR_NAME)
	SET_CODE(co, arg)
	process_factor(node, co);
	SET_CODE(co, INC_SELF)
	SET_CODE(co, STORE_LOCAL)
}

void process_others_stmt(TreeNode *node, CodeObject *co)
{
	if (Incself == node->child[0]->kind.expr)
	{
		process_incself_stmt(node->child[0]->child[0], co);
	}
	else if (Decself == node->child[0]->kind.expr)
	{
		process_decself_stmt(node->child[0]->child[0], co);
	}
}

void process_break_stmt(TreeNode *node, CodeObject *co)
{
	SET_CODE(co, BREAK_LOOP)
	SET_CODE(co, 0)
}

void process_return_stmt(TreeNode *node, CodeObject *co)
{
	process_factor(node->child[0], co);	
	SET_CODE(co, RETURN_VALUE)
}

void process_continue_stmt(TreeNode *node, CodeObject *co)
{
	SET_CODE(co, CONTINUE_LOOP)
	int arg = FIND_LAST_CODE(co, FOR_ITER)
	SET_CODE(co, arg)	
}

void process_stmt(TreeNode *node, CodeObject *co)
{
	if (Stmt == node->type)
	{
		if (Assign == node->kind.stmt)
		{
			// cout << "This is a assignment." << endl;
			process_assign_stmt(node, co);
		}
		else if (If == node->kind.stmt)
		{
			// cout << "This is a if assignment." << endl;
			process_if_stmt(node, co);
		}
		else if (For == node->kind.stmt)
		{
			// cout << "THis is a for assignment." << endl;
			process_for_stmt(node, co);
		}
		else if (Call == node->kind.stmt)
		{
			process_call_stmt(node, co);
		}
		else if (Break == node->kind.stmt)
		{
			process_break_stmt(node, co);
		}
		else if (Continue == node->kind.stmt)
		{
			process_continue_stmt(node, co);
		}
		else if (Return == node->kind.stmt)
		{
			process_return_stmt(node, co);
		}
		else if (Others == node->kind.stmt)
		{
			process_others_stmt(node, co);
		}
		else
		{
			// Other statements.
			cout << "syntax error!" << endl;
		}
	}
	else
	{
		// Not a statement.
		cout << "syntax error" << endl;
	}
}


CodeObject* compile_ast(TreeNode *tree)
{
	// if tree_pointer == 0, 才new, 并把co传入各级解析, 把生成的bytecode存入
	// execute_bytecode(f_cur);int tree_pointer = tree->value.num_value;
	// cout << "In compile_ast: ";
	// cout << co->get_name() << endl;	
	int tree_pointer = tree->value.num_value - 1;
	int num_nodes = tree->child.size();
	/*
	 * if (1 == num_nodes)
	 * {
	 *     init_compile_environment();  // 为co_main初始化, 仅初始化一次
	 * }
	 */
	CodeObject *co = (CodeObject*)(get_codeobject_main());

	// cout << "nodes number are " << num_nodes << endl;
	if (tree_pointer >= num_nodes)
	{
		return NULL;
	}

	// ArrayObject *names = GET_NAMES(co);

	TreeNode *cur_stmt = tree->child[tree_pointer];  // stmt
	// cout << cur_stmt->child[0]->value.string_value << endl;
	process_stmt(cur_stmt, co);

	return co;
}

CodeObject* compile_ast_file(TreeNode *tree)
{
	CodeObject *co = (CodeObject*)(get_codeobject_main());
	// cout << "In compile_ast_file: co is " << co << endl;

	int l = tree->child.size();
	for (int i=0; i<l; i++)
	{
		process_stmt(tree->child[i], co);
	}
	return co;
}

