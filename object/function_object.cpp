# include <iostream>
# include "function_object.h"

using namespace std;

typedef void (*FUNC)(void);

BaseObject* call_function_pointer(BaseObject *var, FunctionObject *func, BaseObject **args, int arg_count)
{
	// cout << "In call_function_pointer" << endl;
	// var->get_type()->print_value(var);
	// func->get_type()->print_value(func);
	// int i = 0;
	// for (; i<arg_count; i++)
	// {
		// cout << "In call_function_pointer" << endl;
		// args[i]->get_type()->print_value(args[i]);
	// }
	// cout << "arg is " << arg_count << endl;

	FUNC f = func->get_pack_func()->func;

	int is_return = func->get_pack_func()->is_return;
	int arg_num = func->get_pack_func()->arg_count;
	// cout << "is_return is " << is_return << endl;
    // 如果arg_num与arg_count不匹配, 应该退出

	switch(arg_count)
	{
		case 0:
		{
			// cout << "case 0" << endl;
			// var->get_type()->print_value(var);
			typedef BaseObject* (*F)(BaseObject*);
			// f();
			F f_ = (F)f;  // 注意这里不能写F f = (F)f;
			BaseObject *ret = (*f_)(var);
			if (NULL != ret)
			{
				return ret;
			}
			break;
		}
		case 1:
		{
			typedef BaseObject* (*F)(BaseObject*, BaseObject*);
			F f_ = (F)f;
			// cout << "case 1." << var << endl;
			// var->get_type()->print_value(var);
			// cout << "var is " << var << endl;	
			args[0]->get_type()->print_value(args[0]);
			BaseObject *ret = (*f_)(var, args[0]);
			// cout << "case 1." << endl;
			if (NULL != ret)
			{
				return ret;
				// cout << "ret is not null." << endl;
			}
			break;
		}
		case 2:
		{
			typedef BaseObject* (*F)(BaseObject*, BaseObject*, BaseObject*);
			F f_=  (F)f;
			BaseObject *ret = (*f_)(var, args[0], args[1]);
			if (NULL != ret)
			{
				return ret;
			}
			break;
		}
		case 3:
		{

		}
		default:
		{

		}
	}
}

BaseObject* call_function_pointer(FunctionObject *func, BaseObject **args, int arg_count)
{
	// cout << "in call_function_pointer" << endl;
	FUNC f = func->get_pack_func()->func;

	int is_return = func->get_pack_func()->is_return;
	int arg_num = func->get_pack_func()->arg_count;
	// cout << "arg_count is " << arg_count << endl;
	// cout << "arg_num is " << arg_num << endl;

	switch(arg_count)
	{
		case 0:
		{
			typedef BaseObject* (*F)();
			F f_ = (F)f;  // 注意这里不能写F f = (F)f;
			BaseObject *ret = (*f_)();
			if (NULL != ret)
			{
				// TODO: 结果应该入栈, 或者返回
				return ret;
			}
			break;
		}
		case 1:
		{
			typedef BaseObject* (*F)(BaseObject*);
			F f_ = (F)f;

			BaseObject *ret = (*f_)(args[0]);
			if (NULL != ret)
			{
				return ret;
			}
			break;
		}
		case 2:
		{
			typedef BaseObject* (*F)(BaseObject*, BaseObject*);
			F f_=  (F)f;
			BaseObject *ret = (*f_)(args[0], args[1]);
			if (NULL != ret)
			{
				return ret;
			}
			break;
		}
		case 3:
		{
			typedef BaseObject* (*F)(BaseObject*, BaseObject*, BaseObject*);
			F f_=  (F)f;
			BaseObject *ret = (*f_)(args[0], args[1], args[2]);
			if (NULL != ret)
			{
				return ret;
			}
			break;
		}
		default:
		{

		}
	}
}

