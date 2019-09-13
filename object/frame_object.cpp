# include <iostream>
# include "frame_object.h"
# include "exec_bytecode.h"
# include "dict_object.h"
# include "array_object.h"
# include "code_object.h"
# include "string_object.h"
# include "stack.h"
# include "iterator_object.h"
# include "function_object.h"
# include "comp_tools.h"
# include "bool_object.h"
# include "file_object.h"

# define JUMP_RELATIVE(x) p_first += x;

extern IntObject Bool_True;
extern IntObject Bool_False;

// static变量初始化
BaseObject* FrameObject::builtin = new DictObject;

void print_locals(FrameObject *f)
{
	BaseObject *l = f->get_locals();
	l->get_type()->print_value(l);
	return;
}

BaseObject* FrameObject::execute_byte_code()
{
	CodeObject *co = (CodeObject*)this->code;
	DictObject *locals_dict = (DictObject*)this->locals;
	DictObject *globals_dict = (DictObject*)this->globals;
	DictObject *builtin_dict = (DictObject*)this->builtin;
	ArrayObject *names = co->get_names();
	ArrayObject *values = co->get_values();
	char *codes = co->get_code();

	// 以下查看所有字节码
	// cout << "\nbyte_code " << co->get_name() << endl;
	// int k = 0;
	// for (; codes[k]!=-1; k++)
	// {
	// 	// cout << (int)codes[k] << endl;
	// 	cout << (int)codes[k] <<  " ";
	// }
	// cout << "\nend byte_code" << endl;  // 字节码没有问题
	// locals->get_type()->print_value(locals);

	int pointer_code = co->get_code_pointer();
	char *p_first = codes + pointer_code;
	char *p_arg = NULL;

	for (;;)
	{
		p_arg = p_first;

		if (-1 == (*p_first))
			break;

		switch (*p_first)
		{
			case GET_VAR_NAME:
			{
				/*
				 * 从names表中获取数据
				 */
				p_arg = p_first + 1;
				co->increase_code_pointer();
				int arg = (int)(*p_arg);

				BaseObject *u = get_elem(VAR_NAMES, arg);  
				// u->get_type()->print_value(u);
				PUSH(u)

				break;
			}
			case GET_VAR_VALUE:
			{
				/*
				 * 从values表中获取数据
				 */
				p_arg = p_first + 1;  // 同上,这里也不能写成char *p_arg
				int arg = (int)(*p_arg);
				co->increase_code_pointer();

				BaseObject *v = get_elem(VAR_VALUES, arg);

				PUSH(v)
				break;
			}
			case STORE_LOCAL:
			{
				/*
				 * 把栈中的两个元素存入locals
				 * 先压变量值,再压变量名
				 * 由于调整了var与right的次序, 先压变量名, 再压变量值
				 */
				BaseObject *y = GET_TOP()
				POP()
				BaseObject *x = GET_TOP()
				POP()

				if (NULL == y)
				{
					break;
				}
				insert_pair_dict(locals_dict, x, y);
				break;
			}
			case GET_VAR_LOCAL:
			{
				/*
				 * 把栈中的变量名取出,进行压栈操作
				 * 所以要提前进行压栈
				 * 从names表中把变量名取出
				 */
				p_arg = p_first + 1;
				co->increase_code_pointer();
				int arg = *p_arg;
				BaseObject *u = get_elem(VAR_NAMES, arg);
				BaseObject *v = NULL;
				// u->get_type()->print_value(u);
				v = GET_ITEM_DICT(locals_dict, u)
				// 先去global, 再builtin查找
				if ((NULL == v) && (NULL != globals_dict))
				{
					v = GET_ITEM_DICT(globals_dict, u)
				}
				if (NULL == v)
				{
					v = GET_ITEM_DICT(builtin_dict, u)
				}

				if (NULL == v)
				{
					cout << "Couldn't find variables ";
					u->get_type()->print_value(u);  // 如果name没有的话, name没存入names表的
					cout << " in locals." << endl;
					// break;  这里不要break, 即使为NULL, 也要执行下面的压栈
				}
				
				PUSH(v)

				break;
			}
			case PRINT_VALUE:
			{
				BaseObject *v = GET_TOP()
				if (NULL == v)
					break;
				v->get_type()->print_value(v);
				cout << "\n";
				break;
			}
			case POP_TOP:
			{
				POP()
				break;
			}
			case BUILD_ARRAY:
			{
				/*
				 * 这里要处理倒序的问题
				 */
				p_arg = p_first + 1;
				co->increase_code_pointer();
				int arg = *p_arg;
				ArrayObject *u = new ArrayObject(arg);

				for (; (arg--)>0;)
				{
					BaseObject *e = GET_TOP()
					set_item_init(u, arg, e);
					POP()
				}

				PUSH(u)
				break;
			}
			case BUILD_DICT:
			{
				p_arg = p_first + 1;
				co->increase_code_pointer();
				int arg = *p_arg;

				DictObject *u = new DictObject;
				BaseObject *x = NULL;
				BaseObject *y = NULL;
				for (; (arg--)>0;)
				{
					x = GET_TOP()
					POP()
					y = GET_TOP()
					POP()
					insert_pair_dict(u, y, x);
				}

				PUSH(u)
				break;
			}
			case GET_ATTR:
			{
				BaseObject *str = GET_TOP()  // met_name
				POP()
				BaseObject *var = GET_TOP()  // var_value
				if (NULL == var)
				{
					cout << "Couldn't find in locals." << endl;
					PUSH(NULL)
					PUSH(NULL)
					break;
				}
				POP()

				StringObject *met = (StringObject*)str;
				string met_name = met->get_value();

				PACK_P_FUNC *func = var->get_type()->get_method(met_name);
				
				FunctionObject *f_obj = new FunctionObject(func);
				PUSH(var)
				PUSH(f_obj)
			
				break;
			}
			case CALL_FUNCTION_POINTER:
			{
				p_arg = p_first + 1;
				co->increase_code_pointer();
				int arg = *p_arg;

				int _ = arg;
				BaseObject *var_name = GET_TOP()
				POP()
				BaseObject *p_func = GET_TOP()
				POP()

				BaseObject **args = (BaseObject**)malloc(sizeof(BaseObject*) * arg);

				while ((_--) > 0)
				{
					args[_] = GET_TOP()
					POP()
				}

				FunctionObject *func = (FunctionObject*)p_func;
				call_function_pointer(var_name, func, args, arg);

				break;
			}
			case GET_ITER:
			{
				BaseObject *u = GET_TOP()
				POP()

				BaseObject *iter = iter_find(u);
				PUSH(iter)
				break;
			}
			case FOR_ITER:
			{
				// cout << "In FOR_ITER" << endl;
				p_arg = p_first + 1;
				co->increase_code_pointer();
				int arg = *p_arg;
				BaseObject *iter = GET_TOP()  // 第二次时, TOP已经不是iterator了, 要先出栈
				// 对iter进行遍历
				BaseObject *x = iter_next(iter);

				if (NULL != x)
				{
					PUSH(x)
					break;  // 字节码指令要后移
				}
				// 如果NULL为空, 说明迭代结束
				p_first = codes + (arg);
				co->set_code_pointer(arg);  // 设置pointer的位置, 使其继续向下执行
				continue;
			}
			case JUMP_CODE:
			{
				p_arg = p_first + 1;
				co->increase_code_pointer();
				int arg = *p_arg;
				JUMP_RELATIVE(arg)
				continue;
			}
			case JUMP_CODE_ABSOLUTE:
			case BREAK_LOOP:
			case CONTINUE_LOOP:
			{
				p_arg = p_first + 1;
				co->increase_code_pointer();
				int arg = *p_arg;

				p_first = codes + arg;
				co->set_pointer_pos(arg);

				continue;
			}
			case CALL_FUNCTION:
			{
				p_arg = p_first + 1;
				co->increase_code_pointer();
				int arg = *p_arg;

				int _ = arg;
				BaseObject *p_func = GET_TOP()  // FunctionObject
				POP()
				if (NULL == p_func)
				{
					break;
				}

				BaseObject **args = (BaseObject**)malloc(sizeof(BaseObject*) * arg);
				for (int i=0; i<arg; i++)
				{
					args[i] = GET_TOP()
					POP()
				}

				FunctionObject *func = (FunctionObject*)p_func;

				if (NULL == func->get_codeobject())
				{
					BaseObject *ret = call_function_pointer(func, args, arg);
					if (NULL != ret)
					{
						PUSH(ret)
					}
				}
				else
				{
					CodeObject *co = (CodeObject*)(func->get_codeobject());
					CodeObject co_ = *co;
					// co->set_code_pointer(0);  //reset
					BaseObject *globals = func->get_globals();
					DictObject *locals = new DictObject();  // 参数初始化

					// process args
					for (int i=0; i<arg; i++)
					{
						insert_pair_dict(locals, get_elem(co_.get_names(), i), args[arg-i-1]);
					}

					FrameObject *f = new FrameObject(&co_, globals, locals);
					BaseObject *ret = f->execute_byte_code();
					PUSH(ret)
				}

				break;
			}
			case GET_ELEM:
			{
				BaseObject *index = GET_TOP()
				POP()
				BaseObject *var = GET_TOP()
				POP()

				BaseObject *res = (*var->get_type()->get_elem_seq())(var, index);
				if (NULL == res)
				{
					cout << "Out of range." << endl;
				}
				PUSH(res)

				break;
			}
			case COMPARE_OP:
			{
				/**
				 * compare int, int
				 * 这里只处理整数的比较
				 */
				BaseObject *op = GET_TOP()
				POP()
				BaseObject *right = GET_TOP()
				POP()
				BaseObject *left = GET_TOP()
				POP()

				StringObject *op_obj = (StringObject*)op;
				IntObject *right_obj = (IntObject*)right;
				IntObject *left_obj = (IntObject*)left;
				string op_str = op_obj->get_value();
				int left_value = left_obj->get_value();
				int right_value = right_obj->get_value();
				int com_type = get_compare_type(op_str);
				int res = 0;

				switch (com_type)
				{
					case GREATER_:
						res = (left_value > right_value);
						break;
					case LESS_:
						// res = (left < right);
						res = (left_value < right_value);
						break;
					case DOUBLE_EQUALS:
						res = (left_value == right_value);
						break;
					case GREATER_EQUALS:
						res = (left_value >= right_value);
						break;
					case LESS_EQUALS:
						res = (left_value <= right_value);
						break;
					default:
						// 处理复杂情况, 不仅仅是简单的数值比较
						break;
				}
				BoolObject *res_obj = res ? &Bool_True : &Bool_False;
				PUSH(res_obj)

				break;
			}
			case JUMP_IF_FALSE:
			{
				p_arg = p_first + 1;
				co->increase_code_pointer();
				int arg = *p_arg;
				
				BaseObject *res = GET_TOP()

				// if 0:时为True， 所以比较hash_value
				// if ((res == &Bool_False) || (0 == res->get_hash_value()))
				if (res->get_hash_value() == Bool_False.get_hash_value())
				{
					p_first = codes + arg;
					co->set_code_pointer(arg);

					continue;
				}
				break;
			}
			case NUMBER_ADD:
			{
				BaseObject *x = GET_TOP()  //const
				POP()
				BaseObject *y = GET_TOP()  // var_value
				POP()
				if (NULL == y)
				{
					PUSH(NULL)
					break;
				}

				BaseObject *res = int_op_add(*(IntObject*)y, *(IntObject*)x);
				PUSH(res)
				break;
			}
			case NUMBER_MINUS:
			{
				BaseObject *x = GET_TOP()
				POP()
				BaseObject *y = GET_TOP()
				POP()

				BaseObject *res = int_op_minus(*(IntObject*)y, *(IntObject*)x);
				PUSH(res)

				break;
			}
			case NUMBER_TIMES:
			{
				BaseObject *x = GET_TOP()
				POP()
				BaseObject *y = GET_TOP()
				POP()

				BaseObject *res = int_op_times(*(IntObject*)y, *(IntObject*)x);
				PUSH(res)

				break;
			}
			case NUMBER_DIV:
			{
				BaseObject *x = GET_TOP()
				POP()
				BaseObject *y = GET_TOP()
				POP()

				BaseObject *res = int_op_div(*(IntObject*)y, *(IntObject*)x);
				PUSH(res)

				break;
			}
			case NUMBER_MOD:
			{
				BaseObject *x = GET_TOP()  // right
				POP()
				BaseObject *y = GET_TOP()  // left
				// y->get_type()->print_value(y);
				POP()
				if (NULL == y)
				{
					break;
				}
				BaseObject *res = int_op_mod(*(IntObject*)y, *(IntObject*)x);
				PUSH(res)

				break;
			}
			case OPEN_FILE:
			{
				p_arg = p_first + 1;
				co->increase_code_pointer();
				int arg = *p_arg;
				BaseObject *v = get_elem(VAR_VALUES, arg);
				// v->get_type()->print_value(v);
				string file_name = ((StringObject*)v)->get_value();
				// cout << file_name << endl;
				FileObject *fp = new FileObject(file_name, 3);
				open_file(fp);

				PUSH(fp)
				break;
			}
			case GET_FROM_LOCAL_2:
			{
				/*
				 * 与GET_VAR_LOCAL不同, 一个从栈中取出查询, 一个从names表中取出查询
				 * 推荐这个
				 * 把栈中第二个取出, (由于栈是链栈, 所以不方便进行参数操作)
				 */

				BaseObject *u = GET_TOP()  // met_name
				POP()
				BaseObject *v = GET_TOP()  // var_name
				POP()
				BaseObject *w = GET_ITEM_DICT(locals_dict, v)   // value
				
				if (NULL == w)
				{
					cout << "Couldn't find variables ";
					u->get_type()->print_value(u);  // 如果name没有的话, name没存入names表的
					cout << " in locals." << endl;
				}
				PUSH(w)
				PUSH(u)

				break;
			}
			case GET_FROM_LOCAL:
			{
                 p_arg = p_first + 1;
                 co->increase_code_pointer();
                 int arg = *p_arg;
                 int _ = arg;
				 
                 BaseObject **args = (BaseObject**)malloc(sizeof(BaseObject*) * arg);
                 while ((_--) > 0)
                 {
                     args[_] = GET_TOP()
                     POP()
                 }
				 
                 BaseObject *u = GET_TOP()
				 POP()

                 BaseObject *v = GET_ITEM_DICT(locals_dict, u)
                 // PUSH(v)
                 for (_=1; _<arg; _++)
                 {
                     PUSH(args[_]);
                 }
				 PUSH(v)
				 if (arg > 0)
				 {
				 	PUSH(args[0])
				 }

				break;
			}
			case SEPERATE_STRING:
			{ 
				p_arg = p_first + 1;
                co->increase_code_pointer();
                int arg = *p_arg;
				StringObject *str = (StringObject*)get_elem(VAR_VALUES, arg);				
				string str_2 = str->get_value();

				VAR_VALUES->get_type()->print_value(VAR_VALUES);

				seperate_string2elem_push_stack((char*)str_2.data(), runtime_stack, values, ',');

				break;
			}
			case SWAP_TOP_TWO:
			{
				BaseObject *x = GET_TOP()
				POP()
				BaseObject *y = GET_TOP()
				POP()
				PUSH(x)
				PUSH(y)
				break;
			}
			case BUILD_FUNCTION:
			{
				p_arg = p_first + 1;
                co->increase_code_pointer();
                int arg = *p_arg;  // 函数参数个数

				BaseObject *o = GET_TOP()
				POP()
				CodeObject *co = (CodeObject*)o;
				FunctionObject *f = new FunctionObject(co->get_name(), co, locals_dict, arg);

				PUSH(f)

				break;
			}
			case INC_SELF:
			{
				BaseObject *v = GET_TOP()
				POP()

				BaseObject *res = int_op_add(*(IntObject*)v, IntObject(1));
				PUSH(res)

				break;
			}
			case DEC_SELF:
			{
				BaseObject *v = GET_TOP()
				POP()

				BaseObject *res = int_op_minus(*(IntObject*)v, IntObject(1));
				PUSH(res)

				break;
			}
			case RETURN_VALUE:
			{
				BaseObject *r = GET_TOP();
				POP()
				return r;
			}
			default:
				;
				break;
		}

		p_first = p_arg + 1;
		co->increase_code_pointer();

		if (-1 == (*p_first))
		{
			break;
		}
	}
}

