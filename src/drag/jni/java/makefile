
JDK32=C:\Program Files (x86)\Java\jdk1.6.0_38
JDK64=C:\Program Files\Java\jdk1.8.0_25

ifndef PLAT
PLAT=x64
endif

ifeq ($(PLAT),x32)
JDK=$(JDK32)
else ifeq ($(PLAT),x64)
JDK=$(JDK64)
else
$(error PLAT is not set; must be x32 or x64; e.g. make PLAT=x64)
endif

LIBDIR=../$(PLAT)

all: testJWC.class jWorkClass.dll jar dist_$(PLAT)
	@echo Usage: java testJWC
	@echo Usage: java64 testJWC

dist_$(PLAT): jWorkClass.dll wc_thin_imp.dll jWorkClass.jar wc_thin_imp.$(PLAT)
	mkdir -p dist_$(PLAT)
	cp $^ dist_$(PLAT)/
	chmod 0777 dist_$(PLAT)/*


jar: jWorkClass.jar
jWorkClass.jar: jWorkClass/jWorkClass.class jWorkClass/DragMod.class
	"$(JDK)/bin/jar" -cf jWorkClass.jar $^

#JE: Prototype makeedit to support different kind of testing
unit-test: UnitTestGS.class jWorkClass.dll
    @echo Usage: java -cp ".;junit-4.8.2.jar" UnitTestGS		
UnitTestGS.class: UnitTestGS.java
	"$(JDK)/bin/javac" -classpath ".;junit-4.8.2.jar" $<
		
wc_thin_imp.dll: $(LIBDIR)/wc_thin_imp.dll
	cp $< $@
	chmod 0777 $@
	touch wc_thin_imp.$(PLAT)

testJWC.class: testJWC.java
	"$(JDK)/bin/javac" testJWC.java

jWorkClass.dll: jWorkClass_jWorkClass.cpp jWorkClass_jWorkClass.h $(LIBDIR)/wc_thin_imp.lib wc_thin_imp.dll
	cl /nologo /E test_$(PLAT).cpp
	cl /O2 -I "$(JDK)/include" -I "$(JDK)/include/win32" -I $(LIBDIR) jWorkClass_jWorkClass.cpp \
	$(LIBDIR)/wc_thin_imp.lib -MT -LD -FejWorkClass.dll

jWorkClass_jWorkClass.h: jWorkClass/jWorkClass.class
	"$(JDK)/bin/javah" jWorkClass.jWorkClass

jWorkClass/jWorkClass.class: jWorkClass/jWorkClass.java 
	"$(JDK)/bin/javac" jWorkClass/jWorkClass.java

jWorkClass/DragMod.class: jWorkClass/DragMod.java 
	"$(JDK)/bin/javac" jWorkClass/DragMod.java
	
clean:
	rm -rf *.class *.h *.obj *.exp *.lib *.dll *.log out*.gs jWorkClass/*.class \
	*.jar lib *.x32 *.x64 jWorkClass/Plat.java dist_x32 dist_x64


clear:
	rm -rf *.class *.h *.obj *.exp *.lib *.dll *.log out*.gs jWorkClass/*.class \
	*.jar lib *.x32 *.x64 jWorkClass/Plat.java 
	