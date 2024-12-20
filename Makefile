BUILD_DIR=./build
SRC_DIR=./src

INPUT_FILE=elephant.ts

SRC=$(SRC_DIR)/main.cpp\
	$(SRC_DIR)/transport-stream-packet.cpp\
	$(SRC_DIR)/program-specific-information.cpp\
	$(SRC_DIR)/program-association-table.cpp\
	$(SRC_DIR)/packetized-elementary-stream.cpp\
	$(SRC_DIR)/payload-file-writer.cpp\
	$(SRC_DIR)/program-mapping-table.cpp

all: $(BUILD_DIR) 
	g++ -std=c++17 $(SRC) -o $(BUILD_DIR)/ts-parser

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

.PHONY: clean \
		run \
        download-file

download-file:
	@if [ ! -f $(INPUT_FILE) ]; then \
		curl "http://test.unified-streaming.com/elephants.ts" --output $(INPUT_FILE); \
	fi

ffmpeg-video-extract:
	ffmpeg -i elephant.ts -c:v copy -f rawvideo -y ffmpeg.video > /dev/null 2>&1

ffmpeg-audio-extract:
	ffmpeg -i elephant.ts -c:a copy -f adts -y ffmpeg.audio > /dev/null 2>&1

ffmpeg: ffmpeg-video-extract ffmpeg-audio-extract

compare-video: ffmpeg-video-extract
	xxd raw_video.dump > myhex && xxd ffmpeg.video > ffmpeghex && diff myhex ffmpeghex


compare-audio: ffmpeg-audio-extract
	xxd raw_audio.dump > myhex_audio && xxd ffmpeg.audio > ffmpeghex_audio && diff myhex_audio ffmpeghex_audio

compare: compare-video compare-audio

test: run compare

run: download-file all
	build/ts-parser $(INPUT_FILE)

clean: 
	rm -rf $(BUILD_DIR)