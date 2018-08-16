EXE=logTest
CC=gcc
SRCPATH=src/
BUILD_DIR=build/
OBJECTS 	=	$(BUILD_DIR)main.o 
OBJECTS    += 	$(BUILD_DIR)writeLog.o

all:$(BUILD_DIR) ${EXE}

$(BUILD_DIR):
	mkdir $@

${EXE}:${OBJECTS}
	${CC} -o $@ $^
${OBJECTS}:${BUILD_DIR}%.o:${SRCPATH}%.c
	${CC} -c $? -o $@

.PHONY:clean

clean:
	rm -rf ${EXE} ${OBJECTS}
	

