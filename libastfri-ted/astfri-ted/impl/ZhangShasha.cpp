#include <astfri-ted/ZhangShasha.hpp>

#include <algorithm>

namespace astfri::ted {

ZhangShasha::ZhangShasha(const TedIndex& tedIndex1, const TedIndex& tedIndex2, Costs costs, NodeComparator& comparator) :
    m_tedIndex1(tedIndex1),
    m_tedIndex2(tedIndex2),
    m_costs(costs),
    m_comparator(comparator),
    m_td(tedIndex1.node_count(),
               std::vector<int>(tedIndex2.node_count(), 0)),

    m_fd(tedIndex1.node_count() + 1, // + 1 pre prazdne stromy
               std::vector<int>(tedIndex2.node_count() + 1, 0)) { }

int ZhangShasha::calculate_treedistance() {
    for (std::size_t i = 1; i < this->m_tedIndex1.keyroots.size(); ++i) {
        for (std::size_t j = 1; j < this->m_tedIndex2.keyroots.size(); ++j) {
            int keyrootI = this->m_tedIndex1.keyroots[i];
            int keyrootJ = this->m_tedIndex2.keyroots[j];

            this->calculate_treedistance(keyrootI, keyrootJ);
        }
    }

    return this->m_td[this->m_tedIndex1.node_count() - 1][this->m_tedIndex2.node_count() - 1];
}

// private

bool ZhangShasha::are_nodes_equal(int i, int j) const {
    return this->m_comparator.equal(this->m_tedIndex1.nodes[i], this->m_tedIndex2.nodes[j]);
}

void ZhangShasha::calculate_treedistance(int i, int j) {
    const int li = this->m_tedIndex1.l[i];
    const int lj = this->m_tedIndex2.l[j];
    
    // zakladny stav
    this->m_fd[li - 1][lj - 1] = 0;

    // nulty riadok
    for (int j1 = lj; j1 <= j; ++j1) {
        this->m_fd[li - 1][j1] = this->m_fd[li - 1][j1 - 1] + this->m_costs.insert;
    }

    // nulty stlpec
    for (int i1 = li; i1 <= i; ++i1) {
        this->m_fd[i1][lj - 1] = this->m_fd[i1 - 1][lj - 1] + this->m_costs.remove;
    }

    for (int i1 = li; i1 <= i; ++i1) {
        for (int j1 = lj; j1 <= j; ++j1) {
            
            // ak sa rovna, stale robime s 2 stromami
            if (this->m_tedIndex1.l[i1] == li && this->m_tedIndex2.l[j1] == lj) {
                this->m_fd[i1][j1] = std::min({
                    this->m_fd[i1 - 1][j1] + this->m_costs.remove,
                    this->m_fd[i1][j1 - 1] + this->m_costs.insert,

                    this->m_fd[i1 - 1][j1 - 1] +
                        (this->are_nodes_equal(i1, j1) ? 0 : this->m_costs.rename)
                });

                // nakolko tu robime zo stromami, vyratali sme skutocnu 
                // treedist a nie forestdist, takze hodnotu ulozime
                this->m_td[i1 - 1][j1 - 1] = this->m_fd[i1][j1];

            // forest
            } else {
                this->m_fd[i1][j1] = std::min({
                    // tieto 2 operacie funguju lokalne na 1 uzle, pretoze
                    // sa deti len posunu v lese
                    this->m_fd[i1 - 1][j1] + this->m_costs.remove,
                    this->m_fd[i1][j1 - 1] + this->m_costs.insert,
                    
                    // tu sa NErobi rename na jednom uzle!
                    // rename (transformovanie) jedneho uzla by to
                    // ignorovalo cely les pod nim a narusilo strukturu
                    //
                    // preto sa robi transformacia celeho podstromu a
                    // cena, operacii, kolko by stalo transformovat taky
                    // podstrom je uz vypocitana v treedistance
                    //
                    //
                    this->m_fd[this->m_tedIndex1.l[i1] - 1][this->m_tedIndex2.l[j1] - 1] 
                        + this->m_td[i1 - 1][j1 - 1]
                });
            }

        }
    }
    
}


} // namespace astfri::ted