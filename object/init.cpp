# include <iostream>
# include "init.h"
# include "int_object.h"
# include "base_object.h"
# include "code_object.h"
# include "dict_object.h"
# include "frame_object.h"
# include "file_object.h"
# include "array_object.h"
# include "function_object.h"

using namespace std;

CodeObject *co_main = NULL;
FrameObject *f_cur = NULL;
MemoryPool *consts_pool = NULL;
MemoryPool *heap_pool = NULL;

/*
 * builtin空间中要存放的函数: print, open, locals ...
 * builtin空间中要存放的变量:  ....
 * 暂时仅支持以上成员
 */

void builtin_function_locals()
{
	FrameObject *f = f_cur;
	f->get_locals()->get_type()->print_value(f->get_locals());
}

BaseObject* builtin_function_print(BaseObject *o)
{
	/*
	 * 参数应该是根据栈来决定的, 而不是传入的参数
	 * 参数为几, 就把栈中的几个元素输出, 这里应该是外层决定的, 这个函数只负责把传入的一个对象输出
	 */
	if (NULL != o)
	{
		o->get_type()->print_value(o);
	}
	return NULL;  // 必须要有返回值
}

BaseObject* builtin_function_range(BaseObject *i)
{
	int j = ((IntObject*)i)->get_value();
	ArrayObject *res = new ArrayObject(j);
	for (int _=0; _<j; _++)
	{
		push_array(res, new IntObject(_));
	}
	return res;
}

BaseObject* get_codeobject_main()
{
    return co_main;
}
 
BaseObject* get_frameobject_main()
{
    return f_cur;
}

MemoryPool* get_consts_pool()
{
    return consts_pool;
}

MemoryPool* get_heap_pool()
{
    return heap_pool;
}

void init_compile_environment()
{
	co_main = new CodeObject("main");
	consts_pool = new MemoryPool();
	heap_pool = new MemoryPool();
	consts_pool->init_pool();
	heap_pool->init_pool();
}

void init_runtime_environment(CodeObject *co)
{
	f_cur = new FrameObject(co);
	builtin_init();
	arraytype_init();
	dicttype_init();
	filetype_init();
}

BaseObject *builtin_function_open(BaseObject *mode, BaseObject *file_name)
{
	StringObject *str = (StringObject*)file_name;
	string name = str->get_value();
	int m = ((IntObject*)mode)->get_value();
	FileObject *fp = new FileObject(name, m);
	open_file(fp);

	return fp;
}

void builtin_functioin_gc()
{
	heap_pool->get_roots(f_cur);
	heap_pool->gc_copy(); 
}

void builtin_init()
{
	DictObject *builtin = (DictObject*)(FrameObject::get_builtin());

	StringObject *locals_str = new StringObject("locals");
	StringObject *print_str = new StringObject("print");
	StringObject *open_str = new StringObject("open");
	StringObject *range_str = new StringObject("range");
	StringObject *gc_str = new StringObject("collect");

	PACK_P_FUNC *f_1 = new PACK_P_FUNC;
	
	insert_pair_dict(builtin, locals_str, new FunctionObject(pack_func((void (*)(void))builtin_function_locals, 0, 0)));
	insert_pair_dict(builtin, print_str, new FunctionObject(pack_func((void (*)(void))builtin_function_print, 0, 1)));
	insert_pair_dict(builtin, open_str, new FunctionObject(pack_func((void (*)(void))builtin_function_open, 1, 2)));
	insert_pair_dict(builtin, range_str, new FunctionObject(pack_func((void (*)(void))builtin_function_range, 1, 1)));
	insert_pair_dict(builtin, gc_str, new FunctionObject(pack_func((void (*)(void))builtin_functioin_gc, 0, 0)));
}

