#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>

#include "transport-stream-packet.h"
#include "program-association-table.h"
#include "program-mapping-table.h"

std::map<int, int> PAT; 

// keep track of all the packets are they come in
std::map<unsigned int, std::vector<unsigned char*>> packets;

void parsePMT(std::vector<unsigned char>::iterator payload_start) {
//    const unsigned int program_info_length = (payload_start[2] & 0x03) << 8 | payload_start[3];
    const unsigned int stream_type = payload_start[0];
    const unsigned int es_pid = (payload_start[1] & 0b11111) << 8 | payload_start[2];
}

void parsePAT(std::vector<unsigned char>::iterator payload_start) {
    // TODO(gmicros): what can go wrong here? 
    //
    
    const unsigned int descriptor_tag = payload_start[0];
    const unsigned int section_length = payload_start[1];
    std::cout << "section_length: " << section_length << std::endl;
    payload_start = payload_start + 9;
    while (*payload_start != 0xFF) {
        const unsigned int program_num = payload_start[0] << 8 | payload_start[1];
        const unsigned int program_map_pid = (payload_start[2] & 0x1F) << 8 | (payload_start[3] & 0xFF);
        std::cout << std::hex << program_num << " -> " << std::hex << program_map_pid << std::endl;
        PAT.insert({program_num, program_map_pid});
        
        payload_start += 4;
        return;
    }
    // there's a CRC at the end here
}

int main() {
    std::ifstream input("elephant.ts", std::ios::binary);
    std::vector<unsigned char> data(std::istreambuf_iterator<char>(input), {});

    
    // jump to sync byte for each packet
    bool foundPAT = false;
    bool foundPMT = false;
    std::map<unsigned int, bool> found_pmts;
    std::vector<unsigned char>::iterator it = data.begin();
    while ((it = std::find(it, data.end(), 'G')) != data.end()) {
        // parse the packet and get the PID
        auto start = const_cast<unsigned char*const>(&(*it));
        TSPacket packet(start);
        const auto PID = packet.getPID();
        // based on the PID parse
        //std::cout << std::hex << "PID: 0x" << PID << std::endl;
        // case PID
        //packet.print();
        
        // only look for PAT once
        if (PID == 0x00 && !foundPAT) {
            ProgramAssociationTable PAT(packet.getPUSI());
            PAT.parse(start+4);
            // found the PAT
            foundPAT = true;
            const auto programs = PAT.getPrograms();
            for (auto it = programs.begin(); it != programs.end(); it++){
                found_pmts.insert({it->second, false});        
            }
            continue;
        }

        const auto foundPMT = found_pmts.find(PID);
        if (foundPMT != found_pmts.end() && !(foundPMT->second)) {
            // parse this specific PMT
            std::cout << "parse PMT" << std::endl;
            found_pmts[PID] = true;
            ProgramMappingTable PMT(packet.getPUSI());
            PMT.parse(start+4);
        }


        it += 184;
        //return 0;
    }


    //for (const auto& [key, value] : packets) {
    //    std::cout << std::hex << key << std::dec << " size: " << value.size() << std::endl;
    //}
    //std::cout << "map size: " << packets.size() << std::endl;
}
