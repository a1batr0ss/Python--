# ifndef COMP_TOOLS_H
# define COMP_TOOLS_H

# include <string>
# include "stack.h"
# include "array_object.h"
# include "code_object.h"

# define GREATER_ 1
# define LESS_ 2
# define DOUBLE_EQUALS 3
# define GREATER_EQUALS 4
# define LESS_EQUALS 5


# define PUSH(x) runtime_stack->push(x);

// int seperate_string2elem(char*, LinkStack*, ArrayObject*, char);
int seperate_string2elem(char*, char);
int seperate_string2elem_push_stack(char*, LinkStack*, ArrayObject*, char);
int seperate_string2elem_dict(char*, LinkStack*, ArrayObject*);
int get_key_value_num(char*, char);
int get_compare_type(string);
int get_file_lines(char*);
void set_arg_break(CodeObject*, int);

# endif
