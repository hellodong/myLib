CC:=gcc
CFLAGS:= -g -I./src
BUILD_DIR=build
SOURCE_DIR=example

BIN=test

all:$(BUILD_DIR) ${BUILD_DIR}/$(BIN)

$(BUILD_DIR):
	mkdir $@

${BUILD_DIR}/$(BIN):$(BUILD_DIR)/main.o
	$(CC) $^ -o $@ 

$(BUILD_DIR)/main.o:$(SOURCE_DIR)/main.c
	$(CC) $< $(CFLAGS) -c -o $@ 

.PHONY:clean

clean:
	rm $(BUILD_DIR)  -rf
