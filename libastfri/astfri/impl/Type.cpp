#include <astfri/impl/Type.hpp>


namespace astfri {


IndirectionType::IndirectionType(Type* const indirect) :
    indirect(indirect) {
}

ScopedType::ScopedType(std::string name, Scope scope) :
    name(std::move(name)),
    scope(std::move(scope)) {
}

ClassType::ClassType(std::string name, Scope scope, ClassDefStmt* def) :
    ScopedType(name, scope),
    def(def) {
}

InterfaceType::InterfaceType(std::string name, Scope scope, InterfaceDefStmt* def) :
    ScopedType(name, scope),
    def(def) {
}

LambdaType::LambdaType(std::string name, LambdaExpr* def) :
    name(std::move(name)),
    def(def) {
}

DeducedType::DeducedType(Type* realType) :
    realType(realType) {
}

IncompleteType::IncompleteType(std::string name) :
    name(name) {
}


} // namespace astfri
