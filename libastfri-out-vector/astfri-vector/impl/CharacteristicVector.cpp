#include <astfri-vector/impl/CharacteristicVector.hpp>

namespace astfri::vector {

CharacteristicVector::CharacteristicVector() : 
    m_characteristicVector(VECTOR_SIZE, 0)
{}    

void CharacteristicVector::increment(VectorComponent comp) {
    const size_t index = static_cast<size_t>(comp);
    if (index < m_characteristicVector.size()) {
        m_characteristicVector[index]++;
    }
}

int CharacteristicVector::get(VectorComponent comp) const {
    const size_t index = static_cast<size_t>(comp);
    if (index < m_characteristicVector.size()) {
        return m_characteristicVector[index];
    }
    return 0;
}

int CharacteristicVector::total_tokens() const {
    int size = 0;
    for (int count : m_characteristicVector) {
        size += count;
    }
    return size;
}

bool CharacteristicVector::is_empty() const {
    return this->total_tokens() == 0;
}

CharacteristicVector& CharacteristicVector::operator+=(const CharacteristicVector &rhs) {
    for (size_t i = 0; i < m_characteristicVector.size(); ++i) {
        m_characteristicVector[i] += rhs.m_characteristicVector[i];
    }
    return *this;
}

CharacteristicVector CharacteristicVector::operator+(const CharacteristicVector &rhs) const {
    CharacteristicVector result = *this;
    result += rhs;
    return result;
}

bool CharacteristicVector::operator==(const CharacteristicVector& rhs) const {
    return m_characteristicVector == rhs.m_characteristicVector;
}

bool CharacteristicVector::operator!=(const CharacteristicVector& rhs) const {
    return !(*this == rhs);
}




VectorTreeNode::VectorTreeNode(std::optional<VectorComponent> comp) : 
    component(comp)    
{}

VectorTreeNode::~VectorTreeNode() {
    for (VectorTreeNode* child : children) {
        delete child;
    }
}

bool VectorTreeNode::is_non_terminal() const {
    return !children.empty();
}

}