#include <astfri/impl/TypeInfo.hpp>

#include <astfri/impl/ExprDef.hpp>
#include <astfri/impl/StmtDef.hpp>
#include <astfri/impl/TypeDef.hpp>


namespace astfri {


namespace detail {


struct base_list_t {
    ASTNodeType bases[static_cast<int>(ASTNodeType::COUNT) - 1];
};

template<int I>
constexpr void init_base_list_impl(base_list_t &list) {
    if constexpr (I == static_cast<int>(ASTNodeType::COUNT) - 1) {
        return;
    } else {
        const ASTNodeType i_tag = static_cast<ASTNodeType>(I);
        const ASTNodeType parent_tag = BaseOf<i_tag>::value;
        list.bases[I] = parent_tag;
        init_base_list_impl<I+1>(list);
    }
}

constexpr base_list_t create_base_list() {
    base_list_t list{};
    init_base_list_impl<1>(list);
    return list;
}

bool is_a_impl(ASTNodeType node_type, ASTNodeType parent_type) {
    static constinit base_list_t s_bases = create_base_list();
    ASTNodeType base_type = node_type;
    while (base_type != ASTNodeType::NO_TYPE) {
        if (base_type == parent_type) {
            return true;
        }
        base_type = s_bases.bases[static_cast<int>(base_type)];
    }
    return false;
}


} // namespace detail


} // namespace astfri
