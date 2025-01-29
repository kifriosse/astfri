#include "libastfri-text/impl/output/export/Exporter.hpp"
#include <iostream>

class UMLExporter : public Exporter
{
public:
    void exportt() override
    {
        // ...
    }

    void write_class()
    {
        // ...
    }

    void write_int()
    {
        std::cout << this->getConfig()->getIntType() << std::endl;
    }

};