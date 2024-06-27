CC = g++
WEBCC = emcc
CFLAGS = -Wall
BUILD_DIR = ./build
SRC_DIR = ./src
TEST_DIR = ./tests

all: asm

asm: 
	$(CC) $(CFLAGS) $(SRC_DIR)/main.cpp $(SRC_DIR)/asm.cpp $(SRC_DIR)/analyze.cpp $(SRC_DIR)/parser.cpp  $(SRC_DIR)/parameterParser.cpp  $(SRC_DIR)/apis/native.cpp $(SRC_DIR)/apis/wasm.cpp $(SRC_DIR)/util.cpp $(SRC_DIR)/preproc.cpp -o $(BUILD_DIR)/asm

asmweb: 
	$(WEBCC) $(CFLAGS) $(SRC_DIR)/main.cpp $(SRC_DIR)/asm.cpp $(SRC_DIR)/analyze.cpp $(SRC_DIR)/parser.cpp  $(SRC_DIR)/parameterParser.cpp  $(SRC_DIR)/apis/native.cpp $(SRC_DIR)/apis/wasm.cpp $(SRC_DIR)/util.cpp $(SRC_DIR)/preproc.cpp -sMODULARIZE -s 'EXPORT_NAME="createAssembler"' -s NO_DISABLE_EXCEPTION_CATCHING=1 -s EXPORTED_RUNTIME_METHODS="['UTF8ToString', 'stringToUTF8', 'allocateUTF8OnStack']" -std=c++1z -s NO_EXIT_RUNTIME=1 --pre-js $(SRC_DIR)/wasm_pre.js -o $(BUILD_DIR)/asm_web.js
	cp $(BUILD_DIR)/asm_web.js ../frontend/www/scripts/asm_web.js
	cp $(BUILD_DIR)/asm_web.wasm ../frontend/www/scripts/asm_web.wasm

clean:
	rm -f $(BUILD_DIR)/vm $(BUILD_DIR)/tests/test_instructions
