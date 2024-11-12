#include <libastfri-cpp/inc/ClangVisitor.hpp>

namespace astfri::cpp
{
ClangVisitor::ClangVisitor(TranslationUnit& visitedTranslationUnit) :
    tu_(&visitedTranslationUnit)
{
}
} // namespace libastfri::cpp