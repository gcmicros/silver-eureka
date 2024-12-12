#include <iostream> 

#include "program-association-table.h"

std::map<unsigned int, unsigned int> ProgramAssociationTable::getPrograms() {
    return programs;
}

ProgramAssociationTable::ProgramAssociationTable(bool payloadUnitStartIndicator) 
: ProgramSpecificInformation(payloadUnitStartIndicator) {
}

void ProgramAssociationTable::parse(const unsigned char* const start) {
 
    ProgramSpecificInformation::parse(start);
    ProgramSpecificInformation::print();
    const auto payload_start = ProgramSpecificInformation::getPayloadStart();
    for(int i = 0; i < getPayloadLength() ; i+=4) {
        const auto program_num = payload_start[i] << 8 | payload_start[i+1];
        const auto program_map_PID = (payload_start[i+2] & 0x1F) << 8 | payload_start[i+3];
        std::cout << std::hex << "program_num: " << program_num
            << " PID: " << program_map_PID << std::endl;
        this->programs.insert({program_num, program_map_PID});
    }
}
