SRC=20130523.c command.c sub.c memory.c opcode.c assemble.c
EXE=20130523.out

all : $(EXE)

$(EXE) : $(SRC)
	gcc $(SRC) -o $(EXE) -Wall

clean :
	rm $(EXE) rm
