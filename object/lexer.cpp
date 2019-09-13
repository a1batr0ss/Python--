# include <iostream>
# include <fstream>
# include <string.h>
# include <cctype>
# include "tag.h"
# include "lexer.h"

using namespace std;
using namespace tag;

void Lexer::store_token(Token t)
{
	keywords.insert({t.get_token_value(), t});
}

bool Lexer::next(char c)
{
	// 处理复朵的情况
	return true;
}

bool Lexer::is(int t)
{
	if (t == cur_token.get_token_tag())
	{
		return true;
	}
	return false;
}

void Lexer::get_input()
{
	fstream f(file_name);
	char ch;
	int i = 0;
	while (EOF != (ch=f.get()))
	{
		str[i++] = ch;
	}

	str[i] = '#';  // 结束符
	cout << "\n";
}

void Lexer::init_array(char *arr, int l)
{
	int i = 0;
	for (; i<l; i++)
	{
		arr[i] = 0;
	}
}

void Lexer::start()
{
	p = 0;
	init_array(str, 200);
	get_input();
}

Token Lexer::look_ahead()
{
	int p_ = p;
	char peek = str[p_];
	while ('\0' != peek)
	{
		if (' ' == peek)
		{
			p++;
			peek = str[p_];
			continue;
		}
		break;
	}
		
	p_++;
	switch (peek)
	{
		case '+':
			cur_token = Token("+", ADD);
			return cur_token;
		case '-':
			cur_token = Token("-", MINUS);
			return cur_token;
		case '*':
			cur_token = Token("*", TIMES);
			return cur_token;
		case '/':
			cur_token = Token("/", DIV);
			return cur_token;
		case '%':
			cur_token = Token("%", MOD);
			return cur_token;
		case '=':
			cur_token = Token("=", ASSIGN);
			if ('=' == str[p])
			{
				cur_token = Token("==", EQ);
				p_++;
			}
			return cur_token;
		case '>':
			cur_token = Token(">", GREATER);
			return cur_token;
		case '<':
			cur_token = Token("<", LESS);
			return cur_token;
		case ':':
			cur_token = Token(":", COLON);
			return cur_token;
		case ',':
			cur_token = Token(",", COMMA);
			return cur_token;
		case '(':
			cur_token = Token("(", LE_S_BRACKET);
			return cur_token;
		case ')':
			cur_token = Token(")", RG_S_BRACKET);
			return cur_token;
		case '[':
			cur_token = Token("[", LE_M_BRACKET);
			return cur_token;
		case ']':
			cur_token = Token("]", RG_M_BRACKET);
			return cur_token;
		case '{':
			cur_token = Token("{", LE_B_BRACKET);
			return cur_token;
		case '}':
			cur_token = Token("}", RG_B_BRACKET);
			return cur_token;
		case '.':
			cur_token = Token(".", DOT);
			return cur_token;
		case '"':
			cur_token = Token("\"", QUOTE_MARK);
			return cur_token;
		case '#':
			cur_token = Token("#", END);
			return cur_token;
		case '\t':
			cur_token = Token("tab", TAB);
			return cur_token;
		case '\n':
			cur_token = Token("eol", EOL);
			return cur_token;
	}
	p_--;
	if (isdigit(peek))
	{
		init_array(words, 200);
		int t = 0;
		while (isdigit(peek))
		{
			words[t] = peek;
			t++;
			p_++;
			peek = str[p_];		
		}
		words[t] = '\0';
		cur_token = Token(words, NUMBERS);
		return cur_token;
	}

	if (isalpha(peek))
	{
		init_array(words, 200);
		int t = 0;

		while (isalnum(peek))
		{
			words[t] = peek;
			t++;
			p_++;
			peek = str[p_];
		}
		words[t] = '\0';

		if (keywords.find(words) != keywords.end())
		{
			cur_token = keywords[words];
			return cur_token;
		}

		cur_token = Token(words, ID);
		return cur_token;
	}

	cur_token = Token("Not Found", EMPTY);
	p_++;
	return cur_token;

}

Token Lexer::get_next_token()
{
	char peek = str[p];

	while ('\0' != peek)
	{
		if (' ' == peek)
		{
			p++;
			peek = str[p];
			continue;
		}
		break;
	}
		
	// + - * / = ....
	p++;
	switch (peek)
	{
		case '+':
			cur_token = Token("+", ADD);
			return cur_token;
		case '-':
			cur_token = Token("-", MINUS);
			return cur_token;
		case '*':
			cur_token = Token("*", TIMES);
			return cur_token;
		case '/':
			cur_token = Token("/", DIV);
			return cur_token;
		case '%':
			cur_token = Token("%", MOD);
			return cur_token;
		case '=':
			cur_token = Token("=", ASSIGN);
			if ('=' == str[p])
			{
				cur_token = Token("==", EQ);
				p++;
			}
			return cur_token;
		case '>':
			cur_token = Token(">", GREATER);
			return cur_token;
		case '<':
			cur_token = Token("<", LESS);
			return cur_token;
		case ':':
			cur_token = Token(":", COLON);
			return cur_token;
		case ',':
			cur_token = Token(",", COMMA);
			return cur_token;
		case '(':
			cur_token = Token("(", LE_S_BRACKET);
			return cur_token;
		case ')':
			cur_token = Token(")", RG_S_BRACKET);
			return cur_token;
		case '[':
			cur_token = Token("[", LE_M_BRACKET);
			return cur_token;
		case ']':
			cur_token = Token("]", RG_M_BRACKET);
			return cur_token;
		case '{':
			cur_token = Token("{", LE_B_BRACKET);
			return cur_token;
		case '}':
			cur_token = Token("}", RG_B_BRACKET);
			return cur_token;
		case '.':
			cur_token = Token(".", DOT);
			return cur_token;
		case '"':
			cur_token = Token("\"", QUOTE_MARK);
			return cur_token;
		case '#':
			cur_token = Token("#", END);
			return cur_token;			
		case '\t':
			cur_token = Token("tab", TAB);
			return cur_token;
		case '\n':
			cur_token = Token("eol", EOL);
			return cur_token;
	}
	p--;

	// NUMBERS
	if (isdigit(peek))
	{
		init_array(words, 200);
		int t = 0;
		while (isdigit(peek))
		{
			// cout << "In while" << endl;
			words[t] = peek;
			t++;
			p++;
			peek = str[p];		
		}
		words[t] = '\0';
		// p--;
		cur_token = Token(words, NUMBERS);
		return cur_token;
	}
	
	int is_string = 0;
	if (isalpha(peek) || ('\\' == peek))
	{
		init_array(words, 200);
		int t = 0;

		while (isalnum(peek) || ('\\' == peek))
		{
			if ('\\' == peek)
			{
				is_string = 1;
			}
			words[t] = peek;
			t++;
			p++;
			peek = str[p];
		}
		words[t] = '\0';
		// p--;
		if (is_string)
		{
			cur_token = Token(words, STRING);
			return cur_token;
		}

		if (keywords.find(words) != keywords.end())
		{
			cur_token = keywords[words];
			return cur_token;
		}

		cur_token = Token(words, ID);
		return cur_token;
	}

	cur_token = Token("Not Found", EMPTY);
	p++;
	return cur_token;
}

