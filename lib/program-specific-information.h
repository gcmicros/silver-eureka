class ProgramSpecificInformation {
    unsigned int pointer_field = 0;                  // 8
    unsigned int tableID = 0;                        // 8
    bool section_syntax_indicator = false;           // 1
    unsigned char private_bit = 0x00;                // 1
    unsigned char reserved_bits = 0x00;              // 2
    unsigned int section_length_unused_bits = 0;     // 2
    unsigned int section_length = 0;                 // 10
    unsigned int tableID_extension = 0;              // 16
    unsigned char section_reserved_bits = 0x00;      // 2
    unsigned int version_number = 0;                 // 5
    unsigned int current_indicator = 0;              // 1
    unsigned int section_number = 0;                 // 8
    unsigned int last_section_number = 0;            // 8

    unsigned int PID = 0;
    bool payload_unit_start_indicator = false;
    unsigned int offset = 0;
    const unsigned char* psi_start = nullptr;
    public:
        ProgramSpecificInformation(bool payloadUnitStartIndicator);

        void parse(const unsigned char *const start);
    
        void print();

        int getPayloadLength();

        const unsigned char *const getPayloadStart();
};
