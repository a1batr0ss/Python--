# include <iostream>
# include "init.h"
# include "gc_object.h"
# include "code_object.h"
# include "array_object.h"
# include "factory_object.h"


int store_in_codeobject(CodeObject *co, int value, int flag)
{
	/*
	 * 无重复地存放对象
	 * 把value存入codeobject的names表或values表, 
	 * flag为0, 存入names表, flag为1存入values表
	 */
	MemoryPool *pool = NULL;
	ArrayObject *arr = NULL;
	int res = 0;
	if (0 == flag)
	{
		arr = co->get_names();
		res = co->get_names_pointer();
		pool = get_consts_pool();
	}
	else
	{
		arr = co->get_values();
		res = co->get_values_pointer();
		pool = get_heap_pool();
	}

	int pos = index_value(arr, value);
	if (-2 == pos)
	{
		// IntObject *obj = new IntObject(value);
		IntObject *obj = FactoryObject::create_object(pool, value);
		push_array(arr, obj);
		if (0 == flag)
		{
			co->increase_names_pointer();
		}
		else
		{
			co->increase_values_pointer();
		}
	}
	else
	{
		res = pos;
	}

	return res;
}

int store_in_codeobject(CodeObject *co, char *value, int flag)
{
	ArrayObject *arr = NULL;
	MemoryPool *pool = NULL;
	int res = 0;
	if (0 == flag)
	{
		arr = co->get_names();
		res = co->get_names_pointer();
		pool = get_consts_pool();
	}
	else
	{
		arr = co->get_values();
		res = co->get_values_pointer();
		pool = get_heap_pool();
	}

	int pos = index_value(arr, value);
	if (-2 == pos)
	{
		StringObject *obj = FactoryObject::create_object(pool, value);
		push_array(arr, obj);
		if (0 == flag)
		{
			co->increase_names_pointer();
		}
		else
		{
			co->increase_values_pointer();
		}
	}
	else
	{
		res = pos;
	}

	return res;
}

int store_in_codeobject(CodeObject *co, string code_name, CodeObject **co_func)
{
	string code_name_ = "#" + code_name;
	ArrayObject *arr = co->get_names();
	int res = co->get_names_pointer();

	int pos = index_value(arr, code_name_);

	if (-2 == pos)
	{
		CodeObject *obj = new CodeObject(code_name);
		push_array(arr, obj);
		co->increase_names_pointer();
	}
	else
	{
		res = pos;
	}
	*co_func = (CodeObject*)get_elem(arr, res);

	return res;
}

