#include <iostream>
#include <stdexcept>

#include "../lib/transport-stream-packet.h" 

const unsigned char* const TSPacket::getPayloadStart() {
    return start 
        + 4  // skip TS header
        + this->adaptation_field.getLength(); // skip adaptation field
}
unsigned int TSPacket::getPayloadSize() {
    return this->payload_size;
}

bool TSPacket::getPUSI() {
    return this->payload_unit_start_indicator;
}

unsigned int TSPacket::getPID() {
    return this->PID;
}
void TSPacket::parsePacket(const unsigned char *const packetStart) {
    if (packetStart[0] != 0x47) {
        throw std::runtime_error("Sync byte missing");
    }

    this->transport_error_indicator = packetStart[1] & 0x80;
    this->payload_unit_start_indicator = packetStart[1] & 0x40;
    this->transport_priority = packetStart[1] & 0x20;
    this->PID = (packetStart[1] & 0x1F) << 8 | packetStart[2];
    this->transport_scrambling_control = (packetStart[3] & 0xc0) >> 6;
    this->adaptation_field_control = (packetStart[3] & 0x30) >> 4;
    this->continuity_counter = packetStart[3] & 0x0F;


    if (this->adaptation_field_control == 0b10 || this->adaptation_field_control == 0b11) {
        // adaption field is present
        // TODO: calculate new payload size 
        this->adaptation_field.parse(packetStart+ 4);
        this->payload_size -= this->adaptation_field.getLength();
    }

    if (this->adaptation_field_control == 0b01 || this->adaptation_field_control == 0b11) {
        // payload data is present
    }

    return;
}

TSPacket::TSPacket(const unsigned char *const packetStart) {
    this->start = packetStart;
    parsePacket(packetStart);
}

void TSPacket::print() {
    std::cout << std::hex << 
        "PID " << this->PID << std::endl <<
        "Transport Error Indicator " << this->transport_error_indicator << std::endl << 
        "Payload Unit Start Indicator " << this->payload_unit_start_indicator << std::endl <<
        "Transport Priority " << this->transport_priority << std::endl <<
        "Transport Scrambling Control " << this->transport_scrambling_control << std::endl <<
        std::dec << "Adaptation Field Control " << this->adaptation_field_control << std::endl <<
        "Continuity Counter " << this->continuity_counter << std::endl << std::endl;
}