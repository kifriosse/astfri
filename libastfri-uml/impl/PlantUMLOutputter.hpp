#ifndef LIBASTFRIUML_PLANT_UML_OUTPUTTER_HPP
#define LIBASTFRIUML_PLANT_UML_OUTPUTTER_HPP

#include <libastfri-uml/impl/UMLOutputter.hpp>

namespace astfri::uml
{
class PlantUMLOutputter : public UMLOutputter
{
private:
    void open(ClassStruct const& cs);
    void apply_style_from_config();
    void add_tags_and_style();
    virtual std::string assemble_param(VarStruct p) override;

public:
    virtual void write_to_file() override;
    virtual void write_to_console() override;

    virtual std::string getFileExtension() override;

    virtual void open_user_type(ClassStruct c) override;
    virtual void close_user_type() override;

    virtual void add_data_member(VarStruct v) override;
    virtual void add_function_member(MethodStruct m) override;
    virtual void add_constructor(ConstructorStruct c) override;
    virtual void add_destructor(DestructorStruct d) override;

    virtual void add_relation(RelationStruct r) override;
};

} // namespace astfri::uml

#endif
