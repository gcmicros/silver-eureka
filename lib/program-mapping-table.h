#ifndef PSI
#define PSI

#include "program-specific-information.h"

#endif

#include <map>

class ProgramMappingTable : ProgramSpecificInformation {
    std::map<unsigned int, unsigned int> PID_to_es_type;
    public:
        ProgramMappingTable(bool payloadUnitStartIndicator);

        void parse(const unsigned char* const start);

        std::map<unsigned int, unsigned int> getElemStreamPIDS();
};