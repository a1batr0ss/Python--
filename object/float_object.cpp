# include <iostream>
# include "float_object.h"

using namespace std;

FloatObject* create_floatobject(float value)
{
	FloatObject *_ = new FloatObject(value);  // 应检查是否new成功
	if (NULL == _)
	{
		exit(-1);
	}
	return _;
}

FloatObject* float_op_add(FloatObject x, FloatObject y)
{
	float x_val = x.get_value();
	float y_val = y.get_value();
	float res = x_val + y_val;
	return create_floatobject(res);
}

FloatObject* float_op_minus(FloatObject x, FloatObject y)
{
	float x_val = x.get_value();
	float y_val = y.get_value();
	float res = x_val - y_val;
	return create_floatobject(res);
}

FloatObject* float_op_times(FloatObject x, FloatObject y)
{
	float x_val = x.get_value();
	float y_val = y.get_value();
	float res = x_val * y_val;
	return create_floatobject(res);
}

// FloatObject* float_op_mod(FloatObject x, FloatObject y)
// {
// 	float x_val = x.get_value();
// 	float y_val = y.get_value();
// 	float res = x_val % y_val;
// 	return create_floatobject(res);
// }

// FloatObject* float_op_and(FloatObject x, FloatObject y)
// {
// 	float x_val = x.get_value();
// 	float y_val = y.get_value();
// 	float res = x_val & y_val;
// 	return create_floatobject(res);
// }

// FloatObject* float_op_or(FloatObject x, FloatObject y)
// {
// 	float x_val = x.get_value();
// 	float y_val = y.get_value();
// 	float res = x_val | y_val;
// 	return create_floatobject(res);
// }

int float_op_compare(FloatObject x, FloatObject y)
{
	float x_val = x.get_value();
	float y_val = y.get_value();
	int res = (x_val > y_val) ? 1 : (x_val < y_val) ? -1 : 0;
	return res;  // return x_val - y_val;
}
