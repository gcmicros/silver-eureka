#include <string>

class PayloadFileWriter {
    public:
        PayloadFileWriter(std::string filename);
        ~PayloadFileWriter();

        void appendChunk(const unsigned char *const start, unsigned int length);
};