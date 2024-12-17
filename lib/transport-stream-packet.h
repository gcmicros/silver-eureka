#include <vector>

#include "adaptation-field.h"

class TSPacket {
    const int MAX_PAYLOAD_BYTES = 184;
    const unsigned char* start = nullptr;
    int payload_size = MAX_PAYLOAD_BYTES;
    bool transport_error_indicator = false;
    bool payload_unit_start_indicator = false;
    bool transport_priority = false;
    unsigned int PID;
    unsigned int transport_scrambling_control = 0;
    unsigned int adaptation_field_control = 0;
    unsigned int continuity_counter = 0;
    AdaptationField adaptation_field;
    std::vector<char> payload;        

    void parsePacket(const unsigned char *const packetStart);
    
    public:
        TSPacket(const unsigned char *const packetStart); 

        void print(); 

        unsigned int getPID();

        bool getPUSI();

        unsigned int getPayloadSize();

        const unsigned char* const getPayloadStart();
};