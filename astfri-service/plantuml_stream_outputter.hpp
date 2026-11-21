#ifndef ASTFRI_SERVICE_PLANTUML_STREAM_OUTPUTTER_HPP
#define ASTFRI_SERVICE_PLANTUML_STREAM_OUTPUTTER_HPP

#include <astfri-uml/impl/PlantUMLOutputter.hpp>

#include <ostream>

namespace astfri::service {

class PlantUMLStreamOutputter : public astfri::uml::PlantUMLOutputter
{
public:
    explicit PlantUMLStreamOutputter(std::ostream& out) : out_(out)
    {
    }

    void write_to_stream(std::ostream& p_stream) override
    {
        PlantUMLOutputter::write_to_stream(out_);
    }

private:
    std::ostream& out_;
};

} // namespace astfri::service

#endif
