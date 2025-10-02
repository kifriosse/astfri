#pragma once

#include <libastfri-uml/inc/Config.hpp>
#include <libastfri-uml/inc/ElementStructs.hpp>

#include <iostream>

namespace astfri::uml
{
class UMLOutputter
{
private:
protected:
    Config* config_;
    std::string outputString_;

public:
    void set_config(Config const& config);

    virtual void write_to_file();
    virtual void write_to_console();

    virtual std::string getFileExtension()                        = 0;

    virtual void open_user_type(ClassStruct c, UserDefinedType t) = 0;
    virtual void close_user_type()                                = 0;

    virtual void add_data_member(VarStruct v)                     = 0;
    virtual void add_function_member(MethodStruct m)              = 0;

    virtual void add_relation(RelationStruct r)                   = 0;
};

} // namespace astfri::uml
