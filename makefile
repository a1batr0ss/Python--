OBJECT = ./object
OBJ_O = ./obj_o
INCLUDE = ./include
TARGET_DIR = ./

OBJECT_SRC = $(wildcard ${OBJECT}/*.cpp)
OBJECT_TARGET = $(patsubst %.cpp, ${OBJ_O}/%.o, $(notdir ${OBJECT_SRC}))

TARGET_FILE = main
BIN_TARGET = ${TARGET_DIR}/${TARGET_FILE}

CC = g++
CCFLAGS = -I ${INCLUDE}

all: exec $(BIN_TARGET) 

exec:
	 lex ./compile/lex.l
	 yacc -d ./compile/yacc.y && mv ./lex.yy.c ./object/lex.yy.cpp && mv ./y.tab.c ./object/y.tab.cpp && mv ./y.tab.h ./include/y.tab.h	

${BIN_TARGET} : ${OBJECT_TARGET}
	$(CC) -o $@ ${OBJECT_TARGET}

${OBJ_O}/%.o : ${OBJECT}/%.cpp 
	 $(CC) $(CCFLAGS) -o $@ -c $<

clean:
	find $(OBJ_O) -name *.o -exec rm -rf {} \;
	rm -rf $(BIN_TARGET)

