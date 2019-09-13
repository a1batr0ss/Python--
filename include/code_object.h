# ifndef CODE_OBJECT_H
# define CODE_OBJECT_H

# include <string>
# include <stdlib.h>
# include <string.h>
# include "base_object.h"
# include "array_object.h"

# define GET_NAMES() (co->get_names())
# define GET_VALUES() (co->get_values())
# define SET_CODE(co, b) (co->set_a_code_seq(b));
# define SET_CODE_POS(co, pos, b) (co->set_code_pos(pos, b));
# define NO_CODE(co) (co->find_no_code_pos());
# define STORE_NAMES(co, value) (store_in_codeobject(co, value, 0));
# define STORE_VALUES(co, value) (store_in_codeobject(co, value, 1));
# define STORE_CODEOBJECT(co, value, co_func) (store_in_codeobject(co, value, co_func));
# define FIND_LAST_CODE(co, b) (co->find_code_pos(-1, b));

class CodeObject : public BaseObject
{
private:
	string name;
	char *code;
	ArrayObject *var_names;  // 变量名集合(a, b, c, ....)
	ArrayObject *var_values;  // 变量值,常量值集合(1, "strings", ....)
	int pointer_code;
	int code_max_length;
	int names_pointer;
	int values_pointer;

public:
	CodeObject(string name_)
	{
		code_max_length = 256;
		code = new char[code_max_length];
		name = name_;
		memset(code, -1, code_max_length);  // 为code分配空间,在本句之前
		var_names = new ArrayObject;
		var_values = new ArrayObject;
		pointer_code = 0;
		names_pointer = 0;
		values_pointer = 0;

		hash<string> h;
		set_hash_value(h(name_));
	}

	CodeObject(string name_, char *code_)
	{
		name = name_;
		code = code_;
		var_names = new ArrayObject;
		var_values = new ArrayObject;
		pointer_code = 0;
		code_max_length = strlen(code);
		names_pointer = 0;
		values_pointer = 0;

		hash<string> h;
		set_hash_value(h(name_));
	}

	int get_names_pointer()
	{
		return names_pointer;
	}

	int get_values_pointer()
	{
		return values_pointer;
	}

	void increase_names_pointer()
	{
		names_pointer++;
	}

	void increase_values_pointer()
	{
		values_pointer++;
	}

	int get_code_pointer(void)
	{
		return pointer_code;
	}

	void set_code_pointer(int _)
	{
		pointer_code = _;
	}

	void set_code_pos(int pos, char code_)
	{
		code[pos] = code_;
		// cout << "modified pos is " << (int)code_ << endl;
	}

	void increase_code_pointer(void)
	{
		pointer_code++;
	}

	ArrayObject* get_names(void)
	{
		return var_names;
	}

	ArrayObject* get_values(void)
	{
		return var_values;
	}

	void set_name(string name_)
	{
		name = name_;
	}

	/*
	 * 对code的赋值要检索index是否合法
	 */
	string  get_name(void)
	{
		return name;
	}

	char* get_code(void)
	{
		return code;
	}

	void set_codes_begin(char *code_)
	{
		strcpy(code, code_);
	}

	void set_a_code(char code_, int index)
	{
		/*
		 * 在指定位置替换字节码
		 */
		code[index] = code_;
	}

	void set_code_arg(int start, int end, char code_, int arg)
	{
		for (int i=start; i<end; i++)
		{
			if (code_ == code[i])
			{
				code[i+1] = arg;
			}
		}
	}

	/*
	 * @Returns: 下一个将要设置字节码的位置
	 */
	int find_no_code_pos(void)
	{
		int _;
		for (_=0; _<code_max_length; _++)
		{
			if (-1 == code[_])
			{
				return _;
			}
		}
	}

	void set_a_code_seq(char code_)
	{
		/*
		 * 顺序插入一个字节码
		 * 第一个为0(无字节码)处插入
		 */
		int _;
		for (_=0; _<code_max_length; _++)
		{
			if (-1 == code[_])
			{
				code[_] = code_;
				break;
			}
		}

		// code数组空间将不足, 进行扩展
		if (_ == (code_max_length-32))
		{
			extend_code_region();
		}
	}

	void increase_code(int step)
	{
		pointer_code += step;
	}

	void set_pointer_pos(int pos)
	{
		pointer_code = pos;
	}

	/*
	 * @Returns: 指定位置的字节码
	 */
	int index_of_bytecode(int index)
	{
		return code[index];
	}

	/*
	 * 查找字节码序列中倒数第n个ch码
	 * -1表示倒数第一个, -2表示倒数第二个
	 * 用于查找字节码的位置
	 */
	int find_code_pos(int n, int ch)
	{
		int start = find_no_code_pos() - 1;
		while (-1 != start)
		{
			if (code[start] == ch)
			{
				n++;
			}
			if (0 == n)
			{
				return start;
			}

			start--;
		}
	}

	void set_codes_seq(char *codes)
	{
		char *p_code = code;
		while (-1 != (*p_code))
		{
			p_code++;
		}
		strcpy(p_code, codes);

		if (((code+code_max_length)-p_code) < 32)
		{
			extend_code_region();
		}
	}

	void extend_code_region()
	{
		/*
		 * code空间不足时, 扩展, 扩展功能未测试
		 */
		int i = code_max_length;
		code = (char*)realloc(code, i*2);
		code_max_length *= 2;
	}
};

int store_in_codeobject(CodeObject*, int, int);
int store_in_codeobject(CodeObject*, char*, int);
int store_in_codeobject(CodeObject*, string, CodeObject**);

# endif
