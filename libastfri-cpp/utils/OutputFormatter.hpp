#pragma once

#include <libastfri/utils/OutputWriter.hpp>

#include <clang/Basic/ExpressionTraits.h>
#include <string>

namespace libastfri::utils
{
enum class OutputToken
{
    Keyword,
    Identifier,
    Literal,
    IntLiteral,
    FloatLiteral,
    CharLiteral,
    StringLiteral,
    BoolLiteral,
    Operator,
    Separator,
    Comment,
    Error,
    None
};

class IOutputFormatter
{
public:
    virtual IOutputWriter& getWriter ()                            = 0;

    virtual void indentIncrease ()                                 = 0;
    virtual void indentDecrease ()                                 = 0;
    virtual int getIndentLevel ()                                  = 0;
    virtual void printIndent ()                                    = 0;

    virtual void startInlinePrinting ()                            = 0;
    virtual void endInlinePrinting ()                              = 0;
    virtual bool isInlinePrinting ()                               = 0;

    virtual void printEndl (bool semicolon = true)                 = 0;

    virtual void print (std::string const& str)                    = 0;
    virtual void print (std::string const& str, OutputToken token) = 0;
    virtual void printIndented (
        std::string const& str,
        OutputToken token = OutputToken::None
    )                                                        = 0;

    virtual void printKeyword (std::string const& str)       = 0;
    virtual void printIdentifier (std::string const& str)    = 0;
    virtual void printLiteral (std::string const& str)       = 0;
    virtual void printIntLiteral (std::string const& str)    = 0;
    virtual void printFloatLiteral (std::string const& str)  = 0;
    virtual void printCharLiteral (std::string const& str)   = 0;
    virtual void printStringLiteral (std::string const& str) = 0;
    virtual void printBoolLiteral (std::string const& str)   = 0;
    virtual void printOperator (std::string const& str)      = 0;
    virtual void printSeparator (std::string const& str)     = 0;
    virtual void printComment (std::string const& str)       = 0;
    virtual void printError (std::string const& str)         = 0;
    virtual void printNone (std::string const& str)          = 0;
};

class OutputFormatterAdapter : public IOutputFormatter
{
public:
    OutputFormatterAdapter(IOutputWriter& writer);

    virtual IOutputWriter& getWriter () override;

    virtual void indentIncrease () override;
    virtual void indentDecrease () override;
    virtual int getIndentLevel () override;

    virtual void startInlinePrinting () override;
    virtual void endInlinePrinting () override;
    virtual bool isInlinePrinting () override;

    virtual void print (std::string const& str) override;
    virtual void print (std::string const& str, OutputToken token) override;
    virtual void printIndented (
        std::string const& str,
        OutputToken token = OutputToken::None
    ) override;

    virtual void printKeyword (std::string const& str) override;
    virtual void printIdentifier (std::string const& str) override;
    virtual void printLiteral (std::string const& str) override;
    virtual void printIntLiteral (std::string const& str) override;
    virtual void printFloatLiteral (std::string const& str) override;
    virtual void printCharLiteral (std::string const& str) override;
    virtual void printStringLiteral (std::string const& str) override;
    virtual void printBoolLiteral (std::string const& str) override;
    virtual void printOperator (std::string const& str) override;
    virtual void printSeparator (std::string const& str) override;
    virtual void printComment (std::string const& str) override;
    virtual void printError (std::string const& str) override;
    virtual void printNone (std::string const& str) override;

private:
    IOutputWriter& writer;
    int indentLevel;
    bool inlinePrinting;
};
} // namespace libastfri::utils