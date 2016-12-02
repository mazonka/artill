all: a.lib

a.lib: *.cpp *.h
	cl -EHsc -Ox -c *.cpp
	lib *.obj /OUT:a.lib


	