#pragma once

#include <libastfri-uml/inc/UMLOutputter.hpp>
#include "libastfri-uml/inc/ElementStructs.hpp"

namespace uml {
    class PlantUMLOutputter : public UMLOutputter {
    private:

    public:
        virtual void open_class(ClassStruct c) override;
        virtual void close_class() override;

        virtual void add_data_member(VarStruct v) override;
        virtual void add_function_member(MethodStruct m) override;

        virtual void add_relation(RelationStruct r) override;
    };

} // namespace uml