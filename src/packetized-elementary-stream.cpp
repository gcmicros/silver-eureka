#include <iostream>

#include "../lib/packetized-elementary-stream.h"

PES::PES(const unsigned char* const start) {
    this->start = start;
    parse();
}

void PES::parse(){
    const unsigned int start_code_prefix = start[2];
    if (start_code_prefix != 1) {
        std::cout << "start code prefix: " << std::dec << start_code_prefix << std::endl;
    }
    const unsigned int index = 4;
    const unsigned int packet_length = start[index] << 8 | start[index+1];
    //std::cout << std::dec << "packet length: " << packet_length << std::endl;
    const int pes_header_length = start[index+4];
    std::cout << std::dec << "packet header length " <<  pes_header_length << std::endl;

    // NOTE(gmicros): +5 was the bug
    this->packet_length =  index + 5 + pes_header_length;
    this->start = start + index + 5 + pes_header_length;
}

unsigned int PES::getPacketLength() {
    return packet_length;
}

const unsigned char* const PES::getPacketStart() {
    // TODO: move this to the data start
    return this->start;
}