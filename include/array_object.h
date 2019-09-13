# ifndef ARRAY_OBJECT_H
# define ARRAY_OBJECT_H

# include <string.h>
# include "base_object.h"
# include "int_object.h"
# include "string_object.h"

extern TypeObject ArrayType;

class ArrayObject : public BaseVarObject
{
private:
	BaseObject **pp_elem_addr;
	int mem_alloc;  // 元素要申请的内在（B）

public:
	ArrayObject(void)
	{
		pp_elem_addr = NULL;
		mem_alloc = 0;
		set_elem_count(0);
		set_type(&ArrayType);
		set_object_size(sizeof(ArrayObject));
	} 

	ArrayObject(int num)
	{
		set_elem_count(0);
		// 如果有元素，则申请相应的空间，暂时不初始化
		mem_alloc = sizeof(BaseObject*) * num;
		pp_elem_addr = new BaseObject*[num];
		memset(pp_elem_addr, 0, mem_alloc);
		set_type(&ArrayType);
		set_object_size(sizeof(ArrayObject));
	}

	int get_mem_alloc(void)
	{
		return mem_alloc;
	}

	void set_mem_alloc(int new_mem)
	{
		mem_alloc = new_mem;
	}

	BaseObject** get_elem_addr(void)
	{
		return pp_elem_addr;
	}

	void set_elem_addr(BaseObject **p)
	{
		pp_elem_addr = p;
	}
};

ArrayObject* create_arrayobject(int);
int get_length(ArrayObject*);
int set_item(ArrayObject*, int, BaseObject*);
void get_item(ArrayObject*, int);
BaseObject* get_elem(ArrayObject*, int);
void get_elem_value(BaseObject*);
void print_elem_value(ArrayObject*, int);
void traverse_all_array(ArrayObject*);
void push_array(ArrayObject*, BaseObject*);
void pop_array(ArrayObject*);
void insert_array(ArrayObject*, int, BaseObject*);
void remove_array(ArrayObject*, int);
int index_elem(ArrayObject*, BaseObject*);
const char* get_elem_string2char(ArrayObject*, int);
int index_char_array(ArrayObject*, char*);
void arraytype_init(void);
void insert_array_object(ArrayObject*, IntObject*, BaseObject*);
void remove_array_object(ArrayObject*, IntObject*);
int index_array_object(ArrayObject*, StringObject*);
int index_int_array(ArrayObject*, int);
BaseObject* push_array_ret(BaseObject*, BaseObject*);
BaseObject* pop_array_ret(ArrayObject*);
BaseObject* insert_array_ret(ArrayObject*, BaseObject*, IntObject*);
BaseObject* remove_array_ret(ArrayObject*, IntObject*);
BaseObject* index_elem_ret(ArrayObject*, BaseObject*);
BaseObject* get_elem_object(BaseObject*, BaseObject*);
PACK_P_FUNC* pack_func(FUNC_POINTER, int, int);
int index_value(ArrayObject*, string);
int index_value(ArrayObject*, int);
int set_item_init(ArrayObject*, int, BaseObject*);

# endif
