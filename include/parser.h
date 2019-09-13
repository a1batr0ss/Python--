# ifndef PARSER_H
# define PARSER_H

# include "lexer.h"
# include "parse.h"

class Parser
{
	private:
		Lexer *lexer;
		Token cur_token;

	public:
		Parser(Lexer *l) : lexer(l){}
		TreeNode* start();
		void sentence(TreeNode*);
		TreeNode* stmt();
		TreeNode* empty();
		TreeNode* stmt_not_empty();
		TreeNode* assign();
		TreeNode* id();
		TreeNode* expr();
		TreeNode* simple_expr();
		TreeNode* term();
		TreeNode* factor();
		TreeNode* numbers();
		TreeNode* if_();
		TreeNode* not_end_array();
		TreeNode* end_array();
		TreeNode* not_end_array_prime(TreeNode*);
		TreeNode* term_prime(TreeNode*);
		TreeNode* simple_expr_prime(TreeNode*);
		TreeNode* sentence_prime(TreeNode*);
		TreeNode* body_stmt();
		TreeNode* body_stmt_prime(TreeNode*);
		TreeNode* for_();
		TreeNode* loop_op();
		TreeNode* id_start();
		TreeNode* define();
};

# endif

