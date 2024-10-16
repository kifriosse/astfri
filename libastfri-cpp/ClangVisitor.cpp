#include <libastfri-cpp/ClangVisitor.hpp>

namespace astfri::cpp
{
ClangVisitor::ClangVisitor(TranslationUnit& visitedTranslationUnit) :
    tu_(&visitedTranslationUnit)
{
}
} // namespace libastfri::cpp