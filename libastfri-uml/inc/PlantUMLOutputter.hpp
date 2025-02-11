#pragma once

#include <libastfri-uml/inc/UMLOutputter.hpp>

namespace uml {
    class PlantUMLOutputter : public UMLOutputter {
    private:

    public:
        virtual void open_class(ClassStruct c) override;
        virtual void close_class() override;

        virtual void add_data_member(VarStruct v) override;
    };

} // namespace uml