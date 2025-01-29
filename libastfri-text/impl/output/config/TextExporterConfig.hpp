#include "libastfri-text/impl/output/config/ExporterConfig.hpp"

class TextExporterConfig : ExporterConfig
{
private:
    int numOfIndentSpaces = 2;
public:
    virtual void parse_json(const char* path) override;
    virtual void save_json(const char* path) override;
};