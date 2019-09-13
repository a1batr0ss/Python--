# include <iostream>
# include <unordered_map>
# include "base_object.h"
# include "dict_object.h"
# include "array_object.h"

using namespace std;

// // 首先要由DictObject对象获取其map(dict属性),作为公共接口使用
// map<BaseObject*, BaseObject*> get_map_dict(DictObject *d)
// {
// 	return d->get_map();
// }

typedef void (*FUNC_POINTER)(void);

extern TypeObject DictType;

void dicttype_init()
{
	DictType.set_method("insert", pack_func((FUNC_POINTER)insert_pair_dict_ret, 0, 2));
	DictType.set_method("remove", pack_func((FUNC_POINTER)remove_entry_dict_ret, 0, 1));
}

BaseObject* insert_pair_dict_ret(DictObject *d, BaseObject *value, BaseObject *key)
{
	// cout << "In insert_pair_dict_ret" << endl;
	insert_pair_dict(d, key, value);
	return NULL;
}

void insert_pair_dict(DictObject *d, BaseObject *key, BaseObject *value)
{
	// map<BaseObject*, BaseObject*> dict = d->get_map();
	// dict.insert(pair<BaseObject*, BaseObject*>(key, value));
	// // cout << "In insert_pair_dict" << endl;
	// unsigned _ = d->get_elem_count();
	// d->set_elem_count(_+1);
	// // cout << "End insert_pair_dict" << endl;
	d->insert_pair(key, value);
}

BaseObject* remove_entry_dict_ret(DictObject *d, BaseObject *key)
{
	remove_entry_dict(d, key);
	return NULL;
}

void remove_entry_dict(DictObject *d, BaseObject *key)
{
	unordered_map<BaseObject*, BaseObject*> &dict = d->get_map();
	for (auto _=dict.begin(); _!=dict.end(); _++)
	{
		if (key->get_hash_value() == _->first->get_hash_value())
		{
			dict.erase(_->first);
		}
	}
}

BaseObject* get_value_dict(DictObject *d, BaseObject *key)
{
	/*
	 * 作为print_value_dict的接口
	 * 返回key对应的value的BaseObject指针
	 */
	unordered_map<BaseObject*, BaseObject*> dict = d->get_map();
	// cout << d->get_size() << endl;
	for (auto _=dict.begin(); _!=dict.end(); _++)
	{
		if (key->get_hash_value() == _->first->get_hash_value())
		{
			return _->second;
		}
	}
	// cout << dict.size() << endl;
	return NULL;
}

BaseObject* get_elem_dict(BaseObject *dict, BaseObject *key)
{
	DictObject *d = (DictObject*)dict;
	return get_value_dict(d, key);
}

void print_value_dict(DictObject *d, BaseObject *key)
{
	BaseObject *_ = get_value_dict(d, key);  // !!!
	// cout << "In print_value_dict: " << _ << endl;
	_->get_type()->print_value(_);
	// cout << _->get_type()->get_type_name();
}

unsigned get_size_dict(DictObject *d)
{
	return d->get_elem_count();
}

void modify_value(DictObject *dict, BaseObject *key, BaseObject *new_value)
{
	// 如果修改失败(没有这个key)... 暂不做处理
	unordered_map<BaseObject*, BaseObject*> &d = dict->get_map();
	// if (d.find(key))
	// {
		// cout << "value is " << d[key] << endl;
		d[key] = new_value;
		// cout << "value is " << d[key] << endl;
		// new_value->get_type()->print_value(new_value);
	// }
}

/**
 * 未测试
 */
ArrayObject* get_all_values(DictObject *dict)
{
	ArrayObject *arr = new ArrayObject();
	unordered_map<BaseObject*, BaseObject*> &d = dict->get_map();
	auto iter_end = d.end();
	if (0 != dict->get_size())
	{
		for (auto _=d.begin(); _!=iter_end; _++)
		{
			// push_array(arr, _->second);
			push_array(arr, (BaseObject*)&(_->second));
		}
	}
	return arr;
}

/**
 * 未测试
 */
ArrayObject* get_all_keys(DictObject *dict)
{
	ArrayObject *arr = new ArrayObject();
	unordered_map<BaseObject*, BaseObject*> &d = dict->get_map();
	auto iter_end = d.end();
	if (0 != dict->get_size())
	{
		for (auto _=d.begin(); _!=iter_end; _++)
		{
			// BaseObject *tmp = _->first;
			// push_array(arr, _->first);
			push_array(arr, (BaseObject*)&(_->first));
		}
	}
	return arr;
}

void traverse_all_dict(DictObject *dict)
{
	/*
	 * 调用各BaseObject*的print操作,指定好格式
	 * 就是输出DictObject中map的key和value
	 * 格式:
	 * {key1: value1, key2: value2, key3: value3}
	 */
	// cout << "traversing..." << endl;
	unordered_map<BaseObject*, BaseObject*> d = dict->get_map();
	// cout << "getted map." << endl;
	int l = dict->get_elem_count();
	int i = 0;
	auto iter_end = d.end();
	cout << "{";
	if (0 != dict->get_size())
	{
		// iter_end--;  // 如果字典为空, 这里会溢出, 所以进行判断 dict->get_size == 0
		
		for (auto _=d.begin(); _!=d.end(); _++)
		{
			// _->member 就是要输出的对象的BaseObject指针
			_->first->get_type()->print_value(_->first);
			cout << ": ";
			_->second->get_type()->print_value(_->second);
			if (i == (l-1))
			{
				break;
			}
			/*
			 * if (_ == iter_end)  // d.end() 这个iterator不支持-, 所以不能直接d.end()-1
			 * {
			 *     break;
			 * }
			 */
			cout << ", ";
		}
	}
	cout << "}";
}
