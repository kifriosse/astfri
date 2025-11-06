#include <libastfri-text/inc/AstfriVisitor.hpp>

using namespace astfri::text;

AstfriVisitor::AstfriVisitor(Exporter*& exp) :
    exporter_(exp)
{
}

void AstfriVisitor::set_exporter(Exporter*& exp)
{
    exporter_ = exp;
}

void AstfriVisitor::write_comma_space(bool isNotLast)
{
    if (isNotLast)
    {
        exporter_->write_separator_sign(",");
        exporter_->write_space();
    }
}