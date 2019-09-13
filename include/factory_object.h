# ifndef FACTORY_OBJECT
# define FACTORY_OBJECT

# include "base_object.h"
# include "int_object.h"
# include "string_object.h"
# include <string>
# include "gc_object.h"

using namespace std;

class FactoryObject
{
	public:
		static IntObject* create_object(MemoryPool*, int);
		static StringObject* create_object(MemoryPool*, string);
};

# endif

