all: dlib/optimization.h dlibad.lib

dlib/optimization.h:
	bzip2 -d -k ../dlib.fcl.bz2
	chmod 0777 ../dlib.fcl
	fcl3 extr ../dlib.fcl
	rm -f ../dlib.fcl
#	mv dlib lib

test: test.cpp dlibad.lib
	cl -Ox -EHsc test.cpp /link dlibad.lib

test2: test2.cpp
	cl -Ox -EHsc -WX -W4 test2.cpp

clean:
	rm -rf dlib test.dat *.exe *.obj *.lib


dlibad.lib: dlibad.cpp dlibad.h
	cl -EHsc -Ox -c dlibad.cpp
	lib dlibad.obj /OUT:dlibad.lib


