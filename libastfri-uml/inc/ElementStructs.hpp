#pragma once

#include <string>

namespace uml {
    // w.i.p. with just a single generic param, TODO
    struct ClassStruct {
        std::string name_;
        std::string genericParam_;
    };
    
    struct VarStruct {
        std::string name_;
        std::string type_;
        std::string init_;
    };

    // w.i.p. with just a single param, TODO
    struct MethodStruct {
        std::string name_;
        std::string retType_;
        VarStruct param_;
    };
} // namespace uml