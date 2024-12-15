class AdaptationField {
    // NOTE(gmicros): uninitilized var was bug
    unsigned int adaptation_field_length = 0;
    unsigned char discontinuity_indicator;
    unsigned char random_access_indicator;
    unsigned char elementary_stream_priority_indicator;
    unsigned char pcr_flag;
    unsigned char opcr_flag;
    unsigned char splicing_point_flag;
    unsigned char transport_private_data_flag;
    unsigned char adaptation_field_extention;
    
    public:
        void parse(const unsigned char *const adaptationFieldStart) {
            this->adaptation_field_length = (unsigned int)adaptationFieldStart[0]; 
        }

        unsigned int getLength() {
            return this->adaptation_field_length; 
        }
};
