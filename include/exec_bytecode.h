# ifndef EXEC_BYTECODE_H
# define EXEC_BYTECODE_H

# include "base_object.h"
# include "code_object.h"
# include "frame_object.h"

# define PUSH(x) runtime_stack->push(x);
# define POP() runtime_stack->pop();
# define GET_TOP() runtime_stack->get_top();
# define GET_ITEM_DICT(w, x) get_value_dict(w, x);

# define VAR_NAMES co->get_names()
# define VAR_VALUES co->get_values()

# define GET_VAR_NAME 1
# define GET_VAR_VALUE 2
# define STORE_LOCAL 3
# define GET_VAR_LOCAL 4
# define PRINT_VALUE 5
# define POP_TOP 6
# define BUILD_ARRAY 7
# define BUILD_DICT 8
# define GET_ATTR 9
# define GET_ITER 10
# define FOR_ITER 11
# define JUMP_CODE 12
# define CALL_FUNCTION_POINTER 13
# define CALL_FUNCTION 14
# define JUMP_CODE_ABSOLUTE 17
# define GET_ELEM 18
# define COMPARE_OP 19
# define JUMP_IF_FALSE 20
# define NUMBER_ADD 21
# define NUMBER_MOD 22
# define OPEN_FILE 23
# define GET_FROM_LOCAL_2 24
# define GET_FROM_LOCAL 25
# define SEPERATE_STRING 26
# define SWAP_TOP_TWO 27
# define BUILD_FUNCTION 28
# define BREAK_LOOP 29
# define CONTINUE_LOOP 30
# define NUMBER_MINUS 31
# define NUMBER_TIMES 32
# define NUMBER_DIV 33
# define DEC_SELF 34
# define INC_SELF 35
# define RETURN_VALUE 36

BaseObject* execute_byte_code(FrameObject*);

# endif
