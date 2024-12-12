#include <iostream>

#include "program-specific-information.h"

ProgramSpecificInformation::ProgramSpecificInformation(bool payloadUnitStartIndicator) {
    this->payload_unit_start_indicator = payloadUnitStartIndicator;
}

int ProgramSpecificInformation::getPayloadLength() {
    return this->section_length
        - 5     // other section field
        - 4;    // CRC
}

const unsigned char *const ProgramSpecificInformation::getPayloadStart() {
    return this->psi_start + this->offset
        + 9; // section stuff
}

void ProgramSpecificInformation::parse(const unsigned char *const start) {
    this->psi_start = start;
    this->pointer_field = start[0];
    if (this->payload_unit_start_indicator) {
        this->offset = this->pointer_field;
    }

    this->tableID = start[this->offset+1];
    this->section_syntax_indicator = start[this->offset+2] & 0b10000000;
    this->private_bit = (start[this->offset+2] & 0b01000000) >> 6;
    this->reserved_bits = (start[this->offset + 2] & 0b00110000) >> 4;
    this->section_length = ((start[this->offset + 2] & 0x0F) << 8) | start[offset+3];
    this->tableID_extension = start[this->offset + 4] << 8 | start[offset + 5];
    this->section_reserved_bits = (start[this->offset + 6] & 0b11000000) >> 6;
    this->version_number = (start[this->offset + 6] & 0b00111110) >> 1;
    this->current_indicator = start[this->offset + 6] & 0b1;
    this->section_number = start[this->offset + 7];
    this->last_section_number = start[this->offset + 8];
}

void ProgramSpecificInformation::print() {
    std::cout << std::hex <<
        "TableID: " << this->tableID << std::endl <<
        "Section Syntax Indicator: " << this->section_syntax_indicator << std::endl <<
        "Private bit: " << this->private_bit << std::endl <<
        "Reserved bits: " << this->reserved_bits << std::endl <<
        "Section Length: " << std::dec << this->section_length << std::endl << std::hex <<
        "TableId ext: " << std::dec << this->tableID_extension << std::hex << std::endl <<
        "Section reserved bits: " << this->section_reserved_bits << std::endl <<
        "Version number: " << this->version_number << std::endl <<
        "Current indicator: " << this->current_indicator << std::endl <<
        "Section number: " << this->section_number << std::endl <<
        "Last section number: " << this->last_section_number << std::endl;
}
