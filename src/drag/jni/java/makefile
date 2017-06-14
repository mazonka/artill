
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

all: testApiCl.class JavaApiCl.dll jar dist_$(PLAT)
	@echo Usage: java testApiCl
	@echo Usage: java64 testApiCl

dist_$(PLAT): JavaApiCl.dll apiclass.dll JavaApiCl.jar apiclass.$(PLAT)
	mkdir -p dist_$(PLAT)
	cp $^ dist_$(PLAT)/
	chmod 0777 dist_$(PLAT)/*


jar: JavaApiCl.jar
JavaApiCl.jar: JavaApiCl/JavaApiCl.class
	"$(JDK)/bin/jar" -cf JavaApiCl.jar $^

#JE: Prototype makeedit to support different kind of testing
unit-test: UnitTestGS.class JavaApiCl.dll
    @echo Usage: java -cp ".;junit-4.8.2.jar" UnitTestGS		
UnitTestGS.class: UnitTestGS.java
	"$(JDK)/bin/javac" -classpath ".;junit-4.8.2.jar" $<
		
apiclass.dll: $(LIBDIR)/apiclass.dll
	cp $< $@
	chmod 0777 $@
	touch apiclass.$(PLAT)

testApiCl.class: testApiCl.java
	"$(JDK)/bin/javac" testApiCl.java

JavaApiCl.dll: JavaApiCl_JavaApiCl.cpp JavaApiCl_JavaApiCl.h $(LIBDIR)/apiclass.lib apiclass.dll
	cl /nologo /E test_$(PLAT).cpp
	cl /O2 -I "$(JDK)/include" -I "$(JDK)/include/win32" -I $(LIBDIR) JavaApiCl_JavaApiCl.cpp \
	$(LIBDIR)/apiclass.lib -MT -LD -FeJavaApiCl.dll

JavaApiCl_JavaApiCl.h: JavaApiCl/JavaApiCl.class
	"$(JDK)/bin/javah" JavaApiCl.JavaApiCl

JavaApiCl/JavaApiCl.class: JavaApiCl/JavaApiCl.java 
	"$(JDK)/bin/javac" JavaApiCl/JavaApiCl.java

	
clean:
	rm -rf *.class *.h *.obj *.exp *.lib *.dll *.log out*.gs JavaApiCl/*.class \
	*.jar lib *.x32 *.x64 JavaApiCl/Plat.java dist_x32 dist_x64


clear:
	rm -rf *.class *.h *.obj *.exp *.lib *.dll *.log out*.gs JavaApiCl/*.class \
	*.jar lib *.x32 *.x64 JavaApiCl/Plat.java 
	