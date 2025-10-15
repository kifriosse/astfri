#include <libastfri-text/inc/GeneralASTVisitor.hpp>

using namespace astfri::text;

GeneralASTVisitor::GeneralASTVisitor(Exporter*& exp) :
    exporter_(exp)
{
}

void GeneralASTVisitor::set_exporter(Exporter*& exp)
{
    exporter_ = exp;
}

void GeneralASTVisitor::write_comma_space(bool isNotLast)
{
    if (isNotLast)
    {
        exporter_->write_separator_sign(",");
        exporter_->write_space();
    }
}