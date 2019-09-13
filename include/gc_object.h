# ifndef GC_OBJECT_H
# define GC_OBJECT_H

# include "base_object.h"

# define _1MB (1 << 10)

using namespace std;

class MemoryPool
{
	private:
		char *from_start;
		char *to_start;
		char *from_pointer;
		char *to_pointer;
		int proportion;  // 默认1/1
		int total_size;  // 默认2M
		int from_region_surplus;  // 当该区域不足时, 进行gc
		int to_region_surplus;  // 当该区域为不足时, 扩大堆
		BaseObject *roots;
		int extend_coefficient;  // pool_size ==> pool_size * extend_coefficient

	public:
		MemoryPool(int proportion_=1, int total_size_=2)
		{
			proportion = proportion_;
			total_size = total_size_ * _1MB;
			from_pointer = NULL;
			to_pointer = NULL;
			from_start = NULL;
			to_start = NULL;
			from_region_surplus = 0;
			to_region_surplus = 0;
			roots = NULL;
			extend_coefficient = 2;
		}
		char* get_memory(int);
		char* get_from_start()
		{
			return from_start;
		}
		char* get_to_start()
		{
			return to_start;
		}
		void swap_from_to()
		{
			char *_ = from_start;
			from_start = to_start;
			to_start = _;

			from_pointer = to_pointer;
			to_pointer = to_start;
		}
		char* new_malloc(int);
		void init_pool();
		void gc_copy();  // 复制算法
		BaseObject* copy_object(BaseObject*);  // 复制对象
		int copy_data(char*, BaseObject*);
		void get_roots(BaseObject*);
		void extend_pool();
		void reset_gc_tag();
		BaseObject* get_children(BaseObject*);
};

# endif

