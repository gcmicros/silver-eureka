class ProgramSpecificInformation {
    unsigned int pointer_field = 0;             // 8
    unsigned int tableID;                       // 8
    bool section_syntax_indicator;              // 1
    unsigned char private_bit;                  // 1
    unsigned char reserved_bits;                // 2
    unsigned int section_length_unused_bits;    // 2
    unsigned int section_length;                // 10
    unsigned int tableID_extension;             // 16
    unsigned char section_reserved_bits;        // 2
    unsigned int version_number;                // 5
    unsigned int current_indicator;             // 1
    unsigned int section_number;                // 8
    unsigned int last_section_number;           // 8

    unsigned int PID;
    bool payload_unit_start_indicator;
    unsigned int offset = 0;
    const unsigned char* psi_start;
    public:
        ProgramSpecificInformation(bool payloadUnitStartIndicator);

        void parse(const unsigned char *const start);
    
        void print();

        int getPayloadLength();

        const unsigned char *const getPayloadStart();
};
