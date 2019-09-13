# include <stdlib.h>
# include <malloc.h>
# include <iostream>
# include <string.h>
# include "gc_object.h"
# include "array_object.h"
# include "frame_object.h"
# include "dict_object.h"

extern MemoryPool *consts_pool;
extern MemoryPool *heap_pool;

using namespace std;

void MemoryPool::init_pool()
{
	from_start = new_malloc(total_size);
	from_region_surplus = (proportion*total_size*1.0) / (proportion+1);
	to_region_surplus = total_size - from_region_surplus;
	to_start = from_start + from_region_surplus;

	from_pointer = from_start;
	to_pointer = to_start;

	// cout << "========In init: ===========" << endl;
	// cout << "from_pointer is " << (void*)from_pointer << endl;
	// cout << "to_pointer is " << (void*)to_pointer << endl;
	// cout << "total_size is " << total_size << endl;
	// cout << "from_region_surplus is " << from_region_surplus << endl;
	// cout << "==========out init===========" << endl;
}

char* MemoryPool::new_malloc(int size)
{
	return (size==0) ? NULL : (char*)malloc(size);
}

// new_object由工厂来生产, 这里只负责空间的分配
char* MemoryPool::get_memory(int size)
{
	char *alloc_start = NULL;
	if (size > from_region_surplus)
	{
		gc_copy();

		if (size > from_region_surplus)
		{
			// TODO: gc failed, 此时要么分配失败, 要么扩大堆(推荐)
			extend_pool();
		}
	}
	else
	{
		alloc_start = from_pointer;
		from_pointer += size;
		from_region_surplus -= size;
	}

	return alloc_start;
}

/**
 * 语法没问题, 理论上可以扩展堆
 * 懒得测试, 先放这
 */
void MemoryPool::extend_pool()
{
	total_size = total_size * extend_coefficient;
	char *new_from_start = new_malloc(total_size);

	from_region_surplus = (proportion*total_size*1.0) / (proportion+1);
	to_region_surplus = total_size - from_region_surplus;
	char *new_to_start = new_from_start + from_region_surplus;

	// 复制内存内容
	memmove(new_from_start, from_start, from_pointer-from_start);
	memmove(new_to_start, to_start, to_pointer-to_start);

	// 释放原内存
	free(from_start);
	free(to_start);

	from_start = new_from_start;
	to_start = new_to_start;
	from_pointer = from_start;
	to_pointer = to_start;
}

void MemoryPool::gc_copy()
{
	ArrayObject *r = (ArrayObject*)roots;  // 这里不能写ArrayObject *roots = (ArrayObject*)roots;
	int l = get_length(r);

	for (int i=0; i<l; i++)
	{
		BaseObject *_ = get_elem(r, i);
		BaseObject **p = (BaseObject**)_;

		(*p) = copy_object(*p);
		cout << (BaseObject*)_ << " ---> " << *p << endl;
		if (NULL == *p)
		{
			return;  // 说明此时to空间无法继续分配, 不进行from_to的交换, size仍然大于from_region_surplus
		}
	}

	swap_from_to();
}

void MemoryPool::reset_gc_tag()
{
	ArrayObject *r = (ArrayObject*)roots;
	int l = get_length(r);
	for (int _=0; _<l; _++)
	{
		// 以下强转可能多余
		BaseObject *p = get_elem(r, _);
		BaseObject **pp = (BaseObject**)p;
		(*pp)->set_tag_NOCOPIED();
	}
}

/*
 * 复制对象
 */
BaseObject* MemoryPool::copy_object(BaseObject *obj)
{
	BaseObject *p = (BaseObject*)to_pointer;
	if (obj->get_gc_tag() != COPIED)
	{
		// TODO: copy_data($free, obj, obj.size)
		int size = obj->get_object_size();
		// cout << "to_region_surplus is " << to_region_surplus << endl;
		if (size < to_region_surplus)
		{
			copy_data(to_pointer, obj);
			obj->set_tag_COPIED();
			to_pointer += size;
		}
		else
		{
			return NULL;
		}
		/*
		 * for (child : children(obj))
		 * {
		 *     *child = copy_object(*child);
		 * }
		 */
		ArrayObject* children = (ArrayObject*)get_children(obj);
		if (NULL != children)
		{
			int l = children->get_elem_count();
			for (int i=0; i<l; i++)
			{
				BaseObject *_ = get_elem(children, i);
				BaseObject **tmp = &_;
				*tmp = copy_object(*tmp);

				// set_item只针对array_object
				set_item(children, i, *tmp);
				
				cout << " " << _ << " ---> " << *tmp << endl;
			}
		}
	}
	return p;
}

int MemoryPool::copy_data(char *dest, BaseObject *obj)
{
	int size = obj->get_object_size();
	
	memmove(dest, (char*)obj, size);

	return size;
}

// get_roots() => map
// 由locals中的变量作为roots
// 遍历locals中的变量value(name, 怎么处理? 不处理, 只处理value) 
// 不是回收指针, 而是堆上的对象
void MemoryPool::get_roots(BaseObject *f)
{
	// 遍历locals, 存放到Array<BaseObject*>中, 用于遍历
	DictObject *locals = (DictObject*)(((FrameObject*)f)->get_locals());
	ArrayObject *r = get_all_values(locals);  // 得到所有的values
	roots = r;
}

BaseObject* MemoryPool::get_children(BaseObject *o)
{
	// TODO: get_children
	typedef BaseObject* (*FUNC)(BaseObject*);
	FUNC f = FUNC(o->get_type()->get_children());
	if (NULL == f)
	{
		return NULL;
	}
	BaseObject *ret = (*f)(o);
	return ret;
}

