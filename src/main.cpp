#include <algorithm>
#include <chrono>
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

int main() {
    auto start = high_resolution_clock::now();
    std::ifstream input("elephant.ts", std::ios::binary);
    std::vector<unsigned char> data(std::istreambuf_iterator<char>(input), {});
    std::cout << "read: " << duration_cast<milliseconds>(high_resolution_clock::now() - start).count() << std::endl;
    
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

    int cnt = 0;
    // seach for the sync byte
    while ((it = std::find(it, data.end(), 'G')) != data.end()) {
        cnt++;
        // parse the packet and get the PID
        auto start = const_cast<unsigned char*const>(&(*it));
        TSPacket packet(start);
        const auto PID = packet.getPID();
        
        std::cout << std::dec << "count: " << cnt << 
            " -> " << PID << " " << packet.getPUSI() << std::endl;
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
            std::cout << "parse PMT" << std::endl;
            std::cout << std::dec << "count: " << cnt << std::endl;
            found_pmts[PID] = true;
            ProgramMappingTable PMT(packet.getPUSI());
            PMT.parse(packet.getPayloadStart());
            const auto es = PMT.getElemStreamPIDS();

            video_pid = es.find((unsigned int)0x1b)->second;
            foundVideo = true;
            audio_pid = es.find((unsigned int)0x0f)->second;
            foundAudio = true;

            std::cout << "video: " << video_pid << std::endl
                    << "audio: " << audio_pid << std::endl;
        }

    
        
        //std::cout << "offset: " << std::dec << (packet.getPayloadStart() - start) << std::endl;
        //std::cout << "payload length: " << std::dec << packet.getPayloadSize() << std::endl;
        if (PID == video_pid && foundVideo) {
            //std::cout << std::dec << "PUSI: " << packet.getPUSI() << std::endl;
            //std::cout << std::dec << "count: " << cnt << std::endl;
            if (packet.getPUSI()) {
                std::cout << "video pusi" << std::endl;
                PES pes(packet.getPayloadStart());
                const unsigned int diff = pes.getPacketStart() - packet.getPayloadStart();
                std::cout << std::dec << "packet: " << pes.getPacketLength() << " " << diff <<  std::endl;
                video_file.appendChunk(pes.getPacketStart(), packet.getPayloadSize() - pes.getPacketLength());
            } else {
                video_file.appendChunk(packet.getPayloadStart(), packet.getPayloadSize());
            }
            
            //video_file.appendChunk(packet.getPayloadStart(), packet.getPayloadSize());
        }

        if (PID == audio_pid && foundAudio) {
            //PES pes(packet.getPayloadStart());
            if (packet.getPUSI()) {
                std::cout << "audio pusi" << std::endl;
                PES pes(packet.getPayloadStart());
                const unsigned int diff = pes.getPacketStart() - packet.getPayloadStart();
                std::cout << std::dec << diff << std::endl;
                audio_file.appendChunk(pes.getPacketStart(), packet.getPayloadSize() - pes.getPacketLength());
            } else {
                audio_file.appendChunk(packet.getPayloadStart(), packet.getPayloadSize());
            }
        }
        // jump to end of this packet
        it += 184;
    }
}
