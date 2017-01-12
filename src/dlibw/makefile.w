
test: test.cpp dlibadw.lib
	cl -Ox -EHsc test.cpp /link dlibadw.lib

test2: test2.cpp
	cl -Ox -EHsc -WX -W4 test2.cpp


dlibadw.lib: dlibad.cpp dlibad.h
	cl -EHsc -Ox -c dlibad.cpp
	lib dlibad.obj /OUT:dlibadw.lib


