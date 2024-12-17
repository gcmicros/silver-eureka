#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>

#include "../lib/transport-stream-packet.h"
#include "../lib/packetized-elementary-stream.h"
#include "../lib/program-association-table.h"
#include "../lib/program-mapping-table.h"
#include "../lib/payload-file-writer.h"

using namespace std::chrono;

int main(int argc, char* argv[]) {
    // Check if the filename was provided
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::string filename = argv[1];
    if (!std::filesystem::exists(filename)) {
        std::cerr << "File [" << filename << "] does not exist" << std::endl;
    }

    std::ifstream input(filename, std::ios::binary);
    std::vector<unsigned char> data(std::istreambuf_iterator<char>(input), {});
    
    bool foundPAT = false;
    bool foundPMT = false;
    bool foundVideo = false;
    bool foundAudio = false;
    unsigned int video_pid = 99; 
    PayloadFileWriter video_file("raw_video.dump");
    PayloadFileWriter audio_file("raw_audio.dump");
    unsigned int audio_pid = 99;
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
            PAT.parse(packet.getPayloadStart());
            // found the PAT
            foundPAT = true;
            const auto programs = PAT.getPrograms();
            for (auto it = programs.begin(); it != programs.end(); it++){
                found_pmts.insert({it->second, false});        
            }
        }

        const auto foundPMT = found_pmts.find(PID);
        if (foundPMT != found_pmts.end() && !(foundPMT->second)) {
            // parse this specific PMT
            found_pmts[PID] = true;
            ProgramMappingTable PMT(packet.getPUSI());
            PMT.parse(packet.getPayloadStart());
            const auto es = PMT.getElemStreamPIDS();

            // TODO(gmicros): fix magic numbers
            video_pid = es.find((unsigned int)0x1b)->second;
            foundVideo = true;
            audio_pid = es.find((unsigned int)0x0f)->second;
            foundAudio = true;

            std::cout << "video: " << video_pid << std::endl
                    << "audio: " << audio_pid << std::endl;
        }
        
        if (PID == video_pid && foundVideo) {
            if (packet.getPUSI()) {
                PES pes(packet.getPayloadStart());
                video_file.appendChunk(pes.getPacketStart(), packet.getPayloadSize() - pes.getPacketLength());
            } else {
                video_file.appendChunk(packet.getPayloadStart(), packet.getPayloadSize());
            }
        }

        if (PID == audio_pid && foundAudio) {
            if (packet.getPUSI()) {
                PES pes(packet.getPayloadStart());
                audio_file.appendChunk(pes.getPacketStart(), packet.getPayloadSize() - pes.getPacketLength());
            } else {
                audio_file.appendChunk(packet.getPayloadStart(), packet.getPayloadSize());
            }
        }
        // jump to end of this packet
        it += 188;
    }
}