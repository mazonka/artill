all: a.exe

a.exe: a.cpp b.dll
	g++ a.cpp -L./ -lb

b.o: b.c
	gcc -c b.c

b.dll: b.o
	gcc -shared -o b.dll b.o


clean:
	rm b.o b.dll a.exe
