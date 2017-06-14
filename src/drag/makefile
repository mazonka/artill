
# to build o_lib
# use: make MAKELIB=1

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
OPT=-WX -W4 -nologo -EHsc -Ox -D_CRT_SECURE_NO_DEPRECATE=1 /wd4355 /wd4127 /wd4702 /wd4100 /wd4290 /wd4267 -D_USING_V110_SDK71_=1

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

##srctrg = draglib.cpp
##trg := $(srctrg:%.cpp=%$(EEXT))

ifdef MAKELIB

OPT += -D main=libmain
O = o_lib
all: $O $O/drag$(OEXT) $O/draglib.lib

else

all: $O drag$(EEXT)
endif


src = dsolver.cpp dataset.cpp psolver.cpp rstester.cpp \
	rsolver.cpp trres.cpp rnd.cpp cd.cpp traj.cpp \
	asolver.cpp physics.cpp timer_$(PLAT).cpp \
	util.cpp progr.cpp psic.cpp fpoint.cpp names.cpp \
	asdlib.cpp explore.cpp binom.cpp vstrmem.cpp
obj := $(src:%.cpp=$O/%$(OEXT))



$O:
	mkdir $O

##drag$(EEXT): %$(EEXT):%.cpp $O/draglib.lib
##	$(CL) -Ox -EHsc $< $O/draglib.lib -Fe$@

#drag$(EEXT): $O/drag$(OEXT) $O/draglib.lib *.h $(ALIB) $(DLIB)
#	$(CL) $(OPT) $< $O/draglib.lib $(EOUT)$@ $(ALIB) $(DLIB)

drag$(EEXT): $O/drag$(OEXT) $O/draglib.lib cpp/*.h $(DLIB)
	$(CL) $(OPT) $< $O/draglib.lib $(EOUT)$@ $(DLIB)

$O/drag$(OEXT): cpp/drag.cpp
	$(CL) -c $(OPT) $< $(OOUT)$@

#$O/drag_libmain$(OEXT): cpp/drag.cpp
#	$(CL) -c $(OPT) -D main=libmain $< $(OOUT)$@

#$(obj): $O/%$(OEXT):%.cpp *.h $(ALGL)
$(obj): $O/%$(OEXT):cpp/%.cpp cpp/*.h
	$(CL) -c $(OPT) $< $(OOUT)$@

$O/draglib.lib: $(obj)
	$(ARCR)$O/draglib.lib $(obj)

$(ALGL):
	@echo "***************************************"
	@echo "** WARNING: BUILD $(ALGL) "
	@echo "***************************************"
	false

clean:
	rm -rf $O *.exe *.obj *.o o_* out/*.dat
