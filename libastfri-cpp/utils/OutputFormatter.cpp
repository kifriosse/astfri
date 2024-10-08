#include <libastfri/utils/OutputFormatter.hpp>

namespace libastfri::utils
{
namespace lsfu = libastfri::utils;

lsfu::OutputFormatterAdapter::OutputFormatterAdapter(IOutputWriter& writer) :
    writer(writer),
    indentLevel(0),
    inlinePrinting(false)
{
}

IOutputWriter& lsfu::OutputFormatterAdapter::getWriter()
{
    return writer;
}

void lsfu::OutputFormatterAdapter::indentIncrease()
{
    indentLevel++;
}

void lsfu::OutputFormatterAdapter::indentDecrease()
{
    indentLevel--;
}

int lsfu::OutputFormatterAdapter::getIndentLevel()
{
    return indentLevel;
}

void lsfu::OutputFormatterAdapter::startInlinePrinting()
{
    inlinePrinting = true;
}

void lsfu::OutputFormatterAdapter::endInlinePrinting()
{
    inlinePrinting = false;
}

bool lsfu::OutputFormatterAdapter::isInlinePrinting()
{
    return inlinePrinting;
}

void lsfu::OutputFormatterAdapter::print(std::string const& str)
{
    writer.write(str);
}

void lsfu::OutputFormatterAdapter::print(
    std::string const& str,
    OutputToken token
)
{
    switch (token)
    {
    case OutputToken::Keyword:
        printKeyword(str);
        break;
    case OutputToken::Identifier:
        printIdentifier(str);
        break;
    case OutputToken::Literal:
        printLiteral(str);
        break;
    case OutputToken::IntLiteral:
        printIntLiteral(str);
        break;
    case OutputToken::FloatLiteral:
        printFloatLiteral(str);
        break;
    case OutputToken::CharLiteral:
        printCharLiteral(str);
        break;
    case OutputToken::StringLiteral:
        printStringLiteral(str);
        break;
    case OutputToken::BoolLiteral:
        printBoolLiteral(str);
        break;
    case OutputToken::Operator:
        printOperator(str);
        break;
    case OutputToken::Separator:
        printSeparator(str);
        break;
    case OutputToken::Comment:
        printComment(str);
        break;
    case OutputToken::Error:
        printError(str);
        break;
    case OutputToken::None:
    default:
        printNone(str);
        break;
    }
}

void lsfu::OutputFormatterAdapter::printIndented(
    std::string const& str,
    OutputToken token
)
{
    printIndent();
    print(str, token);
}

void lsfu::OutputFormatterAdapter::printKeyword(std::string const& str)
{
    print(str);
}

void lsfu::OutputFormatterAdapter::printIdentifier(std::string const& str)
{
    print(str);
}

void lsfu::OutputFormatterAdapter::printLiteral(std::string const& str)
{
    print(str);
}

void lsfu::OutputFormatterAdapter::printIntLiteral(std::string const& str)
{
    print(str);
}

void lsfu::OutputFormatterAdapter::printFloatLiteral(std::string const& str)
{
    print(str);
}

void lsfu::OutputFormatterAdapter::printCharLiteral(std::string const& str)
{
    print(str);
}

void lsfu::OutputFormatterAdapter::printStringLiteral(std::string const& str)
{
    print(str);
}

void lsfu::OutputFormatterAdapter::printBoolLiteral(std::string const& str)
{
    print(str);
}

void lsfu::OutputFormatterAdapter::printOperator(std::string const& str)
{
    print(str);
}

void lsfu::OutputFormatterAdapter::printSeparator(std::string const& str)
{
    print(str);
}

void lsfu::OutputFormatterAdapter::printComment(std::string const& str)
{
    print(str);
}

void lsfu::OutputFormatterAdapter::printError(std::string const& str)
{
    print(str);
}

void lsfu::OutputFormatterAdapter::printNone(std::string const& str)
{
    print(str);
}

} // namespace libastfri::utils