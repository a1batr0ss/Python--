# include <fstream>
# include "parse.h"
# include "ast.h"
# include "init.h"
# include "base_object.h"
# include "frame_object.h"
# include "exec_bytecode.h"
# include "comp_tools.h"
# include "lexer.h"
# include "parser.h"


int main(int argc, char *args[])
{
	if (1 == argc)
	{
		init_compile_environment();
		// CodeObject *co_main = NULL;  // 会把全局变量置空 

		while (true)
		{
			// 1. compile
			TreeNode *tree = generate_ast();
			int parse_status = tree->kind.expr;
			if (1 == parse_status)
			{
				continue;
			}
			CodeObject *co_main = compile_ast(tree);	// ==> bytecode
		
			// 2. runtime	
			if (1 == tree->value.num_value)
			{
				init_runtime_environment(co_main);
			}
	
			FrameObject *f_main = (FrameObject*)(get_frameobject_main());
			// execute_byte_code(f_main);
			f_main->execute_byte_code();
		}
	}
	else
	{
		init_compile_environment();
		Lexer *lexer = new Lexer(args[1]);
		Parser *parser = new Parser(lexer);
		TreeNode *tree = parser->start();  // generate_ast;
		CodeObject *co_main = compile_ast_file(tree);
		init_runtime_environment(co_main);
		FrameObject *f_main = (FrameObject*)(get_frameobject_main());
		// execute_byte_code(f_main);
		f_main->execute_byte_code();
	}

	return 0;
}

