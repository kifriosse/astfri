#ifndef ASTFRI_TED_IMPL_TED_INDEX_HPP
#define ASTFRI_TED_IMPL_TED_INDEX_HPP

#include <string>
#include <vector>

namespace astfri::ted {

struct TedIndex {
    // identifikator pre porovnavanie rovnosti 2 uzlov 
    // (cost operacie 0 ak sa rovnaju)
    std::vector<std::string> label; // 1 - indexovany

    // postorder cislo najlavejsieho listu v podstrome uzla i
    std::vector<int> l; // 1 - indexovany
    
    // musia byt v vzostupnom poradi, aby pri vypocte vacsich podstromov
    // boli uz vypocitane hodnoty v treedist mensich podstromov
    std::vector<int> keyroots; // 1 - indexovany

    int node_count() const;
};

} // namespace astfri::ted

#endif