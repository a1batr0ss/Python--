# ifndef STACK_H
# define STACK_H

# include "base_object.h"

typedef struct l_node
{
	BaseObject *data;
	struct l_node *p_next;
} NODE, *PNODE;

class LinkStack
{
private:
	PNODE top;

public:
	LinkStack(void)
	{
		top = NULL;
	}

	bool is_empty(void)
	{
		return (NULL == top);
	}

	void push(BaseObject *p);
	void pop(void);
	BaseObject* get_top(void);
	~LinkStack(void);
};

# endif
