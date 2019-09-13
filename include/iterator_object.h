# ifndef ITERATOR_OBJECT_H
# define ITERATOR_OBJECT_H

# include "base_object.h"

class IteratorObject : public BaseObject
{
private:
	int index;
	BaseObject *seq;

public:
	IteratorObject(void)
	{
		index = 0;
		seq = NULL;
		change_ref(INCREASE);
	}

	IteratorObject(BaseObject *seq_)
	{
		index = 0;
		// 应该先检查seq_的类型
		seq = seq_;
		// set_type()
		change_ref(INCREASE);
	}

	int get_index(void)
	{
		return index;
	}

	void set_index(int index_)
	{
		index = index_;
	}

	void inc_index(void)
	{
		index++;
	}

	void dec_index(void)
	{
		index--;
	}

	BaseObject* get_seq(void)
	{
		return seq;
	}

	void set_seq(BaseObject *seq_)
	{
		seq = seq_;
	}
};

BaseObject* iter_next(BaseObject*);
BaseObject* iter_find(BaseObject*);

# endif
