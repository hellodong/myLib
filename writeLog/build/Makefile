EXE=logTest
SRCPATH=../src/
CC=gcc
OBJECTS=main.o writeLog.o
all:${EXE}

${EXE}:${OBJECTS}
	${CC} -o $@ $^
${OBJECTS}:%.o:${SRCPATH}%.c
	${CC} -c $?

.PHONY:clean

clean:
	rm -rf ${EXE} ${OBJECTS}
	

