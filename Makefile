TARGET = Binary_Tree

CC = g++
FLAG = -o

all : $(TARGET)

Binary_Tree : Binary_Tree.cpp
	$(CC) $(FLAG) Binary_Tree Binary_Tree.cpp

clean :
	rm *~ $(TARGET)