# ifndef FLOAT_OBJECT_H
# define FLOAT_OBJECT_H

# include "base_object.h"

extern TypeObject FloatType;

class FloatObject : public BaseObject
{
private:
	float value;

public:
	FloatObject(float value_)
	{
		set_type(&FloatType);
		value = value_;
		change_ref(INCREASE);
	}

	void set_value(float value_)
	{
		value = value_;
	}

	float get_value(void)
	{
		return value;
	}
};

FloatObject* create_floatobject(float);
FloatObject* float_op_add(FloatObject, FloatObject);
FloatObject* float_op_minus(FloatObject, FloatObject);
FloatObject* float_op_times(FloatObject, FloatObject);
// FloatObject* float_op_mod(FloatObject, FloatObject);
// FloatObject* float_op_and(FloatObject, FloatObject);
// FloatObject* float_op_or(FloatObject, FloatObject);
int float_op_compare(FloatObject, FloatObject);

# endif
