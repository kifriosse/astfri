#ifndef LIBASTFRI_TEXT_ABSTRACT_CODE_VISITOR
#define LIBASTFRI_TEXT_ABSTRACT_CODE_VISITOR

#include <libastfri-text/inc/AbstractVisitor.hpp>
#include <libastfri-text/inc/code/AbstractCodeBuilder.hpp>

namespace astfri::text
{
    class AbstractCodeVisitor : public AbstractVisitor
    {
    public:
        explicit AbstractCodeVisitor(AbstractCodeBuilder* const& builder);
        virtual ~AbstractCodeVisitor() = default;
    };
}

#endif