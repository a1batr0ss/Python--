# ifndef DICT_OBJECT_H
# define DICT_OBJECT_H

# include <unordered_map>
# include "base_object.h"
# include "array_object.h"

extern TypeObject DictType;

class DictObject : public BaseVarObject
{
private:
	// map<BaseObject*, BaseObject*> dict;
	unordered_map<BaseObject*, BaseObject*> dict;

public:
	DictObject(void)
	{
		// dict[1] = 1;
		// dict[2] = 2;
		// dict[3] = 3;
		// dict[4] = 3;
		set_elem_count(0);
		set_type(&DictType);
	}

	unordered_map<BaseObject*, BaseObject*>& get_map(void)
	{
		// cout << "In get_map " << endl;
		return dict;
	}

	void insert_pair(BaseObject *p_key, BaseObject *p_value)
	{
		/*
		 * 这是暂时要暴露一个接口使用,否则在外部无法向dict中插入数据,
		 * 可以尝试使用指针操作,如对下句
		 * map<BaseObject*, BaseObject*> dict = d->get_map();
		 * 如果可以,那么所有的私有成员都只要暴露给外部指针即可,但可能违反OOP的设计原则
		 * 同时处理同key覆盖的问题
		 */
		// cout << "inserting..." << endl;
		// cout << p_key << endl;
		unordered_map<BaseObject*, BaseObject*>::iterator iter;
		iter = dict.find(p_key);
		if (iter != dict.end())
		{
			dict.erase(iter);
			dict.insert(pair<BaseObject*, BaseObject*>(p_key, p_value));
		}
		else
		{
			dict.insert(pair<BaseObject*, BaseObject*>(p_key, p_value));
			unsigned _ = get_elem_count();
			set_elem_count(_+1);
		}
	}

	// BaseObject* get_value(BaseObject *p_key)
	// {
	// 	return dict[p_key];
	// }

	int get_size(void)
	{
		return dict.size();
	}
};

unordered_map<BaseObject*, BaseObject*> get_map_dict(DictObject*);
void insert_pair_dict(DictObject *, BaseObject*, BaseObject*);
BaseObject* get_value_dict(DictObject*, BaseObject*);
void print_value_dict(DictObject*, BaseObject*);
unsigned get_size_dict(DictObject*);
void traverse_all_dict(DictObject*);
BaseObject* get_elem_dict(BaseObject*, BaseObject*);
ArrayObject* get_all_keys(DictObject*);
ArrayObject* get_all_values(DictObject*);
void modify_value(DictObject*, BaseObject*, BaseObject*);
BaseObject* insert_pair_dict_ret(DictObject*, BaseObject*, BaseObject*);
void dicttype_init();
void remove_entry_dict(DictObject*, BaseObject*);
BaseObject* remove_entry_dict_ret(DictObject*, BaseObject*);

# endif
