class PES{
    const unsigned char* start;
    unsigned int packet_length;
        void parse();
    public:
        PES(const unsigned char* const start);
        unsigned int getPacketLength();
        const unsigned char* const getPacketStart(); 
};