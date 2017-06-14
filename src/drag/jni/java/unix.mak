
JAVA_HOME=C:\Program Files\Java\jdk1.8.0_25

PLAT=unx
LIBDIR=../$(PLAT)

all: testHeComb.class libJavaHeComb.so jar dist_$(PLAT)
	@echo Usage: LD_LIBRARY_PATH=. java testHeComb

dist_$(PLAT): libJavaHeComb.so libhecombiner.so JavaHeComb.jar hecombiner.$(PLAT)
	mkdir -p dist_$(PLAT)
	cp $^ dist_$(PLAT)/
	chmod 0777 dist_$(PLAT)/*

jar: JavaHeComb.jar
JavaHeComb.jar: JavaHeComb/JavaHeComb.class
	jar -cf JavaHeComb.jar $^
	
#JE: Prototype makeedit to support different kind of testing
unit-test: UnitTestGS.class libJavaHeComb.so
    @echo Usage: java -cp ".;junit-4.8.2.jar" UnitTestGS		
UnitTestGS.class: UnitTestGS.java
	javac -classpath ".;junit-4.8.2.jar" $<
		
libhecombiner.so: $(LIBDIR)/libhecombiner.so
	cp $< $@
	chmod 0777 $@
	touch hecombiner.$(PLAT)

testHeComb.class: testHeComb.java
	javac testHeComb.java
	
libJavaHeComb.so: JavaHeComb_JavaHeComb.o libhecombiner.so
	g++ JavaHeComb_JavaHeComb.o	libhecombiner.so -shared -Wl,-rpath,'$$ORIGIN' -o libJavaHeComb.so

JavaHeComb_JavaHeComb.o: JavaHeComb_JavaHeComb.cpp JavaHeComb_JavaHeComb.h
	g++ -std=c++11 -c -O2 -fPIC -I "$(JAVA_HOME)/include" \
	-I "$(JAVA_HOME)/include/linux" -I "$(JAVA_HOME)/include/win32" \
    -I $(LIBDIR) JavaHeComb_JavaHeComb.cpp

JavaHeComb_JavaHeComb.h: JavaHeComb/JavaHeComb.class
	javah JavaHeComb.JavaHeComb

JavaHeComb/JavaHeComb.class: JavaHeComb/JavaHeComb.java 
	javac JavaHeComb/JavaHeComb.java

	
clean:
	rm -rf *.class *.h *.obj *.exp *.lib *.dll *.log out*.gs JavaHeComb/*.class \
	*.jar lib *.unx *.x32 *.x64 JavaHeComb/Plat.java dist_x32 dist_x64 dist_unx


clear:
	rm -rf *.class *.h *.obj *.exp *.lib *.so *.log out*.gs JavaHeComb/*.class \
	*.jar lib *.unx *.x32 *.x64 JavaHeComb/Plat.java 
	