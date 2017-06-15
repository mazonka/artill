DLIL = ../../../dlad/
DLIB = $(DLIL)dlibadw.lib
DOBJ = ../../o_lib/

all: test64cl.exe workclass_s.exe workclass_d.exe wc_thin_imp.dll 

test64cl.exe: test64cl.cpp
	@cl /nologo /E test64cl.cpp > test64cl.ok
	@rm -f test64cl.ok
	@echo test x64 ok

workclass_d.exe: wc_thin_imp.lib main.cpp workclass_dyn.obj utils.obj
	cl /O2 -EHsc main.cpp workclass_dyn.obj wc_thin_imp.lib utils.obj -Feworkclass_d.exe

workclass_s.exe: workclass_imp.lib main.cpp wc_thin_api.h
	cl /O2 -EHsc main.cpp workclass_imp.lib -Feworkclass_s.exe

workclass_imp.lib: $(DOBJ) workclass_imp.obj $(DOBJ)draglib.lib \
	$(DOBJ)drag.obj $(DLIB) utils.obj
	lib workclass_imp.obj $(DOBJ)drag.obj \
	$(DOBJ)draglib.lib $(DLIB) utils.obj /OUT:workclass_imp.lib

wc_thin_imp.lib: wc_thin_imp.dll
	@echo dll

wc_thin_imp.dll: workclass_imp.obj wc_thin_imp.obj utils.obj $(DOBJ) \
	$(DOBJ)drag.obj $(DOBJ)draglib.lib $(DLIB)
	link /DLL workclass_imp.obj wc_thin_imp.obj utils.obj \
	$(DOBJ)drag.obj $(DOBJ)draglib.lib $(DLIB) /OUT:wc_thin_imp.dll

wc_thin_imp.obj: wc_thin_imp.cpp wc_thin_api.h
	cl /O2 -c -EHsc wc_thin_imp.cpp

#workclass.obj: workclass.cpp workclass.h utils.h
#	cl /O2 -c -EHsc workclass.cpp

workclass_imp.obj: workclass_imp.cpp workclass_api.h utils.h
	cl /O2 -c -EHsc workclass_imp.cpp

workclass_dyn.obj: workclass_dyn.cpp workclass_api.h utils.h wc_thin_api.h
	cl /O2 -c -EHsc workclass_dyn.cpp

utils.obj: utils.cpp utils.h
	cl /O2 -c -EHsc utils.cpp

$(DOBJ):
	@echo "=================================="
	@echo build library first
	@echo in drag directory: make MAKELIB=1
	@echo "=================================="

clean:
	rm -rf *.exe *.obj *.exp *.lib *.dll out*.gs *.h \
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

