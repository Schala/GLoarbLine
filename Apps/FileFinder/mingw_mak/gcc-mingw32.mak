
CFLAGS=-O2 -Wno-deprecated -Wno-ctor-dtor-privacy 
LFLAGS=

all:  FileFinder_gcc.exe

clean:
	rm *.o *.lib *.exe

INCDIR= -I ../../../Core/Includes \
		-I ../../../GUI/Includes \
		-I ../../../File/Includes \
		-I ../../../Codecs/Includes \
		-I ../../_base/Sources \
		-I ../../../../Libs/regexpp2/include

HEADER=-include ../../../Targets/Includes/TargetAPI_Win32.h

ALL_HEADER= ../../../Targets/Includes/TargetAPI_Win32.h \
	../../_base/Sources/CreateSkin.h

ALL_OBJ= CreateSkin.o \
		resources.o \
		FileFinder.o

CreateSkin.o: ../../_base/Sources/CreateSkin.cpp $(ALL_HEADER)
	g++ -o $@ -c $(INCDIR) $(HEADER) $(CFLAGS) $<

FileFinder.o: ../Sources/FileFinder.cpp $(ALL_HEADER)
	g++ -o $@ -c $(INCDIR) $(HEADER) $(CFLAGS) $<

resources.o: ../Resources/App.rc
	windres --include-dir=../Resources -o $@ $<

FileFinder_gcc.exe: $(ALL_OBJ)
	g++ $(ALL_OBJ) \
		--nostdlib -L /mingw/lib -lmingw32 \
		../../../GUI/mingw_mak/XSP_GUI.lib \
		../../../Codecs/mingw_mak/XSP_Codecs.lib \
		../../../../Libs/LIBPNG/libpng.a \
		../../../../Libs/ZLIB/libz.a \
		../../../File/mingw_mak/XSP_File.lib \
		../../../Core/mingw_mak/XSP_Core.lib \
		-lkernel32 -luser32 -lgdi32 \
		-lshell32 -ladvapi32 -lmsimg32 -lole32 -luuid \
		-mwindows -o $@
