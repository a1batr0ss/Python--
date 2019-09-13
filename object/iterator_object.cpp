# include "iterator_object.h"
# include "array_object.h"

BaseObject* iter_find(BaseObject *seq)
{
/*
 *     BaseObject *iter = seq->get_type()->get_iter();  // 这里应该只能存在一个iterator
 *     // 应该先判断seq是否是数组或字典
 *     if (iter == NULL)
 *     {
 *         iter = new IteratorObject(seq);
 *         seq->get_type()->set_iter(iter);
 *     }
 * 
 *     ((IteratorObject*)iter)->set_index(0);  // 这里仅限于for全遍历使用, 其他情况会有问题
 *     ((IteratorObject*)iter)->set_seq(seq);
 */
	IteratorObject *iter = new IteratorObject(seq);
	
	iter->set_index(0);

	return iter;
}

BaseObject* iter_next(BaseObject *i)
{
	IteratorObject *iter = (IteratorObject*)i;
	BaseObject *seq = iter->get_seq();
	int index = iter->get_index();
	// seq 可能是array, 也可能是dict
	// 先只处理array
	ArrayObject *arr = (ArrayObject*)seq;
	// 从array中取出元素
	BaseObject *e = get_elem(arr, index);
	iter->inc_index();  // index会一直递增下去
	return e;
}
