# ifndef INT_OBJECT_H
# define INT_OBJECT_H

# include "base_object.h"
# include "gc_object.h"

extern TypeObject IntType;  // 这句不能省，引用base_object.cpp文件中的IntType

class IntObject : public BaseObject  // 不可加virtual 
{
private:
	int value;

public:
	IntObject(int value_)
	{
		set_type(&IntType);
		value = value_;
		change_ref(INCREASE);
		
		hash<int> h;
		set_hash_value(h(value_));
		set_object_size(sizeof(IntObject));
	}

	static IntObject* get_intobject(MemoryPool *pool, int value)
	{
		char *mem = pool->get_memory(sizeof(IntObject));
		// cout << "get_intobject " << (void*)mem << endl;
		IntObject *obj = new (mem)IntObject(value);
		return obj;
	}

	void set_value(int value_)
	{
		value = value_;
	}

	int get_value(void)  // 这个方法应该放在类型对象中
	{
		return value;
	}
};

IntObject* create_intobject(int);
IntObject* int_op_add(IntObject, IntObject);
IntObject* int_op_minus(IntObject, IntObject);
IntObject* int_op_times(IntObject, IntObject);
IntObject* int_op_mod(IntObject, IntObject);
IntObject* int_op_and(IntObject, IntObject);
IntObject* int_op_or(IntObject, IntObject);
IntObject* int_op_div(IntObject, IntObject);
int int_op_compare(IntObject, IntObject);

# endif
