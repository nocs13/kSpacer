OS = $(shell uname -s)
RM =

CC = g++ 
DEFS = -DLINUX
FLGS = -ansi -std=c++98 
DIRS =	-I./  -I../kgmEngine -L./	
LIBS = -lpthread -lXxf86vm -lGL -lGLU -lopenal
OUT  = kSpacer

ifeq ($(OS), Linux)
	DEFS = -DLINUX -DOAL -DGPUSHADERS
	FLGS = -fPIC -O0 -fpermissive  -ansi
	LIBS = -lpthread -lXxf86vm -lX11 -lGL -lGLU -lopenal
	OUT  = kSpacer
	RM   = rm -f
else
	DEFS = -DWIN32 -DTEST -DGPUSHADERS -DD3DS
	FLGS = -O0 -mwindows
	LIBS = -lpthread -lopengl32 -luser32 -lkernel32 -lgdi32 -lglu32 -ldsound
	OUT  = kSpacer.exe
	RM   = rm -f
endif

all: debug


debug: set_debug $(OUT)
	echo 'end debug'

set_debug:
	$(eval DEFS += -DDEBUG)
	$(eval FLGS += -g)

release: set_release $(OUT)
	echo 'end release'

set_release:
	$(eval DEFS += -DRELEASE)

$(OUT): libkgmEngine.a
	echo "start kSpacer building..."
	$(CC) -g -c *.cpp $(DEFS) $(FLGS) $(DIRS) $(LIBS)
	$(CC) -o kSpacer *.o ../kgmEngine/libkgmEngine.a $(DEFS) $(FLGS) $(DIRS) $(LIBS)
	

clean:	
	$(RM) kSpacer

distclean: clean
	$(RM) kSpacer
run: 
	./kSpacer

libkgmEngine.a:
	make -C ../kgmEngine/
