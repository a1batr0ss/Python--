# include <iostream>
# include "string_object.h"

using namespace std;

StringObject* create_stringobject(string value)
{
	StringObject *_ = new StringObject(value);
	if (NULL == _)
	{
		exit(-1);
	}
	return _;
}

StringObject* connect(StringObject x, StringObject y)
{
	string x_val = x.get_value();
	string y_val = y.get_value();
	string res = x_val + y_val;
	return create_stringobject(res);
}

char get_position_char(StringObject x, int pos)
{
	/*
	 *  pos从0开始
	 */
	string x_val = x.get_value();
	int pos_max = x_val.length() - 1;

	if ((pos < 0) || (pos > pos_max))
	{
		return 0;
	}
	return x_val.at(pos);
}

int get_substring_position(StringObject x, string ch)
{
	string x_val = x.get_value();
	int res_max = x_val.length() - 1;
	int res = x_val.find(ch);
	if (res > (res_max))
	{
		return -1;  // 没有查找到
	}
	return res;
}
