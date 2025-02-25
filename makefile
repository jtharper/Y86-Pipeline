CC = g++
CFLAGS = -g -c -Wall -std=c++11 -O0
OBJ = lab5.o Loader.o Memory.o Tools.o RegisterFile.o \
 ConditionCodes.o 

.C.o:
	$(CC) $(CFLAGS) $< -o $@

lab5: $(OBJ)

clean:
	rm $(OBJ) lab5

run:
	make clean
	make lab5
	./run.sh

removeR:
	sed -i -e 's/\r$$//' *

