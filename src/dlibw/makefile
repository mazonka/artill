all: dlib/optimization.h

dlib/optimization.h:
	bzip2 -d -k ../dlib.fcl.bz2
	chmod 0777 ../dlib.fcl
	fcl3 extr ../dlib.fcl
	rm -f ../dlib.fcl
#	mv dlib lib

test: test.cpp
	cl -Ox -EHsc test.cpp

test2: test2.cpp
	cl -Ox -EHsc test2.cpp

clean:
	rm -rf dlib test.dat test.obj test.exe test2.obj test2.exe


