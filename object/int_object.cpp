// # include "base_object.h"
# include <iostream>
# include "int_object.h"  // 这里iostream和int_object.h的先后顺序不能换，千万不能换，换了之后IntObject会调用默认无参构造函数

using namespace std;

// 可以改为宏定义
IntObject* create_intobject(int value)
{
	IntObject *_ = new IntObject(value);
	if (NULL == _)
	{
		exit(-1);
	}
	return _;
}

IntObject* int_op_add(IntObject x, IntObject y)
{
	int x_val = x.get_value();
	int y_val = y.get_value();
	int res = x_val + y_val;  // 这里应该有溢出处理
	return create_intobject(res);
}

/*
	x.value - y.value
*/
IntObject* int_op_minus(IntObject x, IntObject y)
{
	int x_val = x.get_value();
	int y_val = y.get_value();
	int res = x_val - y_val;
	return create_intobject(res);
}

IntObject* int_op_times(IntObject x, IntObject y)
{
	int x_val = x.get_value();
	int y_val = y.get_value();
	int res = x_val * y_val;  // 这里应有溢出处理
	return create_intobject(res);
}

/*
	x_val是否大于y_val
*/
int int_op_compare(IntObject x, IntObject y)
{
	int x_val = x.get_value();
	int y_val = y.get_value();
	int res = (x_val > y_val) ? 1 : (x_val < y_val) ? -1 : 0;
	return res;
}

IntObject* int_op_div(IntObject x, IntObject y)
{
	int x_val = x.get_value();
	int y_val = y.get_value();
	if (0 == y_val)
	{
		cout << "除数不能为0" << endl;
		return NULL;
	}
	int res = x_val / y_val;
	return create_intobject(res);
}

// 取模运算 x_val % y_val
IntObject* int_op_mod(IntObject x, IntObject y)
{
	int x_val = x.get_value();
	int y_val = y.get_value();
	int res = x_val % y_val;
	return create_intobject(res);
}

// 按位与运算
IntObject* int_op_and(IntObject x, IntObject y)
{
	int x_val = x.get_value();
	int y_val = y.get_value();
	int res = x_val & y_val;
	return create_intobject(res);
}

// 按位或运算
IntObject* int_op_or(IntObject x, IntObject y)
{
	int x_val = x.get_value();
	int y_val = y.get_value();
	int res = x_val | y_val;
	return create_intobject(res);
}
