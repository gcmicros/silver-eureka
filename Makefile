BUILD_DIR=./build

SRC=main.cpp\
	transport-stream-packet.cpp\
	program-specific-information.cpp\
	program-association-table.cpp\
	program-mapping-table.cpp

main: $(BUILD_DIR) 
	g++ $(SRC) -o $(BUILD_DIR)/ts-parser

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
