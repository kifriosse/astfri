#include <libastfri-uml/impl/UMLOutputter.hpp>

namespace astfri::uml
{
void UMLOutputter::set_config(Config const& config)
{
    this->config_ = (Config*)&config;
}

void UMLOutputter::write_to_stream(std::ostream& p_stream)
{
    p_stream << this->outputString_;
}

void UMLOutputter::write_to_console()
{
    //std::cout << "UMLOutputter - diagram output:" << std::endl;
    write_to_stream(std::cout);
}

void UMLOutputter::write_to_file()
{
    std::string v_path = this->config_->outputFilePath_ + this->getFileExtension();
    std::fstream v_stream;
    v_stream.open(v_path);

    if (!v_stream.is_open())
    {
        // TODO - throw?
    }

    write_to_stream(v_stream);
    v_stream.close();
}

} // namespace astfri::uml
