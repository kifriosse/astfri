#pragma once

#include <string>
#include <vector>
#include "libastfri/inc/Stmt.hpp"

namespace uml {
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
} // namespace uml