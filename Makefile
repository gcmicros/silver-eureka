BUILD_DIR=./build
SRC_DIR=./src

SRC=$(SRC_DIR)/main.cpp\
	$(SRC_DIR)/transport-stream-packet.cpp\
	$(SRC_DIR)/program-specific-information.cpp\
	$(SRC_DIR)/program-association-table.cpp\
	$(SRC_DIR)/program-mapping-table.cpp

main: $(BUILD_DIR) 
	g++ $(SRC) -o $(BUILD_DIR)/ts-parser

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)