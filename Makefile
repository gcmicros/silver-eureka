BUILD_DIR=./build

main: $(BUILD_DIR) 
	g++ main.cpp transport-stream-packet.cpp program-specific-information.cpp program-association-table.cpp program-mapping-table.cpp -o $(BUILD_DIR)/ts-parser

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
