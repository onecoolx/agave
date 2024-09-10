CXX=g++ 
CXXFLAGS=-Wall -g -pg -fno-rtti -fno-exceptions
INC=-I./src/ps -I./include -I./include/vgcl -I./src -I./src/win32

VPATH=./src/ps ./src/win32 ./include ./include/vgcl ./src

objects=\
		vapplication.o \
		vgraphic.o \
		vthread.o \
		\
		vplatform.o

all: libvgcl.a

libvgcl.a : ${objects}
	ar rcu $@ ${objects}

%.o : %.cpp 
	${CXX} ${CXXFLAGS} -c $<  -o $@ ${INC}

clean:
	rm *.o *.a
