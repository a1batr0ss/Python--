# ifndef FRAME_OBJECT_H
# define FRAME_OBJECT_H

# include "code_object.h"
# include "base_object.h"
# include "dict_object.h"
# include "stack.h"

class FrameObject : public BaseObject
{
private:
	BaseObject *code;  // CodeObject
	BaseObject *locals;
	BaseObject *globals;  // global和local名字空间
	LinkStack *runtime_stack;  // 运行时栈
	static BaseObject *builtin;

public:
	FrameObject(BaseObject *code_)
	{
		code = code_;
		globals = new DictObject;
		locals = globals;
		runtime_stack = new LinkStack;
	}

	FrameObject(BaseObject *code_, BaseObject *globals_)
	{
		code = code_;
		globals = globals_;
		locals = new DictObject;
		runtime_stack = new LinkStack;
	}

	FrameObject(BaseObject *code_, BaseObject *globals_, BaseObject *locals_)
	{
		code = code_;
		globals = globals_;
		locals = locals_;
		runtime_stack = new LinkStack;
	}

	BaseObject* get_code(void)
	{
		return code;
	}

	BaseObject* get_locals(void)
	{
		return locals;
	}

	BaseObject* get_globals(void)
	{
		return globals;
	}

	static BaseObject* get_builtin()
	{
		return builtin;
	}

	LinkStack* get_stack(void)
	{
		return runtime_stack;
	}

	BaseObject* execute_byte_code();

	~FrameObject(void)
	{
		delete runtime_stack;
	}
};

void print_locals(FrameObject*);

# endif
