#pragma once

#include <libastfri-uml/inc/UMLOutputter.hpp>
#include "libastfri-uml/inc/ElementStructs.hpp"

namespace astfri::uml {
    class PlantUMLOutputter : public UMLOutputter {
    private:
        void open(ClassStruct const& cs);

    public:
        virtual std::string getFileExtension() override;

        virtual void open_class(ClassStruct c) override;
        virtual void open_interface(ClassStruct i) override;
        virtual void close_class() override;

        virtual void add_data_member(VarStruct v) override;
        virtual void add_function_member(MethodStruct m) override;

        virtual void add_relation(RelationStruct r) override;
    };

} // namespace astfri::uml