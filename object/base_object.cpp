# include <iostream>
# include <string>
# include "base_object.h"
# include "int_object.h"
# include "float_object.h"
# include "string_object.h"
# include "array_object.h"
# include "dict_object.h"
# include "function_object.h"
# include "file_object.h"


using namespace std;

typedef int (*INT_GET)(BaseObject*);
typedef float (*FLOAT_GET)(BaseObject*);
// typedef string (*STRING_GET)(StringObject*);
typedef string (*STRING_GET)(BaseObject*);

// 最后一行要加空行，不然编译出错，原因未知
// (void (*)(BaseObject*))

void print_value_int(BaseObject *p)
{
	cout << ((INT_GET)(p->get_type()->p_get_value()))(p);
}

void print_value_float(BaseObject *p)
{
	cout << ((FLOAT_GET)(p->get_type()->p_get_value()))(p);
}

void print_value_string(BaseObject *p)
{
	/*
	 * 字符串的输出要有双引号, 如: "123"
	 */
	cout << '"';
	cout << ((STRING_GET)(p->get_type()->p_get_value()))(p);
	cout << '"';
}

void print_func(BaseObject *p)
{
	FunctionObject *f = (FunctionObject*)p;
	cout << "\" FunctionObject: " << f->get_func_name() << " \"";
}

void print_file(BaseObject *p)
{
	FileObject *f = (FileObject*)p;
	cout << "\" FileObject: " << f->get_file_name() << "\"";
}

int get_value_int(BaseObject *p)
{
	IntObject *q = NULL;
	q = (IntObject*)p;
	return q->get_value();
}

float get_value_float(BaseObject *p)
{
	FloatObject *q = NULL;
	q = (FloatObject*)p;
	return q->get_value();
}

string get_value_string(BaseObject *p)
{
	StringObject *q = NULL;
	q = (StringObject*)p;
	return q->get_value();
}

BaseObject* get_children_array(BaseObject *arr)
{
	return arr;
}

GET_VALUE p_get_int = (GET_VALUE)(&get_value_int);
GET_VALUE p_get_float = (GET_VALUE)get_value_float;
GET_VALUE p_get_string = (GET_VALUE)get_value_string;

TypeObject IntType("整型", 1, p_get_int, (GET_VALUE)print_value_int, NULL, NULL);  // 类型的名称要使用汉字，并且doc要初始化 
TypeObject FloatType("实型", 1, p_get_float, (GET_VALUE)print_value_float, NULL, NULL);
TypeObject StringType("字符串", 1, p_get_string, (GET_VALUE)print_value_string, NULL, NULL);
TypeObject ArrayType("数组", 2, NULL, (GET_VALUE)traverse_all_array, get_elem_object, (GET_VALUE)get_children_array);
TypeObject DictType("字典", 4, NULL, (GET_VALUE)traverse_all_dict, get_elem_dict, NULL);  // NULL, get_children不应为NULL, 暂设为NULL
TypeObject FunctionType("函数", 0, NULL, (GET_VALUE)print_func, NULL, NULL);
TypeObject FileType("文件", 0, NULL, (GET_VALUE)print_file, NULL, NULL);

