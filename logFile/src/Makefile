

CC=gcc
CFLAGS=-g -O0 -Wall
LDFLAGS=-lpthread
BIN=log
BUILD_DIR=build
all:${BUILD_DIR} ${BUILD_DIR}/${BIN} 

${BUILD_DIR}:
	@mkdir $@

${BUILD_DIR}/${BIN}:${BUILD_DIR}/main.o ${BUILD_DIR}/log_file.o
	${CC} $^ -o $@ ${LDFLAGS}

${BUILD_DIR}/main.o:main.c
	${CC} $< ${CFLAGS} -c -o $@ 

${BUILD_DIR}/log_file.o:log_file.c
	${CC} $< ${CFLAGS} -c -o $@

phony:clean run

clean:
	rm ${BUILD_DIR} -rf

run:
	./${BUILD_DIR}/${BIN}
