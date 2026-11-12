#ifndef ASTFRI_TED_IMPL_TED_INDEX_BUILDER_HPP
#define ASTFRI_TED_IMPL_TED_INDEX_BUILDER_HPP

#include <astfri-ted/impl/TedIndex.hpp>
#include <astfri/impl/StmtFwd.hpp>

namespace astfri::ted {

class TedIndexBuilder {
public:
    TedIndexBuilder() = delete;

    static TedIndex build(const astfri::TranslationUnit& translationUnit);
};

} // namespace astfri::ted

#endif
