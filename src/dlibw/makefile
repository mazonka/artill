
ifeq ($(OS),Windows_NT)
PLAT=w
else
PLAT=u
endif

all: dlib/optimization.h dlibad$(PLAT).lib

dlib/optimization.h: dlib.tar
	tar xvf dlib.tar > /dev/null
	touch dlib/optimization.h

dlib.tar:
	bzip2 -d -k ../dlib.tar.bz2
	mv ../$@ $@
	chmod 0777 $@

clean:
	rm -rf dlib test.dat *.exe *.obj *.lib *.o dlib.tar


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

$(info PLAT=$(PLAT))

include makefile$(PLAT)

