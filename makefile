CC = g++
CFLAGS = -g -c -Wall -std=c++11 -O0
OBJ = ConditionCodes.o Memory.o RegisterFile.o Tools.o Loader.o \
yess.o PipeReg.o PipeRegField.o W.o F.o D.o M.o E.o FetchStage.o MemoryStage.o WritebackStage.o DecodeStage.o ExecuteStage.o Simulate.o

.C.o:
	$(CC) $(CFLAGS) $< -o $@

yess: $(OBJ)
	$(CC) $(OBJ) -o yess

clean:
	rm -f $(OBJ) yess

removeR:
	sed -i -e 's/\r$$//' *