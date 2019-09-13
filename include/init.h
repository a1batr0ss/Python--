# ifndef INIT_H
# define INIT_H

# include "base_object.h"
# include "frame_object.h"

void builtin_init();
// void methods_locals();
// BaseObject* methods_print(BaseObject*);
// BaseObject* methods_open_file(BaseObject*, BaseObject*);
void init_compile_environment();
void init_runtime_environment(CodeObject*);
BaseObject* get_frameobject_main();
BaseObject* get_codeobject_main();
MemoryPool* get_consts_pool();
MemoryPool* get_heap_pool();

# endif

