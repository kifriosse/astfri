#ifndef ASTFRI_TED_ZHANG_SHASHA_HPP
#define ASTFRI_TED_ZHANG_SHASHA_HPP

#include <vector>
#include <astfri-ted/impl/TedIndex.hpp>
#include <astfri-ted/Costs.hpp>

namespace astfri::ted {
class ZhangShasha {
public:
    ZhangShasha(const TedIndex& tedIndex1, const TedIndex& tedIndex2, Costs costs);
    
    int calculate_treedistance();

private:
    bool are_nodes_equal(int i, int j) const;
    void calculate_treedistance(int i, int j);

private:
    const TedIndex& m_tedIndex1;
    const TedIndex& m_tedIndex2;
    const Costs m_costs;

    // treedistance (nema nulte riadky pre prazdne stromy)
    std::vector<std::vector<int>> m_td;

    // tato 'matica' forestdistance sa znova pouziva a nikdy necisti
    // pri kazdom vypocte sa pouziva len ta cast matice,
    // ktoru treba
    //
    // na nultych indexoch su 'prazdne' lesy
    std::vector<std::vector<int>> m_fd;
};

} // namespace astfri::ted

#endif