#include <astfri-ted/impl/TedIndex.hpp>

namespace astfri::ted {

int TedIndex::node_count() const {
    return this->label.size() - 1;
}

} // namespace astfri::ted