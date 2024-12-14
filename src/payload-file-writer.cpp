#include "../lib/payload-file-writer.h"

PayloadFileWriter::PayloadFileWriter(std::string filename){
    output_file.open(filename, std::fstream::out | std::ios::binary);
}

PayloadFileWriter::~PayloadFileWriter(){
    output_file.close();
}

void PayloadFileWriter::appendChunk(const unsigned char *const start, unsigned int length) {
    output_file.write(reinterpret_cast<const char*>(start), length);    
}
