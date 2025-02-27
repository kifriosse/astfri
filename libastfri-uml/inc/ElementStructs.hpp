#pragma once

#include <string>
#include <vector>
#include "libastfri/inc/Stmt.hpp"

namespace uml {
    enum class RelationType {
        ASSOCIATION = 0,
        COMPOSITION,
        SPECIALIZATION
    };

    struct ClassStruct {
        std::string name_;
        std::vector<std::string> genericParams_;
    };

    struct VarStruct {
        std::string name_;
        std::string type_;
        std::string init_;
        astfri::AccessModifier accessMod_;
    };

    struct MethodStruct {
        std::string name_;
        std::string retType_;
        std::vector<VarStruct> params_;
        astfri::AccessModifier accessMod_;
    };

    struct RelationStruct {
        std::string from_;
        std::string to_;
        RelationType type_;
    };
} // namespace uml