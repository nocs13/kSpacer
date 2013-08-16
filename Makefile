OS = $(shell uname -s)
RM =

CC = g++ 
DEFS = -DLINUX
FLGS = -ansi -std=c++98 
DIRS =	-I./  -I../kgmEngine -L./	
LIBS = -lpthread -lXxf86vm -lGL -lGLU -lopenal

ifeq ($(OS), Linux)
	DEFS = -DLINUX -DDEBUG -DTEST -DOAL -DGPUSHADERS
    FLGS = -g -fPIC -O1 -fpermissive  -ansi
	LIBS = -lpthread -lXxf86vm -lX11 -lGL -lGLU -lopenal
	RM = rm -f
else
	DEFS = -DWIN32 -DDEBUG -DTEST -DGPUSHADERS
    FLGS = -g -O0 -mwindows
    LIBS = -lpthread -lopengl32 -luser32 -lkernel32 -lgdi32 -lglu32
	RM = rm -f
endif

all:
	$(CC) -g -c *.cpp $(DEFS) $(FLGS) $(DIRS) $(LIBS)
	$(CC) -o kSpacer *.o ../kgmEngine/libkgmEngine.a $(DEFS) $(FLGS) $(DIRS) $(LIBS)

clean:	
	$(RM) kSpacer *.o
	
run: 
	./kSpacer