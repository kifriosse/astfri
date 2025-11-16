#include <libastfri-text/inc/code/JavaCodeBuilder.hpp>

#include <libastfri-text/inc/OutputFileManager.hpp>

#include <fstream>

using namespace astfri::text;

JavaCodeBuilder& JavaCodeBuilder::get_instance()
{
    static JavaCodeBuilder builder;
    return builder;
}

//
// GENERAL_TEXT
//

void JavaCodeBuilder::write_opening_curl_bracket()
{
    append_space();
    append_text("{");
    append_new_line();
    ++indentationLevel_;
}

void JavaCodeBuilder::write_opening_else_word()
{
    append_space();
    append_text("else");
    write_opening_curl_bracket();
}

//
// EXPORT
//

void JavaCodeBuilder::create_java_file(std::string const& name, Scope const& scope)
{
    std::string folder = "build";
    OutputFileManager::get_instance().check_and_set_file_path(folder);
    folder.append("src/");
    for (size_t i = 0; i < scope.names_.size(); ++i)
    {
        folder.append(scope.names_.at(i) + "/");
    }
    folder.append(name + ".java");
    std::ofstream(folder) << buildedText_->str();
    reset_builder();
}

void JavaCodeBuilder::create_java_file(std::string const& name)
{
    std::string folder = "build";
    OutputFileManager::get_instance().check_and_set_file_path(folder);
    folder.append("src/");
    folder.append(name + ".java");
    std::ofstream(folder) << buildedText_->str();
    reset_builder();
}