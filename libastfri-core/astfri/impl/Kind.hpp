#ifndef ASTFRI_IMPL_KIND_HPP
#define ASTFRI_IMPL_KIND_HPP

namespace astfri {


template<typename T>
struct KindOf {
    // sizeof(T) == 0 is allways false, but:
    // static_assert(false, ...)
    // would trigger allways, this way it only triggers if someone
    // actually instantiates this, which should never happen
    static_assert(sizeof(T) == 0, "KindOf is not specialized for this type!");
};


} // namespace astfri

#endif
