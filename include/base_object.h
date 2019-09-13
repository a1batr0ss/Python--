# ifndef BASE_OBJECT_H
# define BASE_OBJECT_H

# include <string>
# include <iostream>
# include <map>
# include <string>

# define INCREASE 0
# define DECREASE 1

# define COPIED 0
# define NOCOPIED 1

using namespace std;

class TypeObject;

typedef void (*FUNC_POINTER)(void);
typedef struct
{
	FUNC_POINTER func;
	int is_return;
	int arg_count;
} PACK_P_FUNC;

class GCHead
{
	private:
		int object_size;
		int gc_tag;
	
	public:
		GCHead()
		{
			object_size = 0;
			gc_tag = NOCOPIED;
			// cout << "Init gc tag is " << gc_tag << endl;
		}
		int get_gc_tag()
		{
			return gc_tag;
		}
		void set_tag_NOCOPIED()
		{
			gc_tag = NOCOPIED;
		}

		void set_tag_COPIED()
		{
			gc_tag = COPIED;
		}

		void set_object_size(int size)
		{
			object_size = size;
		}

		int get_object_size()
		{
			return object_size;
		}

};

class BaseObject : public GCHead
{
private:
	unsigned ref;  // 引用
	TypeObject *type; // 类型  类型待确定
	// int object_size;
	// int gc_tag;
    long unsigned int hash_value;
public:
	BaseObject(void)
	{
		// std::cout << "In BaseObject's constructor.";
		ref = 0;
		// object_size = 0;
		hash_value = 0;
		// gc_tag = NOCOPIED;
	}

	unsigned get_ref(void)
	{
		return ref;
	}

	long unsigned int get_hash_value()
	{
		return hash_value;
	}

	void set_hash_value(long unsigned int h)
	{
		hash_value = h;
	}

	void change_ref(int change_type)
	{
		if (0 == change_type)
		{
			ref++;
		}
		else
		{
			ref--;
		}
	}

	void set_type(TypeObject *type_)
	{
		type = type_;
	}

	TypeObject* get_type(void)
	{
		return type;
	}
};

class BaseVarObject : public BaseObject
{
	/*
	 * 变长对象：array, dict, 暂时不包括string
	 */
private:
	int elem_count;  // 元素的数量

public:
	BaseVarObject(void)
	{
		elem_count = 0;
	}

	void set_elem_count(int num)
	{
		elem_count = num;
	}

	int get_elem_count(void)
	{
		return elem_count;
	}
};

typedef double (*GET_VALUE)(BaseObject*);
typedef void (*FUNC_POINTER)(void);
typedef BaseObject* (*GET_ELEM)(BaseObject*, BaseObject*);

class TypeObject : public BaseObject
{
private:
	string type_name;  // 类型名称，如int, str 
	string type_doc;
	GET_VALUE get_value;
	GET_VALUE func_print_value;
	map<string, PACK_P_FUNC*> methods;
	BaseObject *iterator;
	GET_VALUE children;
	GET_ELEM get_elem;
	int op_flag;

public:
	TypeObject(string type_name_, int flag, GET_VALUE get_value_, GET_VALUE print_value_, GET_ELEM get_elem_, GET_VALUE get_children_)
	{
		/* 各类型对象的初始化 */
		type_name = type_name_;
		op_flag = flag;
		get_value = get_value_;
		func_print_value = print_value_;
		iterator = NULL;
		get_elem = get_elem_;
		children = get_children_;
	}

	TypeObject(string type_name_, int flag)
	{
		/* 各类型对象的初始化 */
		type_name = type_name_;
		op_flag = flag;
		get_value = NULL;
		iterator = NULL;
		get_elem = NULL;
		children = NULL;
	}

	GET_VALUE get_children()
	{
		return children;
	}

	GET_ELEM get_elem_seq(void)
	{
		return get_elem;
	}

	BaseObject* get_iter(void)
	{
		return iterator;
	}

	void set_iter(BaseObject *iterator_)
	{
		iterator = iterator_;
	}

	GET_VALUE p_get_value(void)
	{
		return get_value;
	}

	void set_method(string name, PACK_P_FUNC *f)
	{
		// cout << "appending method.\n" << endl;
		methods.insert(pair<string, PACK_P_FUNC*>(name, f));
	}

	PACK_P_FUNC* get_method(string name)
	{
		// cout << "In BaseObject: " << name << endl;
		return methods[name];
	}

	void print_value(BaseObject *p)
	{
		/* 
		 *  TODO 输出值 obj.get_value()  // 无换行符
		 */
		func_print_value(p);
	}

	string get_type_name(void)
	{
		return type_name;
	}

};

# endif
