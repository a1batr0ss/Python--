# include <iostream>
# include <string>
# include <fstream>
# include "base_object.h"
# include "file_object.h"
# include "array_object.h"

using namespace std;

extern TypeObject FileType;

void filetype_init(void)
{
	// 方法名中不能有下划线(编译规则)
	FileType.set_method("readLine", pack_func((FUNC_POINTER)read_file_line, 1, 0));
	FileType.set_method("close", pack_func((FUNC_POINTER)close_file, 0, 0));
	FileType.set_method("write", pack_func((FUNC_POINTER)write_file, 0, 1));
}

/*
 * 以下对文件的操作暂不处理异常
 */

void open_file(FileObject *f_obj)
{
	string name = f_obj->get_file_name();
	int mode = f_obj->get_mode();
	fstream *f = new fstream();
	switch (mode)
	{
		case 1:
			f->open(name, ios::in);
			break;
		case 2:
			f->open(name, ios::out);
			break;
		case 3:
			// cout << "mode is 3" << endl;
			f->open(name, ios::in|ios::out);
			/*
			 * if ((*f))
			 * {
			 *     // (*f) << "Welcome to C plus plus";
			 *     // f->close();
			 *     cout << "打开成功" << endl;
			 * }
			 * else
			 * {
			 *     cout << "打开失败" << endl;
			 * }
			 */
			break;
		default:
			f = NULL;
			break;
	}
	f_obj->set_file(f);
}

BaseObject* close_file(BaseObject *f)
{
	/*
	 * 关闭文件, 但是FileObject对象暂时不处理
	 */
	// cout << "In close_file" << endl;
	FileObject *f_obj = (FileObject*)f;
	fstream *fp = f_obj->get_file();
	// cout << "fp is " << fp << endl;
	fp->close();
	// cout << "successfully close" << endl;
	return NULL;
}

BaseObject* write_file(BaseObject *f, BaseObject* contents)
{
	FileObject *f_obj = (FileObject*)f;
	string cont = ((StringObject*)contents)->get_value();
	fstream *fp = f_obj->get_file();
	(*fp) << cont;
	// cout << "In WriteFile" << cont << endl;
	// fp->close();
	return NULL;
}

BaseObject* read_file_line(BaseObject *f)
{
	/*
	 * 按行读取
	 */
	cout << "In read_file_line" << endl;
	FileObject *f_obj = (FileObject*)f;
	string res;
	fstream *fp = f_obj->get_file();
	getline(*fp, res);
	cout << res << endl;
	StringObject *str = new StringObject(res);
	return str;
}

