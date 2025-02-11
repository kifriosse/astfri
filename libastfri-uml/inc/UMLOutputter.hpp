#pragma once

#include <iostream>

#include <libastfri-uml/inc/Config.hpp>
#include <libastfri-uml/inc/ElementStructs.hpp>

namespace uml {
    class UMLOutputter {
    private:

    protected:
        Config* config_;
        std::string outputString_;

    public:
        void set_config(Config* config);

        void write_to_file();
        void write_to_console();

        virtual void open_class(ClassStruct c) = 0;
        virtual void close_class() = 0;

        virtual void add_data_member(VarStruct v) = 0;
    
    };

} // namespace uml