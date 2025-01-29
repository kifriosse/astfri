#include "libastfri-text/impl/output/config/ExporterConfig.hpp"

class Exporter
{
private:
    //AST* ast_;
    ExporterConfig* config_;
public:
    virtual void exportt() = 0;
    ExporterConfig* getConfig() {return this->config_;}
};