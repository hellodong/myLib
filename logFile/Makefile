EXE=logTest
CC=gcc
CFLAGS=-g -Wall
FLAGLD=
SRCPATH=example/
BUILD_DIR=build/
OBJECTS 	=	$(BUILD_DIR)main.o 
OBJECTS    += 	$(BUILD_DIR)logFile.o


all:$(BUILD_DIR) ${EXE}

$(BUILD_DIR):
	mkdir $@

${EXE}:${OBJECTS}
	${CC} -o $@ $^ ${FLAGLD}
${OBJECTS}:${BUILD_DIR}%.o:${SRCPATH}%.c
	${CC} -c ${CFLAGS} $? -o $@

.PHONY:clean

clean:
	rm -rf ${EXE} ${OBJECTS} test.txt
	

