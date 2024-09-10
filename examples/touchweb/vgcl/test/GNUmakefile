
CXX=g++ -g -pg
INC=-I../include -I../include/vgcl -I../src/win32

LIBS = ../libvgcl.a -lgtest

all: app.exe unit_test.exe

unit_src = \
		unit_test/test_all.cpp \
		unit_test/test_vstring.cpp \
		unit_test/test_vthread.cpp

unit_test.exe : ${unit_src}
	${CXX} ${unit_src} ${LIBS} -o $@ ${INC}


%.exe : %.cpp
	${CXX} $< ${LIBS} -o $@ ${INC} 

clean:
	rm *.o *.exe *.out
