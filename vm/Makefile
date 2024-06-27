CC = gcc
WEBCC = emcc
CFLAGS = -Wall
BUILD_DIR = ./build
SRC_DIR = ./src
TEST_DIR = ./tests

all: vm

vm: $(SRC_DIR)/main.c $(SRC_DIR)/vm.c
	$(CC) $(CFLAGS) $(SRC_DIR)/main.c $(SRC_DIR)/vm.c $(SRC_DIR)/sheduler.c $(SRC_DIR)/util.c $(SRC_DIR)/api.c -o $(BUILD_DIR)/vm

vmweb: $(SRC_DIR)/main.c $(SRC_DIR)/vm.c
	$(WEBCC) $(CFLAGS) $(SRC_DIR)/main.c $(SRC_DIR)/vm.c $(SRC_DIR)/sheduler.c $(SRC_DIR)/util.c $(SRC_DIR)/api.c $(SRC_DIR)/api_wasm.c -sMODULARIZE -s 'EXPORT_NAME="createVM"' -s EXPORTED_RUNTIME_METHODS="['UTF8ToString', 'allocateUTF8OnStack']" -s NO_EXIT_RUNTIME=1 -o $(BUILD_DIR)/vm_web.js
	cp $(BUILD_DIR)/vm_web.js ../frontend/www/scripts/core.js
	cp $(BUILD_DIR)/vm_web.wasm ../frontend/www/scripts/vm_web.wasm
test_positioning: $(TEST_DIR)/test_positioning $(SRC_DIR)/vm.c
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(TEST_DIR)/test_positioning $(SRC_DIR)/vm.c $(SRC_DIR)/sheduler.c -o $(BUILD_DIR)/tests/test_instructions

test_base64: $(TEST_DIR)/test_base64 $(SRC_DIR)/vm.c
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(TEST_DIR)/test_base64 $(SRC_DIR)/vm.c $(SRC_DIR)/sheduler.c -o $(BUILD_DIR)/tests/test_instructions
clean:
	rm -f $(BUILD_DIR)/vm $(BUILD_DIR)/tests/test_instructions
