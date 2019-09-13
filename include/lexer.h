# ifndef LEXER_H
# define LEXER_H

# include <string>
# include <string.h>
# include <unordered_map>
# include "tag.h"

using namespace std;
using namespace tag;

class Token
{
	private:
		string value;
		int tag;

	public:
		Token(){}
		Token(string v, int t) : value(v), tag(t) {}
		string get_token_value()
		{
			return value;
		}

		int get_token_tag()
		{
			return tag;
		}

		void set_token_tag(int t)
		{
			tag = t;
		}

		void set_token_value(string v)
		{
			value = v;
		}
};


class Lexer
{
	private:
		char peek;
		char words[200];
		char str[1024];
		unordered_map<string, Token> keywords;
		int lineno;  // 编译文件时有用, 交互式时暂时不用
		int p;
		string file_name;
		Token cur_token;

	public:
		Lexer(string file_name_)
	   	{
			file_name = file_name_;
			init_array(str, 1024);
			store_token(Token("if", IF));
			store_token(Token("else", ELSE));
			store_token(Token("for", FOR));
			store_token(Token("in", IN));
			store_token(Token("def", DEF));
			store_token(Token("break", BREAK));
			// store_token(Token("print", PRINT));
			store_token(Token("continue", CONTINUE));
			store_token(Token("return", RETURN));

			peek = ' ';
			p = 0;
			lineno = 1;
		}
		void store_token(Token);
		bool next(char);
		Token get_next_token();
		Token look_ahead();
		bool is(int);
		void get_input();
		void start();
		int get_lineno()
		{
			return lineno;
		}
	
		void set_str(char *str_)
		{
			init_array(str, 200);
			strcpy(str, str_);
		}

		void init_array(char*, int);
};

# endif

