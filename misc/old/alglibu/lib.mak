all: a.lib

ap.o: *.cpp *.h
	g++ -O2 -c *.cpp

a.lib: ap.o
	ar crs a.lib *.o


	