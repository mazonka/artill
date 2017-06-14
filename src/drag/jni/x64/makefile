
all: test64cl.exe apiclass.exe apiclass.dll

test64cl.exe: test64cl.cpp
	cl /nologo /E test64cl.cpp

apiclass.exe: apiclass.lib main.cpp apiclass.obj utils.obj
	cl /O2 -EHsc main.cpp apiclass.obj apiclass.lib utils.obj -Feapiclass.exe

apiclass.lib: apiclass.dll
	@echo dll

apiclass.dll: apiclass.obj apiclass_iface.obj utils.obj
	link /DLL apiclass.obj apiclass_iface.obj utils.obj

apiclass_iface.obj: apiclass_iface.cpp apiclass_iface.h
	cl /O2 -c -EHsc apiclass_iface.cpp

apiclass.obj: apiclass.cpp apiclass.h utils.h
	cl /O2 -c -EHsc apiclass.cpp

utils.obj: utils.cpp utils.h
	cl /O2 -c -EHsc utils.cpp

clean:
	rm -rf *.exe *.obj *.exp *.lib *.dll out*.gs *.h apicl*.cpp main.cpp utils.cpp


#src = hq_sl_hdt.cpp hq_sl_meta.cpp hq_sl_file.cpp hq_dbslice.cpp hq_dbindex.cpp
#obj := $(src:%.cpp=$O/%$(OEXT))

#$(obj): $O/%$(OEXT):%.cpp *.h *.inc
#	$(COMPILER) -c $(OPT) $(INC) $(FLAGS) $< $(OOUT)$@


fileh = apiclass.h utils.h apiclass_iface.h
filec = apiclass.cpp apiclass_iface.cpp utils.cpp main.cpp

$(fileh): %.h:../src/%.h
	cp $< $@
	chmod 0777 $@

$(filec): %.cpp:../src/%.cpp
	cp $< $@
	chmod 0777 $@

