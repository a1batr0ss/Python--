# include "base_object.h"
# include "stack.h"

void LinkStack::push(BaseObject *p)
{
	PNODE _ = new NODE;
	_->data = p;
	_->p_next = top;
	top = _;

	return;
}

void LinkStack::pop(void)
{
	// 在POP() 时, 可能由于没有找到变量值, 而为空, 所以要处理
	if (NULL == top)
	{
		return;
	}
	PNODE _ = top;
	top = top->p_next;
	delete _;

	return;
}

BaseObject* LinkStack::get_top(void)
{
	return top->data;
}

LinkStack::~LinkStack(void)
{
	while (NULL != top)
	{
		PNODE _ = top->p_next;
		delete top;
		top = _;
	}
}
