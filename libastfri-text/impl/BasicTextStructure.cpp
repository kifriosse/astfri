#include <libastfri-text/inc/BasicTextStructure.hpp>

using namespace astfri::text;

BasicTextStructure::BasicTextStructure()
{
    set_default();
}

void BasicTextStructure::set_default()
{
    tabulatorLength_ = 4;
    //
    useNamespaceTabulator_ = false;
    namespaceBlockBracketNewLine_ = false;
    //
    objectBlockBracketNewLine_ = false;
    functionBlockBracketNewLine_ = false;
    //
    loopBlockBracketNewLine_ = false;
    dowhileConditionNewLine_ = false;
    //
    conditionBlockBracketNewLine_ = false;
    elseifConditionNewLine_ = false;
    elseConditionNewLine_ = false;
    switchBlockBracketNewLine_ = false;
    //
    trycatchBlockBracketNewLine_ = false;
    catchConditionNewLine_ = false;
}