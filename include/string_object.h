# ifndef STRING_OBJECT_H
# define STRING_OBJECT_H

# include <string>
# include <regex>
# include "base_object.h"
# include "gc_object.h"

extern TypeObject StringType;

class StringObject : public BaseObject
{
private:
	string value;

public:
	StringObject(string value_)
	{
		value_ = process_string(value_);

		set_type(&StringType);
		value = value_;
		change_ref(INCREASE);
		 
		hash<string> h;
		set_hash_value(h(value_));
		set_object_size(sizeof(StringObject));
	}

	static StringObject* get_stringobject(MemoryPool *pool, string value)
	{
		char *mem = pool->get_memory(sizeof(StringObject));
		StringObject *obj = new (mem)StringObject(value);
		return obj;
	}

	/*
	 * 处理\n
	 */
	string process_string(string value_)
	{
		regex reg0("\\\\\\\\n");
		regex reg1("\\\\n");
		regex reg2("#");
		string res0 = regex_replace(value_, reg0, "#");
		string res1 = regex_replace(res0, reg1, "\n");
		string res2 = regex_replace(res1, reg2, "\\n");

		return res2;
	}

	void set_value(string value_)
	{
		value = value_;
	}

	string get_value(void)
	{
		return value;
	}
};

StringObject* connect(StringObject, StringObject);
char get_position_char(StringObject, int);
int get_substring_position(StringObject, string);

# endif
