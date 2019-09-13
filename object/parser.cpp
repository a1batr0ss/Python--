# include <iostream>
# include "parser.h"
# include "parse.h"
# include "tag.h"

using namespace std;
using namespace tag;

TreeNode* Parser::start()
{
	TreeNode *root = get_root_node();
	lexer->start();
	sentence(root);
	// cout << "In start: " << root->child.size() << endl; 
	return root;
}

TreeNode* Parser::sentence_prime(TreeNode *node)
{
	while (lexer->is(EOL) || lexer->is(EMPTY) || lexer->is(TAB))
	{
		cur_token = lexer->get_next_token();
	}

	if (lexer->is(ID) || lexer->is(IF) || lexer->is(FOR) || lexer->is(DEF) || lexer->is(BREAK) || lexer->is(CONTINUE) || lexer->is(RETURN))
	{
	// cout << "token is " << cur_token.get_token_value() << endl;
		TreeNode *cur = stmt();
		node->child.push_back(cur);
		sentence_prime(node);
	}
	else
	{
		return NULL;
	}
}

void Parser::sentence(TreeNode *node)
{
	cur_token = lexer->get_next_token();
	sentence_prime(node);
}

TreeNode* Parser::stmt()
{
	while (lexer->is(EOL) || lexer->is(EMPTY) || lexer->is(TAB))
	{
		cur_token = lexer->get_next_token();
	}
	if (lexer->is(ID) || lexer->is(IF) || lexer->is(FOR) || lexer->is(DEF) || lexer->is(BREAK) || lexer->is(CONTINUE) || lexer->is(RETURN))
	{
		return stmt_not_empty();
	}
	else
	{
		return NULL;
	}
}

TreeNode* Parser::empty()
{
	return NULL;
}

TreeNode* Parser::stmt_not_empty()
{
	// cout << "In stmt_not_empty: " << cur_token.get_token_tag() << endl;
	TreeNode *cur = NULL;
	if (lexer->is(ID) || lexer->is(DEF))
	{
		// assign();  // 不一定是assign
		cur = id_start();	
	}
	else if (lexer->is(IF))
	{
		cur = if_();
	}
	else if (lexer->is(FOR))
	{
		cur = for_();
		// cout << "end for: " << cur_token.get_token_tag() << endl;
	}
	else if (lexer->is(BREAK) || lexer->is(CONTINUE) || lexer->is(RETURN))
	{
		cur = loop_op();
	}
	/*
	 * else if (lexer->is(DEF))
	 * {
	 *     // define
	 *     cur = define();
	 * }
	 */
	return cur;
	// cout << "In stmt_not_empty" << endl;
}

TreeNode* Parser::define()
{
	TreeNode *cur_ret = get_stmt_node(Assign);
	TreeNode *cur = get_expr_node(Define);
	if (lexer->is(DEF))
		cur_token = lexer->get_next_token();
	TreeNode *child1 = id();
	if (lexer->is(LE_S_BRACKET))
		cur_token = lexer->get_next_token();
	TreeNode *child2 = not_end_array();
	TreeNode *child3 = end_array();
	if (lexer->is(RG_S_BRACKET))
		cur_token = lexer->get_next_token();
	if (lexer->is(LE_B_BRACKET))
		cur_token = lexer->get_next_token();
	TreeNode *child4 = body_stmt();
	if (lexer->is(RG_B_BRACKET))
	{
		// cout << "A define statement." << endl;
		cur->child.push_back(child1);
		cur->child.push_back(child2);
		cur->child.push_back(child3);
		cur->child.push_back(child4);
		cur_ret->child.push_back(cur);
		cur_token = lexer->get_next_token();
	}
	return cur_ret;
}

TreeNode* Parser::id_start()
{
	// 也包括函数定义语句 define(也认为是一种赋值语句)
	if (lexer->is(DEF))
	{
		TreeNode *cur = define();
		return cur;
	}
	TreeNode *child1 = id();
	// cout << "In assign: " << cur_token.get_token_value() << endl;
	if (lexer->is(ASSIGN))
	{
		// assign
		cur_token = lexer->get_next_token();
		TreeNode *child2 = expr();
		// cout << "type is " << child2->kind.expr << endl;
		TreeNode *cur = get_stmt_node(Assign);
		cur->child.push_back(child1);
		cur->child.push_back(child2);
		return cur;
	}
	else if (lexer->is(DOT))
	{
		// method
		cur_token = lexer->get_next_token();
		TreeNode *child2;
		if (lexer->is(ID))
			child2 = id();
		if (lexer->is(LE_S_BRACKET))
		{
			TreeNode *cur_ret = get_stmt_node(Call);
			TreeNode *cur = get_expr_node(Method);
			cur_token = lexer->get_next_token();
			TreeNode *child3 = not_end_array();
			TreeNode *child4 = end_array();
			if (lexer->is(RG_S_BRACKET))
			{
				// cout << "A method called." << endl;
				cur_token = lexer->get_next_token();
				cur->child.push_back(child1);
				cur->child.push_back(child2);
				cur->child.push_back(child3);
				cur->child.push_back(child4);
				cur_ret->child.push_back(cur);
				return cur_ret;
			}
			else
			{
				// TODO: syntax error
			}
		}
	}
	else if (lexer->is(LE_S_BRACKET))
	{
		// function
		TreeNode *cur_ret = get_stmt_node(Call);
		TreeNode *cur = get_expr_node(Function);
		cur_token = lexer->get_next_token();
		TreeNode *child2 = not_end_array();
		TreeNode *child3 = end_array();
		if (lexer->is(RG_S_BRACKET))
		{
			// cout << "A function called." << endl;
			cur_token = lexer->get_next_token();
			cur->child.push_back(child1);
			cur->child.push_back(child2);
			cur->child.push_back(child3);
			cur_ret->child.push_back(cur);
			return cur_ret; 
		}
		else
		{
			// TODO: syntax error
		}
	}
	return NULL;
}

TreeNode* Parser::loop_op()
{
	// cout << "A return statement." << endl;
	TreeNode *cur = NULL;
	if (lexer->is(BREAK))
	{
		cur = get_stmt_node(Break);
		cur_token = lexer->get_next_token();
	}
	else if (lexer->is(CONTINUE))
	{
		cur = get_stmt_node(Continue);
		cur_token = lexer->get_next_token();
	}
	else if (lexer->is(RETURN))
	{
		cur = get_stmt_node(Return);
		cur_token = lexer->get_next_token();
		TreeNode *child = term();
		cur->child.push_back(child);
		// cout << "A return statement2." << endl;
	}

	return cur;
}

TreeNode* Parser::assign()
{
	TreeNode *child1 = id();
	if (lexer->is(ASSIGN))
	{
		cur_token =	lexer->get_next_token();  // NUMBER
	}
	else
	{
		// TODO: syntax error
	}
	TreeNode *child2 = expr();
	TreeNode *cur = get_stmt_node(Assign);
	cur->child.push_back(child1);
	cur->child.push_back(child2);
	return cur;
}

TreeNode* Parser::id()
{
	TreeNode *cur = get_expr_node(Id);
	if (lexer->is(ID))
	{
		// cout << "id is " << cur_token.get_token_value() << endl;
		char *id = new char[cur_token.get_token_value().length()];
		strcpy(id, cur_token.get_token_value().data());
		cur->value.string_value = id;
		cur_token = lexer->get_next_token();
	}
	else
	{
		cout << "Not a identifier." << endl; 
	}
	// cur->value.string_value = (char*)(cur_token.get_token_value().data());
	// cur->value.string_value = const_cast<char*>(cur_token.get_token_value().data());
	/*
	 * char *id = const_cast<char*>(cur_token.get_token_value().data());
	 * cur->value.string_value = id;
	 */
	return cur;
}

TreeNode* Parser::body_stmt_prime(TreeNode *node)
{
	while (lexer->is(EOL) || lexer->is(EMPTY) || lexer->is(TAB))
	{
		cur_token = lexer->get_next_token();
	}

	if (lexer->is(ID) || lexer->is(IF) || lexer->is(FOR) || lexer->is(DEF) || lexer->is(BREAK) || lexer->is(CONTINUE) || lexer->is(RETURN))
	{
		TreeNode *child = stmt_not_empty();
		node->child.push_back(child);
		body_stmt_prime(node);
	}
	else
	{
		return NULL;
	}
	return NULL;
}

TreeNode* Parser::body_stmt()
{
	TreeNode *cur = get_root_node();
	body_stmt_prime(cur);
	return cur;
}

TreeNode* Parser::if_()
{
	TreeNode *cur = get_stmt_node(If);
	cur_token = lexer->get_next_token();
	if (lexer->is(IF))
		cur_token = lexer->get_next_token();
	if (lexer->is(LE_S_BRACKET))
		cur_token = lexer->get_next_token();
	TreeNode *child1 = expr();
	if (lexer->is(RG_S_BRACKET))
		cur_token = lexer->get_next_token();
	TreeNode *child2 = NULL;
	if (lexer->is(LE_B_BRACKET))
	{
		cur_token = lexer->look_ahead();
		child2 = body_stmt();
	}
	if (lexer->is(RG_B_BRACKET))
	{
		cur_token = lexer->get_next_token();
	}
	cur->child.push_back(child1);
	cur->child.push_back(child2);
	if (lexer->is(ELSE))
	{
		cur_token = lexer->get_next_token();
		if (lexer->is(LE_B_BRACKET))
			cur_token = lexer->look_ahead();
		TreeNode *child3 = body_stmt();
		if (lexer->is(RG_B_BRACKET))
		{
			cur_token = lexer->get_next_token();
		}
		cur->child.push_back(child3);
	}
	return cur;
}

TreeNode* Parser::for_()
{
	TreeNode *cur = get_stmt_node(For);
	cur_token = lexer->get_next_token();
	if (lexer->is(FOR))
		cur_token = lexer->get_next_token();
	if (lexer->is(LE_S_BRACKET))
		cur_token = lexer->get_next_token();
	TreeNode *child1 = id();
	if (lexer->is(IN))
		cur_token = lexer->get_next_token();
	TreeNode *child2 = expr();
	if(lexer->is(RG_S_BRACKET))
		cur_token = lexer->get_next_token();
	TreeNode *child3 = NULL;
	if (lexer->is(LE_B_BRACKET))
	{
		cur_token = lexer->look_ahead();
		child3 = body_stmt();
	}
	if (lexer->is(RG_B_BRACKET))
	{
		// cout << "a for sentence. " << endl;
		cur_token = lexer->get_next_token();
	}
	cur->child.push_back(child1);
	cur->child.push_back(child2);
	cur->child.push_back(child3);
	return cur;
}

TreeNode* Parser::expr()
{
	TreeNode *child1 = NULL;
	child1 = simple_expr();
	if (lexer->is(LESS))
	{
		cur_token = lexer->get_next_token();
		TreeNode *child2 = simple_expr();
		TreeNode *cur = get_expr_node(Comp);
		cur->child.push_back(child1);
		cur->child.push_back(child2);
		cur->value.op_type = Less;
		return cur;
	}
	else if (lexer->is(GREATER))
	{
		cur_token = lexer->get_next_token();
		TreeNode *child2 = simple_expr();
		TreeNode *cur = get_expr_node(Comp);
		cur->child.push_back(child1);
		cur->child.push_back(child2);
		cur->value.op_type = Greater;
		return cur;
	}
	else if (lexer->is(EQ))
	{
		cur_token = lexer->get_next_token();
		TreeNode *child2 = simple_expr();
		TreeNode *cur = get_expr_node(Comp);
		cur->child.push_back(child1);
		cur->child.push_back(child2);
		cur->value.op_type = Equal;
		return cur;
	}
	else
	{
		return child1;
	}
	// cout << "expr size is " << cur->child.size() << endl;
	// cout << "expr type is " << cur->kind.expr << endl;
}

TreeNode* Parser::simple_expr_prime(TreeNode *node)
{
	// cout << "In simple_expr_prime" << endl;
	TreeNode *cur = get_expr_node(Op);
	if (lexer->is(ADD))
	{
		cur_token = lexer->get_next_token();
		TreeNode *child2 = term();
		node = simple_expr_prime(node);
		cur->child.push_back(node);
		cur->child.push_back(child2);
		cur->value.op_type = Add;
		// cout << node->value.num_value << endl;
		// cout << child2->value.num_value << endl;
		return cur;
	}
	else if (lexer->is(MINUS))
	{
		cur_token = lexer->get_next_token();
		TreeNode *child2 = term();
		node = simple_expr_prime(node);
		cur->child.push_back(node);
		cur->child.push_back(child2);
		cur->value.op_type = Minus;
		return cur;
	}
	else if (lexer->is(MOD))
	{
		cur_token = lexer->get_next_token();
		TreeNode *child2 = simple_expr();
		node = simple_expr_prime(node);
		cur->child.push_back(node);
		cur->child.push_back(child2);
		cur->value.op_type = Mod;
		return cur;
	}
	else
	{
		return node;
	}
}

TreeNode* Parser::simple_expr()
{
	TreeNode *cur = NULL;
	cur = term();
	cur = simple_expr_prime(cur);
	// cout << "size is " << cur->child.size() << endl;
	// cout << "type is " << cur->kind.expr << endl;
	return cur;
}

TreeNode* Parser::not_end_array_prime(TreeNode *node)
{
	if (lexer->is(ID) || lexer->is(NUMBERS) || lexer->is(QUOTE_MARK) || lexer->is(LE_S_BRACKET))
	{
		TreeNode *cur = term();
		node->child.push_back(cur);

		if (lexer->is(COMMA))
		{
			cur_token = lexer->get_next_token();
		}
		not_end_array_prime(node);
	}
	else
	{
		return NULL;
	}
	return NULL;
}

TreeNode* Parser::not_end_array()
{
	TreeNode *cur = get_expr_node(Void);
	not_end_array_prime(cur);
	return cur;
}

TreeNode* Parser::end_array()
{
	TreeNode *cur = NULL;
	// end_array: term | 
	if (lexer->is(ID) || lexer->is(NUMBERS) || lexer->is(QUOTE_MARK) || lexer->is(LE_S_BRACKET))
	{
		cur = term();
	}
	else
	{
		return NULL;
	}
	return cur;
}

TreeNode* Parser::term_prime(TreeNode *node)
{
	TreeNode *cur = get_expr_node(Op);
	if (lexer->is(TIMES))
	{
		cur_token = lexer->get_next_token();
		TreeNode *child2 = factor();
		node = term_prime(node);
		cur->child.push_back(node);
		cur->child.push_back(child2);
		cur->value.op_type = Times;
		return cur;
	}
	else if (lexer->is(DIV))
	{
		cur_token = lexer->get_next_token();
		TreeNode *child2 = factor();
		node = term_prime(node);
		cur->child.push_back(node);
		cur->child.push_back(child2);
		cur->value.op_type = Div;
		return cur;
	}
	else
	{
		return node;
	}
	// cout << "end term_prime" << endl;
}

TreeNode* Parser::term()
{
	TreeNode *cur = NULL;
	if (lexer->is(LE_M_BRACKET))
	{
		cur = get_expr_node(Array);
		cur_token = lexer->get_next_token();
		TreeNode *child1 = not_end_array();
		TreeNode *child2 = end_array();
		cur->child.push_back(child1);
		cur->child.push_back(child2);
		if (lexer->is(RG_M_BRACKET))
		{
			// cout << "An array." << endl;
			cur_token = lexer->get_next_token();
			return cur;
		}
		else
		{
			cout << "Syntax error: may miss a ]." << endl;
		}
	}
	else if (lexer->is(LE_B_BRACKET))
	{
		cur_token = lexer->get_next_token();
		// TODO: discard a dictionay.
	}
	else if (lexer->is(ID))
	{
		// cout << "A ID. ";
		TreeNode *child1 = get_expr_node(Id);
		// cur->value.string_value = const_cast<char*>(cur_token.get_token_value().data());
		char *id_ = new char[cur_token.get_token_value().length()];
		strcpy(id_, cur_token.get_token_value().data());
		child1->value.string_value = id_;

		cur_token = lexer->get_next_token();

		TreeNode *cur_ret = NULL;
		if (lexer->is(DOT))
		{
			// method
			cur_token = lexer->get_next_token();
			TreeNode *child2;
			if (lexer->is(ID))
				child2 = id();
			if (lexer->is(LE_S_BRACKET))
			{
				TreeNode *cur_ret = get_stmt_node(Call);
				TreeNode *cur = get_expr_node(Method);
				cur_token = lexer->get_next_token();
				TreeNode *child3 = not_end_array();
				TreeNode *child4 = end_array();
				if (lexer->is(RG_S_BRACKET))
				{
					// cout << "A method called." << endl;
					cur_token = lexer->get_next_token();
					cur->child.push_back(child1);
					cur->child.push_back(child2);
					cur->child.push_back(child3);
					cur->child.push_back(child4);
					cur_ret->child.push_back(cur);
					return cur_ret;
				}
				else
				{
					// TODO: syntax error
				}
			}
		}
		else if (lexer->is(LE_S_BRACKET))
		{
			// function
			TreeNode *cur_ret = get_stmt_node(Call);
			TreeNode *cur = get_expr_node(Function);
			cur_token = lexer->get_next_token();
			TreeNode *child2 = not_end_array();
			TreeNode *child3 = end_array();
			if (lexer->is(RG_S_BRACKET))
			{
				// cout << "A function called." << endl;
				cur_token = lexer->get_next_token();
				cur->child.push_back(child1);
				cur->child.push_back(child2);
				cur->child.push_back(child3);
				cur_ret->child.push_back(cur);
				return cur_ret; 
			}
			else
			{
				// TODO: syntax error
			}
		}
		else
		{
			return child1;
		}
	}
	else if (lexer->is(NUMBERS) || lexer->is(QUOTE_MARK) || lexer->is(LE_S_BRACKET))
	{
		// 终结符
		cur = factor();
		// return cur;
	}
	cur = term_prime(cur);

	return cur;
}

TreeNode* Parser::factor()
{
	if (lexer->is(NUMBERS))
	{
		// cout << "A NUMBER. ";
		// cout << "toekn: " << cur_token.get_token_value() << ", " << cur_token.get_token_tag() << endl;
		TreeNode *cur = get_expr_node(Integer);
		cur->value.num_value = atoi(cur_token.get_token_value().data());
		cur_token = lexer->get_next_token();
		return cur;
	}
	else if (lexer->is(QUOTE_MARK))
	{
		// cout << "A String. ";
		cur_token = lexer->get_next_token();
		// cout << "toekn: " << cur_token.get_token_value() << ", " << cur_token.get_token_tag() << endl;
		TreeNode *cur = get_expr_node(String);
		// cur->value.string_value = const_cast<char*>(cur_token.get_token_value().data());
		char *id = new char[cur_token.get_token_value().length()];
		strcpy(id, cur_token.get_token_value().data());
		cur->value.string_value = id;

		cur_token = lexer->get_next_token();
		cur_token = lexer->get_next_token();
		return cur;
	}
	else if (lexer->is(LE_S_BRACKET))
	{
		// cout << "A ( expr ). ";
		cur_token = lexer->get_next_token();
		TreeNode *cur = expr();
		cur_token = lexer->get_next_token();
		cur_token = lexer->get_next_token();
		return cur;
	}
	else
	{
		cout << "Other factors." << endl;
		cur_token = lexer->get_next_token();
		return NULL;
	}
}

