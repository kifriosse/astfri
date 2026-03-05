#ifndef LIBASTFRI_TEXT_BASIC_TEXT_STRUCTURE
#define LIBASTFRI_TEXT_BASIC_TEXT_STRUCTURE

namespace astfri::text
{
    struct BasicTextStructure
    {
    public:
        int tabulatorLength_; ///< length of indentation is same for each situation
        //
        bool useNamespaceTabulator_; ///< should use indentation for namespace block?
        bool namespaceBlockBracketNewLine_; ///< should open namespace body bracket on new line?
        //
        bool objectBlockBracketNewLine_; ///< should open object body bracket on new line?
        bool functionBlockBracketNewLine_; ///< should open func/method body bracket on new line?
        //
        bool loopBlockBracketNewLine_; ///< should open loop body bracket on new line?
        bool dowhileConditionNewLine_; ///< should write while condition on new line?
        //
        bool conditionBlockBracketNewLine_; ///< should open condition body bracket on new line?
        bool elseifConditionNewLine_; ///< should write else-if condition on new line?
        bool elseConditionNewLine_; ///< should write else condition on new line?
        bool switchBlockBracketNewLine_; ///< shoud open switch body bracket on new line?
        //
        bool trycatchBlockBracketNewLine_; ///< should open try-catch body bracket on new line?
        bool catchConditionNewLine_; ///< should write catch condition on new line?
    public:
        BasicTextStructure();
        virtual ~BasicTextStructure() = default;
        //
        void set_default(); ///< set default values
    };
}

#endif