#include <iostream>

#include "../lib/program-mapping-table.h"

ProgramMappingTable::ProgramMappingTable(bool payloadUnitStartIndicator) : ProgramSpecificInformation(payloadUnitStartIndicator) {
}

std::map<unsigned int, unsigned int> ProgramMappingTable::getElemStreamPIDS() {
    return PID_to_es_type;
}

void ProgramMappingTable::parse(const unsigned char* const start) {
    ProgramSpecificInformation::parse(start);

    auto payload_start = ProgramSpecificInformation::getPayloadStart();
    const unsigned int program_descriptor_length = (payload_start[2] & 0x03) << 8 | payload_start[3];
    payload_start += 4 + program_descriptor_length;

    int i = 0;
    while(i < getPayloadLength()) {
        const int stream_type = payload_start[i];
        const int es_PID = (payload_start[i + 1] & 0x1F) << 8 | payload_start[i + 2];
        const int es_info_length = (payload_start[i + 3] & 0x03) << 8 | payload_start[i + 4];
        i += 5 + es_info_length;
        PID_to_es_type.insert({stream_type, es_PID});
    } 
}
