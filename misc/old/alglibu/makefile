all: lib/a.lib

lib/a.lib: lib
	cd lib && make -f ../lib.mak

lib:
	bzip2 -d -k ../alglib.fcl.bz2
	chmod 0777 ../alglib.fcl
	fcl3 extr ../alglib.fcl
	rm -f ../alglib.fcl
	mv alglib lib

clean:
	rm -rf lib test.dat test.obj test.exe


