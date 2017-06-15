DLIL = ../../../dlad/
DLIB = $(DLIL)dlibadu.lib
DOBJ = ../../o_libu/

#fPIC?
OPT=-Wall -O2 -std=c++11 -fno-strict-aliasing -ffloat-store -pthread
ifeq ($(OS),Windows_NT)
OPT=-Wall -O2 -std=c++11 -fno-strict-aliasing -ffloat-store -lpthread
endif

all: workclass_s.exe workclass_d.exe wc_thin_imp.so

workclass_d.exe: wc_thin_imp.lib main.cpp workclass_dyn.o utils.o
	g++ $(OPT) main.cpp workclass_dyn.o wc_thin_imp.lib utils.o -Feworkclass_d.exe

workclass_s.exe: workclass_imp.lib main.cpp wc_thin_api.h
	g++ $(OPT) main.cpp workclass_imp.lib -o workclass_s.exe

workclass_imp.lib: $(DOBJ) workclass_imp.o $(DOBJ)draglib.lib \
	$(DOBJ)drag.o $(DLIB) utils.o
	ar crs workclass_imp.lib workclass_imp.o $(DOBJ)drag.o \
	$(DOBJ)draglib.lib $(DLIB) utils.o

wc_thin_imp.lib: wc_thin_imp.dll
	@echo dll

wc_thin_imp.dll: workclass_imp.o wc_thin_imp.o utils.o $(DOBJ) \
	$(DOBJ)drag.o $(DOBJ)draglib.lib $(DLIB)
	link /DLL workclass_imp.o wc_thin_imp.o utils.o \
	$(DOBJ)drag.o $(DOBJ)draglib.lib $(DLIB) /OUT:wc_thin_imp.dll

wc_thin_imp.o: wc_thin_imp.cpp wc_thin_api.h
	g++ -c $(OPT) wc_thin_imp.cpp

#workclass.o: workclass.cpp workclass.h utils.h
#	g++ /O2 -c -EHsc workclass.cpp

workclass_imp.o: workclass_imp.cpp workclass_api.h utils.h
	g++ -c $(OPT) workclass_imp.cpp

workclass_dyn.o: workclass_dyn.cpp workclass_api.h utils.h wc_thin_api.h
	g++ -c $(OPT) workclass_dyn.cpp

utils.o: utils.cpp utils.h
	g++ -c $(OPT) utils.cpp

$(DOBJ):
	@echo "=================================="
	@echo build library first
	@echo in drag directory: make MAKELIB=1
	@echo "=================================="

clean:
	rm -rf *.exe *.o *.exp *.lib *.dll out*.gs *.h \
	work*.cpp wc_*.cpp main.cpp utils.cpp *.log


#src = hq_sl_hdt.cpp hq_sl_meta.cpp hq_sl_file.cpp hq_dbslice.cpp hq_dbindex.cpp
#obj := $(src:%.cpp=$O/%$(OEXT))

#$(obj): $O/%$(OEXT):%.cpp *.h *.inc
#	$(COMPILER) -c $(OPT) $(INC) $(FLAGS) $< $(OOUT)$@


fileh = wc_thin_api.h utils.h workclass_api.h
filec = workclass_imp.cpp workclass_dyn.cpp utils.cpp main.cpp wc_thin_imp.cpp

$(fileh): %.h:../src/%.h
	cp $< $@
	chmod 0777 $@

$(filec): %.cpp:../src/%.cpp
	cp $< $@
	chmod 0777 $@

