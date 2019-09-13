# ifndef FILE_OBJECT_H
# define FILE_OBJECT_H

# include <fstream>
# include <string>
# include "base_object.h"

extern TypeObject FileType;

class FileObject : public BaseObject
{
	private:
	 	fstream *f;
		string name;
		int mode;   // read:1, write: 2, read+write: 3

	public:
		FileObject(string file_name)
		{
			name = file_name;
			mode = 1;  // 默认只读的方式打开文件
			f = NULL;
			set_type(&FileType);
		}

		FileObject(string file_name, int mode_)
		{
			name = file_name;
			mode = mode_;
			f = NULL;
			set_type(&FileType);
		}

		fstream* get_file(void)
		{
			return f;
		}

		string get_file_name(void)
		{
			return name;
		}

		int get_mode(void)
		{
			return mode;
		}

		void set_file(fstream *f_)
		{
			f = f_;
		}
};

void open_file(FileObject*);
BaseObject* close_file(BaseObject*);
BaseObject* write_file(BaseObject*, BaseObject*);
BaseObject* read_file_line(BaseObject*);
void filetype_init(void);

# endif

