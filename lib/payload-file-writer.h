#include <fstream>
#include <string>

class PayloadFileWriter {
    std::fstream output_file;
    public:
        PayloadFileWriter(std::string filename);
        ~PayloadFileWriter();

        void appendChunk(const unsigned char *const start, unsigned int length);
};