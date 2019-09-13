# include <iostream>
# include <string>
# include <string.h>
# include "array_object.h"
# include "int_object.h"
# include "string_object.h"
# include "comp_tools.h"
# include "stack.h"
# include "frame_object.h"
# include "code_object.h"
# include "exec_bytecode.h"

using namespace std;

int seperate_string2elem_push_stack(char *str, LinkStack *runtime_stack, ArrayObject *values, char ch)
{
	char *p = str;
	int is_string = 0;
	int num = 0;
	char _[32];
	int i = 0;  // 用于遍历上面的字符数组
	while ('\0' != (*p))
	{
		if ('"' == (*p))
		{
			is_string++;
			p++;
		}

		if (ch == (*p))
		{
			num++;
			_[i] = '\0';
			// new StringObject, 要在这里区分数据的类型, int, string...
			if (is_string > 0)
			{
				string s(_);
				// cout << _ << endl;
				StringObject *t = new StringObject(s);
				push_array(values, t);
				// push stack
				PUSH(t)
				is_string = 0;		
			}
			else
			{
				int num = atoi(_);
				IntObject *t = new IntObject(num);
				push_array(values, t);

				PUSH(t)
			}

			i = 0;
			p++;
			continue;
		}
		_[i] = *p;
		p++;
		i++;
	}
	// 要对最后一个元素压栈
	_[i] = '\0';
	// PUSH(t);
	if (is_string > 0)
	{
		string s(_);
		StringObject *t = new StringObject(s);
		push_array(values, t);
		// push stack
		PUSH(t)				
	}
	else
	{
		int num = atoi(_);
		IntObject *t = new IntObject(num);
		push_array(values, t);

		PUSH(t)
	}
	num++;
	// cout << num << endl;
	return num;
}

int seperate_string2elem(char *str, char ch)
{
	/**
	 * 仅用于返回个数
	 */
	char *p = str;
	int num = 0;
	int i = 0;  // 用于遍历上面的字符数组
	while ('\0' != (*p))
	{
		if (ch == (*p))
		{
			num++;
			p++;
			continue;
		}
		p++;
		i++;
	}
	num++;
	return num;
}

int get_key_value_num(char *str, char ch)
{
	// 先push key, 再push value
	char *p = str;
	int num = 0;
	char _[64];
	int i = 0;

	while ('\0' != (*p))
	{
		while ((',' != (*p)) && ('\0' != (*p)))  // 这里不能使用||
		{
			_[i] = *p;
			p++;
			i++;
		}
		_[i] = '\0';
		i = 0;
		if (0 == strlen(_))
		{
			break;
		}
		p++;
		num++;
	}
	return num;	
}

int seperate_string2elem_dict(char *str, LinkStack *runtime_stack, ArrayObject *values)
{
	// 先push key, 再push value
	char *p = str;
	int num = 0;
	int is_string = 0;
	char _[64];
	int i = 0;

	while ('\0' != (*p))
	{
		// 可以直接对整个字符串进行分割
		// 也可以先把整个字符串按,分割, 然后再进行每个键值对进行分割
		// 可以改造上面的函数, 加入第三个参数
		// cout << "12" << endl;
		while ((',' != (*p)) && ('\0' != (*p)))  // 这里不能使用||
		{
			_[i] = *p;
			p++;
			i++;
			// cout << "34" << endl;

		}
		_[i] = '\0';
		i = 0;
		// cout << "string is " << _ << endl;
		if (0 == strlen(_))
		{
			break;
		}
		seperate_string2elem_push_stack(_, runtime_stack, values, ':');
		p++;
		num++;
	}
	return num;
}

int get_compare_type(string op)
{
	if (">" == op)
	{
		return GREATER_;
	}
	else if ("<" == op)
	{
		return LESS_;
	}
	else if ("==" == op)
	{
		return DOUBLE_EQUALS;
	}
	else if (">=" == op)
	{
		return GREATER_EQUALS;
	}
	else if ("<=" == op)
	{
		return LESS_EQUALS;
	}
	else
	{
		return 0;
	}
}

int get_file_lines(char *file_name)
{
	FILE *f = fopen(file_name, "r");
	int line = 0;
	char ch;
	int flag = 0;
	while (!feof(f))
	{
		ch = fgetc(f);
		if ('\n' == ch)
		{
			line++;
			if (1 == flag)
			{
				line--;
			}
			flag = 1;
		}
		else
		{
			flag = 0;
		}
	}
	fclose(f);
	return line;
}

void set_arg_break(CodeObject *co, int n)
{
	int last_for = co->find_code_pos(-1, FOR_ITER);
	int end_code = co->find_no_code_pos();
	co->set_code_arg(last_for, end_code, BREAK_LOOP, n);
}

