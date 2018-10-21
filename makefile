EXECNAME = bin/compiler.exe
ifeq ($(OS), Windows_NT)
	EXEC = $(EXECNAME)
else
	EXEC = ./$(EXECNAME)
endif

SRCF = $(wildcard src/*.cpp)
OBJF = $(patsubst src/%.cpp, obj/%.o, $(SRCF))

GDB = -g -ggdb
# CFLAGS = -std=c++11 -Wall -ansi
GLOBALFLAGS = -std=c++11 $(GDB)

LFLAGS = -Wall $(GLOBALFLAGS)
CFLAGS = -Wall $(GLOBALFLAGS)
LFLAGS = -Wall $(GLOBALFLAGS)


OP     = o

# Call make run mod=yes to compile a modularized file, and link it
IF1   = fat_mod_A
IF2   = fat_mod_B
IDIR = ./res

OF   = bin_res
ODIR = ./res


ifeq ($(mod), no)
PROG_INPUT = $(IDIR)/$(IF1)
else
PROG_INPUT = $(IDIR)/$(IF1)
PROG_INPUT = $(IDIR)/$(IF1) $(IDIR)/$(IF2) 
endif


PROG_OUTPUT = $(ODIR)/$(OF)

PROG_ARGS   = -$(OP) $(PROG_INPUT)

ifeq ($(clean), no)
all: build run	
else
all: clean build run
endif

run:
	$(EXEC) $(PROG_ARGS)

build: $(OBJF)
	g++ $^ -o $(EXEC)


debug:
	gdb $(EXEC) -- $(PROG_ARGS)

obj/%.o: 
	g++ $(patsubst obj/%.o, src/%.cpp, $@) $(CFLAGS) -c -o $@ 

ifeq ($(OS), Windows_NT)
clean:
	rm -f obj/*.o
# clean:
# 	rd /s /q "obj"
# 	mkdir "obj"

clearscreen:
	cls
else
clean:
	rm -f obj/*.o

clearscreen:
	clear
endif
run:

slide:
	./bin/compiler.exe -o ./res/slide1 ./res/slide2
	

