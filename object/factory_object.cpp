# include <iostream>
# include "factory_object.h"
# include "base_object.h"
# include "int_object.h"
# include "string_object.h"
# include <string>
# include "gc_object.h"

using namespace std;

IntObject* FactoryObject::create_object(MemoryPool *pool, int value)
{
	return IntObject::get_intobject(pool, value);
}

StringObject* FactoryObject::create_object(MemoryPool *pool, string value)
{
	return StringObject::get_stringobject(pool, value);
}

