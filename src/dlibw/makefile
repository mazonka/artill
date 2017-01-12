
ifeq ($(OS),Windows_NT)
PLAT=w
else
PLAT=u
endif

all: dlib/optimization.h dlibad$(PLAT).lib

#dlib/optimization.h:
#	bzip2 -d -k ../dlib.fcl.bz2
#	chmod 0777 ../dlib.fcl
#	fcl3 extr ../dlib.fcl
#	rm -f ../dlib.fcl

dlib/optimization.h:
	bzip2 -d -k ../dlib.tar.bz2
	chmod 0777 ../dlib.fcl
	tar xvf ../dlib.tar
	rm -f ../dlib.tar

clean:
	rm -rf dlib test.dat *.exe *.obj *.lib *.o


ifeq ($(PLAT),w)
CL = cl
O = o_msc
OEXT = .obj
OPT=-WX -W4 -nologo -EHsc -Ox -D_CRT_SECURE_NO_DEPRECATE=1 /wd4355 /wd4127 /wd4702 /wd4100 /wd4290 -D_USING_V110_SDK71_=1
OOUT=-Fo
EOUT=-Fe
ARCR=lib /OUT:
endif

ifeq ($(PLAT),u)
CL = g++
O = o_unx
OEXT = .o
OPT=-Wall -O2 -std=c++11 -fno-strict-aliasing -ffloat-store
OOUT=-o 
EOUT=-o 
ARCR=ar crs #
endif

include makefile.$(PLAT)

