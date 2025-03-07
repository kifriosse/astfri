#pragma once

#include <libastfri-uml/inc/ClassVisitor.hpp>
#include <libastfri-uml/inc/PlantUMLOutputter.hpp>
#include "libastfri/inc/Stmt.hpp"

namespace uml {
    class UMLLibWrapper {
    private:
        ClassVisitor classVisitor_;
    public:
        void init(Config& config, UMLOutputter& output, TypeConvention const& typeConvention);
        void run(astfri::TranslationUnit& translationUnit);
    };
} // namespace uml