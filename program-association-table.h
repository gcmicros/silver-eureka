#include <map> 

#ifndef PSI
#define PSI

#include "program-specific-information.h"

#endif

class ProgramAssociationTable : ProgramSpecificInformation {
    std::map<unsigned int, unsigned int> programs;

    public:
        ProgramAssociationTable(bool payloadUnitStartIndicator);
        
        void parse(const unsigned char* const start);    

        void print();

        std::map<unsigned int, unsigned int> getPrograms();
};
