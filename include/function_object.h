# ifndef FUNCTION_OBJECT_H
# define FUNCTION_OBJECT_H

# include <string>
# include "base_object.h"
# include "array_object.h"

// typedef void (*FUNC_POINTER)(void);
// typedef struct
// {
// 	FUNC_POINTER func;
// 	int is_return;
// 	int arg_count;
// } PACK_P_FUNC;

extern TypeObject FunctionType;

class FunctionObject : public BaseObject
{
private:
	BaseObject *code;
	BaseObject *globals;
	string func_name;
	// ArrayObject *args;
	int arg_num;
	// FUNC_POINTER p_func;
	PACK_P_FUNC *pack_func;

public:
	// FunctionObject(string func_name_)
	// {
	// 	func_name = func_name_;
	// 	code = NULL;
	// 	globals = NULL;
	// 	pack_func = NULL;
	// }

	FunctionObject(string func_name_, BaseObject *code_, BaseObject *globals_, int arg_num_)
	{
		set_type(&FunctionType);
		func_name = func_name_;
		code = code_;
		globals = globals_;
		pack_func = NULL;
		arg_num = arg_num_;
		// args = new ArrayObject(arg_num);
	}

	FunctionObject(string func_name_)
	{
		set_type(&FunctionType);
		func_name = func_name_;
		// pack_func = p_func_;
		code = NULL;
		globals = NULL;
	}

	FunctionObject(PACK_P_FUNC *f)
	{
		set_type(&FunctionType);
		func_name = "used_to_pack_pointer";
		// cout << "create a functionobject" << endl;
		pack_func = f;
		// (*f->func)();
		code = NULL;
		globals = NULL;
	}
	
	int get_arg_num()
	{
		return arg_num;
	}

	/*
	 * ArrayObject* get_args_values()
	 * {
	 *     return args_values;
	 * }
	 */

	PACK_P_FUNC* get_pack_func(void)
	{
		return pack_func;
	}

	string get_func_name(void)
	{
		return func_name;
	}

	BaseObject* get_codeobject(void)
	{
		return code;
	}

	BaseObject* get_globals(void)
	{
		return globals;
	}
};

BaseObject* call_function_pointer(BaseObject*, FunctionObject*, BaseObject**, int);
BaseObject* call_function_pointer(FunctionObject*, BaseObject**, int);
BaseObject* call_method(FunctionObject*, BaseObject**, int);
void init_function_args(FunctionObject*, int, BaseObject**);

# endif
