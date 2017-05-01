

ifeq ($(OS),Windows_NT)
PLAT=w
else
PLAT=u
endif

ALGL = ../alglib$(PLAT)/lib/
ALIB = $(ALGL)/a.lib
DLIL = ../dlad/
DLIB = $(DLIL)dlibad$(PLAT).lib

ifeq ($(PLAT),w)
CL = cl
O = o_msc
OEXT = .obj
OPT=-WX -W4 -nologo -EHsc -Ox -D_CRT_SECURE_NO_DEPRECATE=1 /wd4355 /wd4127 /wd4702 /wd4100 /wd4290 -D_USING_V110_SDK71_=1

# unref var
#OPT+= /wd4189

OOUT=-Fo
EOUT=-Fe
ARCR=lib /OUT:
endif

ifeq ($(PLAT),u)
CL = g++
O = o_unx
OEXT = .o
OPT=-Wall -O2 -std=c++11 -fno-strict-aliasing -ffloat-store -pthread
ifeq ($(OS),Windows_NT)
OPT=-Wall -O2 -std=c++11 -fno-strict-aliasing -ffloat-store -lpthread
endif
OOUT=-o 
EOUT=-o 
ARCR=ar crs #
endif

OPT += -I $(ALGL) -I $(DLIL)

EEXT = .$(PLAT).exe

src = asolver.cpp asdlib.cpp func.cpp
obj := $(src:%.cpp=$O/%$(OEXT))

##srctrg = drag.cpp
##trg := $(srctrg:%.cpp=%$(EEXT))

all: $O frep$(EEXT)

$O:
	mkdir $O

##drag$(EEXT): %$(EEXT):%.cpp $O/my.lib
##	$(CL) -Ox -EHsc $< $O/my.lib -Fe$@

#drag$(EEXT): $O/drag$(OEXT) $O/my.lib *.h $(ALIB) $(DLIB)
#	$(CL) $(OPT) $< $O/my.lib $(EOUT)$@ $(ALIB) $(DLIB)

frep$(EEXT): $O/frep$(OEXT) $O/my.lib cpp/*.h $(DLIB)
	$(CL) $(OPT) $< $O/my.lib $(EOUT)$@ $(DLIB)

$O/frep$(OEXT): cpp/frep.cpp
	$(CL) -c $(OPT) $< $(OOUT)$@

#$(obj): $O/%$(OEXT):%.cpp *.h $(ALGL)
$(obj): $O/%$(OEXT):cpp/%.cpp cpp/*.h
	$(CL) -c $(OPT) $< $(OOUT)$@

$O/my.lib: $(obj)
	$(ARCR)$O/my.lib $(obj)

$(ALGL):
	@echo "***************************************"
	@echo "** WARNING: BUILD $(ALGL) "
	@echo "***************************************"
	false

clean:
	rm -rf $O *.exe *.obj *.o o_* out/*.dat
	rm -f tests/*_o.dat
