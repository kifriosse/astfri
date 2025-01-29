#include <string>

class ExporterConfig
{
private:
    enum pohlad_ {VNUTORNY = 0, VONKAJSI};
    std::string intTypeName_;
    std::string boolTypeName_;
public:
    virtual void parse_json(const char* path) = 0;
    virtual void save_json(const char* path) = 0;
    std::string getIntType() { return this->intTypeName_; }
};