#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>

#include "../lib/transport-stream-packet.h"
#include "../lib/program-association-table.h"
#include "../lib/program-mapping-table.h"
#include "../lib/payload-file-writer.h"


int main() {
    std::ifstream input("elephant.ts", std::ios::binary);
    std::vector<unsigned char> data(std::istreambuf_iterator<char>(input), {});
    
    bool foundPAT = false;
    bool foundPMT = false;
    unsigned int video_pid; 
    PayloadFileWriter video_file("raw_video.dump");
    PayloadFileWriter audio_file("raw_audio.dump");
    unsigned int audio_pid;
    std::map<unsigned int, bool> found_pmts;
    std::vector<unsigned char>::iterator it = data.begin();

    // seach for the sync byte
    while ((it = std::find(it, data.end(), 'G')) != data.end()) {
        // parse the packet and get the PID
        auto start = const_cast<unsigned char*const>(&(*it));
        TSPacket packet(start);
        const auto PID = packet.getPID();
        
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
            continue;
        }


        if (PID == video_pid) {

            continue;
        }

        if (PID == audio_pid) {

            continue;
        }

        // jump to end of this packet
        it += 184;
    }
}
