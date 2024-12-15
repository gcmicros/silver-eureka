#include <vector>

#include "adaptation-field.h"


class TSPacket {
    const int MAX_PAYLOAD_BYTES = 184;
    const unsigned char* start;
    int payload_size = MAX_PAYLOAD_BYTES;
    bool transport_error_indicator;
    bool payload_unit_start_indicator;
    bool transport_priority;
    unsigned int PID;
    unsigned int transport_scrambling_control;
    unsigned int adaptation_field_control;
    unsigned int continuity_counter;
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
