# include <iostream>
# include <string>
# include "base_object.h"
# include "int_object.h"
# include "array_object.h"
# include "string_object.h"

using namespace std;

typedef void (*FUNC_POINTER)(void);

extern TypeObject ArrayType;

PACK_P_FUNC* pack_func(FUNC_POINTER func_, int is_return_, int arg_count_)
{
	PACK_P_FUNC *f = new PACK_P_FUNC;
	f->func = func_;
	f->is_return = is_return_;
	f->arg_count = arg_count_;
	return f;
}

void arraytype_init(void)
{
	PACK_P_FUNC *t = pack_func((FUNC_POINTER)push_array_ret, 0, 1);
	ArrayType.set_method("push", t);
	ArrayType.set_method("pop", pack_func((FUNC_POINTER)pop_array_ret, 0, 0));
	ArrayType.set_method("insert", pack_func((FUNC_POINTER)insert_array_ret, 0, 2));
	ArrayType.set_method("remove", pack_func((FUNC_POINTER)remove_array_ret, 0, 1));
	ArrayType.set_method("index", pack_func((FUNC_POINTER)index_elem_ret, 1, 1));
}

ArrayObject* create_arrayobject(int elem_num)
{
	ArrayObject *_ = new ArrayObject(elem_num);
	if (NULL == _)
	{
		exit(-1);  // 可以把这里出错的信息进行记录
	}
	return _;
}

int get_length(ArrayObject *arr)
{
	return arr->get_elem_count();
}

int set_item_init(ArrayObject *arr, int index, BaseObject *elem)
{
	int l = arr->get_mem_alloc() / sizeof(BaseObject*);
	int index_max = l - 1;
	if ((index < 0) || (index > index_max))
	{
		return 0;
	}

	BaseObject **pp_elem = arr->get_elem_addr();
	pp_elem[index] = elem;
	l = arr->get_elem_count();
	arr->set_elem_count(l+1);
	return 1;
}

int set_item(ArrayObject *arr, int index, BaseObject *elem)
{
	int index_max = arr->get_elem_count() - 1;
	if ((index < 0) || (index > index_max))
	{
		return 0;
	}

	BaseObject **pp_elem = arr->get_elem_addr();
	pp_elem[index] = elem;  // 这个对象的引用也要相应地改变
	return 1;
}

void get_item(ArrayObject *arr, int index)
{
	BaseObject *p = get_elem(arr, index);
	get_elem_value(p);
}

BaseObject* get_elem_object(BaseObject *arr, BaseObject *i)
{
	ArrayObject *a = (ArrayObject*)arr;
	int index = ((IntObject*)i)->get_value();
	return get_elem(a, index);
}

BaseObject* get_elem(ArrayObject *arr, int index)
{
	/*
	 * 注意这里返回的是泛型指针,需要对返回的指针进行处理,才能提取数据
	 */

	int index_max = get_length(arr) - 1;
	if ((index < 0) || (index > index_max))
	{
		return NULL;
	}
	BaseObject **pp_elem = arr->get_elem_addr();
	return pp_elem[index];
}

void print_elem_value(ArrayObject *arr, int index)
{
	/*
	 * 本函数尝试通过指针获取其值
	 * 无法实现:原因就算能确定其类型,也无法返回多种类型的值
	 */
	BaseObject *_ = get_elem(arr, index);

	if (NULL == _)
	{
		cout << "\nindex :" << index << " 下标越界!" << endl;
		return;
	}
	_->get_type()->print_value(_);
}

void get_elem_value(BaseObject *p)
{
	// 暂时无法实现:
	// 原因,就算获取了各元素的值,也无法返回(只能返回一种类型)
	// return 1;
}

void traverse_all_array(ArrayObject *arr)
{
	/*
	 * 输出效果:[1, 2, 3]
	 */
	// cout << "Traversing" << endl;
	int _;
	int l = get_length(arr);
	cout << "[";
	for (_=0; _<l; _++)
	{
		print_elem_value(arr, _);
		if (_ == (l-1))
		{
			break;
		}
		cout << ", ";
	}
	cout << "]";
}

BaseObject* push_array_ret(BaseObject *arr_, BaseObject *value)
{
	ArrayObject *arr = (ArrayObject*)arr_;
	// cout << "In push_array_ret" << endl;
	push_array(arr, value);
	return NULL;  // 这里可以return None, 但要新建None对象
}

void push_array(ArrayObject *arr, BaseObject *value)
{
	int old_size = arr->get_mem_alloc();
	int elem_num = arr->get_elem_count();
	if (old_size/sizeof(BaseObject*) <= elem_num)
	{
		BaseObject **pp_old_arr = arr->get_elem_addr();
	
		if (NULL == pp_old_arr)
		{
			BaseObject **pp_new_arr = new BaseObject*;
			*pp_new_arr = value;
			arr->set_elem_addr(pp_new_arr);  // !!!
			arr->set_mem_alloc(sizeof(BaseObject*));
		}
		else
		{
			int old_size = arr->get_mem_alloc();
			BaseObject **pp_new_arr = (BaseObject**)realloc(pp_old_arr, old_size + sizeof(BaseObject*));
			arr->set_elem_addr(pp_new_arr);
			*(pp_new_arr + old_size/sizeof(BaseObject*)) = value;
			arr->set_mem_alloc(old_size + sizeof(BaseObject*));
		}
	}
	else
	{
		BaseObject **pp_old_arr = arr->get_elem_addr();
		*(pp_old_arr + elem_num) = value;
	}

	int l = get_length(arr);
	arr->set_elem_count(l+1);
	return;
}


BaseObject* insert_array_ret(ArrayObject *arr, BaseObject *value, IntObject *index)
{
	int i = index->get_value();
	// cout << "i is " << i << endl;
	insert_array(arr, i, value);
	return NULL;
}

void insert_array(ArrayObject *arr, int index, BaseObject *value)
{
	int old_size = arr->get_mem_alloc();
	int length = get_length(arr);

	if ((index > length) || (index < 0))
	{
		return;
	}

	BaseObject **pp_old_arr = arr->get_elem_addr();
	// cout << "old_arr_base is " << pp_old_arr << endl;
	BaseObject **pp_new_arr = (BaseObject**)realloc(pp_old_arr, old_size + sizeof(BaseObject*));
	// 这里old_arr为NULL时, 也可以正常分配内存. 上面就会有问题
	// cout << "new_arr_base is " << pp_new_arr << endl;
	arr->set_elem_addr(pp_new_arr);
	// a b c d e f        s 2  index = 2
	// 0 1 2 3 4 5        length = 6
	memmove(pp_new_arr+index+1, pp_new_arr+index, (length-index)*sizeof(BaseObject*));
	pp_new_arr[index] = value;
	arr->set_elem_count(length+1);
	arr->set_mem_alloc(old_size + sizeof(BaseObject*));
	return;
}

// void insert_array_object(ArrayObject *arr, IntObject *i, BaseObject *value)
// {
// 	// IntObject *intobj = (IntObject*)i;
// 	int index = i->get_value();
// 	insert_array(arr, index, value);
// 	return;
// }

BaseObject* remove_array_ret(ArrayObject *arr, IntObject *index)
{
	int i = index->get_value();
	remove_array(arr, i);
	return NULL;
}

void remove_array(ArrayObject *arr, int index)
{
	/* 
	 * 移除index位置的元素,内存前移 
	 */
	int old_size = arr->get_mem_alloc();
	int length = get_length(arr);

	if ((index < 0) || (index > (length-1)))
	{
		return;
	}

	BaseObject **pp_old_arr = arr->get_elem_addr();
	memmove(pp_old_arr+index, pp_old_arr+index+1, (length-index-1)*sizeof(BaseObject*));
	// 释放最后一个元素的内存,对于最后一个元素,delete, free无法使用
	BaseObject **pp_new_arr = (BaseObject**)realloc(pp_old_arr, (length-1)*sizeof(BaseObject*));
	arr->set_elem_addr(pp_new_arr);
	// BaseObject **pp_del = pp_old_arr + length - 2;
	// delete (pp_del);  // 这里要加括号
	arr->set_elem_count(length-1);
	arr->set_mem_alloc(old_size - sizeof(BaseObject*));
}
void remove_array_object(ArrayObject *arr, IntObject *i)

{
	int index = i->get_value();
	remove_array(arr, index);
	return;
}

BaseObject* pop_array_ret(ArrayObject *arr)
{
	// cout << "In pop_array_ret" << endl;
	pop_array(arr);
	// cout << "End pop_array_ret" << endl;
	return NULL;
}

void pop_array(ArrayObject *arr)
{
	/*
	 * 如果arr已经为空(old_size==0),再进行pop,
	 * 原array([])不变,仍为[]
	 */
	// cout << "In pop array" << endl;
	BaseObject **pp_old_arr = arr->get_elem_addr();
	int old_size = arr->get_mem_alloc();
	if (0 == old_size)
	{
		return;
	}

	BaseObject **pp_new_arr = (BaseObject**)realloc(pp_old_arr, old_size - sizeof(BaseObject*));
	arr->set_elem_addr(pp_new_arr);
	int l = get_length(arr);
	arr->set_elem_count(l-1);
	arr->set_mem_alloc(old_size-sizeof(BaseObject*));
	arr->get_type()->print_value(arr);
	// cout << "Out pop array" << endl;
	return;
}

BaseObject* index_elem_ret(ArrayObject *arr, BaseObject *v)
{
	int i = index_elem(arr, v);
	// cout << "index is " << i << endl;  // 这里-2
	if (-2 == i)
	{
		return NULL;
	}
	else
	{
		IntObject *_ = new IntObject(i);
		return _;
	}
}

int index_elem(ArrayObject *arr, BaseObject *v)
{
	int i = 0;
	int l = get_length(arr);
	// v->get_type()->print_value(v);
	// cout << l << endl;
	for (; i<l; i++)
	{
		// cout << i << endl;
		if (v->get_hash_value() == get_elem(arr, i)->get_hash_value())
		{
			return i;
		}
	}
	return -2;
}

const char* get_elem_string2char(ArrayObject *arr, int i)
{
	/*
	 * 这个元素只能是StringObject类型的	
	 */
	BaseObject *e = get_elem(arr, i);
	StringObject *str = (StringObject*)e;
	string v = str->get_value();
	const char *_ = v.data();
	// cout << _ << endl;
	return _;
}

int index_int_array(ArrayObject *arr, int num)
{
	int i = 0;
	int l = get_length(arr);
	for (; i<l; i++)
	{
		BaseObject *e = get_elem(arr, i);
		IntObject *intobj = (IntObject*)e;
		if (num == intobj->get_value())
		{
			return i;
		}
	}
	return -2;
}

int index_char_array(ArrayObject *arr, char *str)
{
	int i = 0;
	int l = get_length(arr);
	for (; i<l; i++)
	{
		if (0 == strcmp(get_elem_string2char(arr, i), str))
		{
			return i;
		}
	}
	return -2;  // 这里不能用-1, 因为codes以-1结尾
}

int index_value(ArrayObject *arr, string value)
{
	// arr->get_type()->print_value(arr);
	int i = 0;
	int l = get_length(arr);
	hash<string> h;
	for (; i<l; i++)
	{
		if (get_elem(arr, i)->get_hash_value() == h(value))
		{
			return i;
		}
		// cout << get_elem(arr, i)->get_hash_value() << endl;
		// cout << h(value) << endl;
	}

	return -2;
}

int index_value(ArrayObject *arr, int value)
{
	int i = 0;
	int l = get_length(arr);
	hash<int> h;
	for (; i<l; i++)
	{
		if ((get_elem(arr, i)->get_hash_value()) == h(value))
		{
			return i;
		}
	}

	return -2;
}

