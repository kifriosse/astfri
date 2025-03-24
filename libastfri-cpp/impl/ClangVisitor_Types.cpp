#include <libastfri-cpp/inc/ClangVisitor.hpp>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/Type.h>
#include <clang/AST/TypeLoc.h>
#include <clang/Basic/Specifiers.h>
#include <clang/Frontend/FrontendActions.h>
#include <llvm-18/llvm/Support/Casting.h>
#include "libastfri/inc/Type.hpp"

namespace astfri::astfri_cpp 
{

clang::BuiltinType::Kind get_clang_type_from_builtin(const clang::BuiltinType* builtin) {
    // malo by prejst vzdy
    if (builtin->isVoidType()) {
        return clang::BuiltinType::Void;
    }
    if (builtin->isBooleanType()) {
        return clang::BuiltinType::Bool;
    }
    if (builtin->isFloatingType()) {
        return clang::BuiltinType::Float;
    }
    if (builtin->isCharType()) {
        // vraciam specificky char8, lebo nieje obycajny char, len jeho podtypy, tak som si vybral char8
        return clang::BuiltinType::Char8;
    }
    if (builtin->isIntegerType()) {
        return clang::BuiltinType::Int;
    }
    // nieco je zle ak som sa dostal sem
    std::cout << "\n\n\nError in finding builtin type: astfri_cpp project\n\n\n";
    return clang::BuiltinType::Char32;
}
astfri::Type* ClangVisitor::get_astfri_type_from_clang_builtintype(const clang::BuiltinType* builtin) {
    switch (get_clang_type_from_builtin(builtin)) {
        case clang::BuiltinType::Void: {
            //std::cout << "Void\n\n\n";
            return this->type_factory_->mk_void();
        } break;
        case clang::BuiltinType::Bool: {
            //std::cout << "Bool\n\n\n";
            return this->type_factory_->mk_bool();
        } break;
        case clang::BuiltinType::Char8: {
            //std::cout << "Char\n\n\n";
            return this->type_factory_->mk_char();
        } break;
        case clang::BuiltinType::Float: {
            //std::cout << "Float\n\n\n";
            return this->type_factory_->mk_float();
        } break;
        case clang::BuiltinType::Int: {
            //std::cout << "Int\n\n\n";
            return this->type_factory_->mk_int();
        } break;
        default: return this->type_factory_->mk_unknown(); break;
    }
}
// rekurzivna fukncia na naplnenie astfri::typu
astfri::Type* ClangVisitor::get_astfri_pointee(const clang::PointerType* pointer) {

    auto pointee = pointer->getPointeeType().getTypePtr();

    // ak je to builtin, koniec rekurzie
    if (auto builtin_pointee = llvm::dyn_cast<clang::BuiltinType>(pointee)) {
        return this->get_astfri_type_from_clang_builtintype(builtin_pointee);
    }

    // ak je to Record, koniec rekurzie
    if (pointee->isRecordType()) {
        auto record = pointee->getAsCXXRecordDecl();
        return this->type_factory_->mk_user(record->getNameAsString());
    }

    // ak je to template
    if (pointee->isTemplateTypeParmType()) {
        auto template_type = pointee->getContainedAutoType();
        return this->type_factory_->mk_user(template_type->getTypeClassName());
    }

    // ak je pointee pointer
    if (auto pointer_as_pointee = llvm::dyn_cast<clang::PointerType>(pointee)) {
        return this->type_factory_->mk_indirect(this->get_astfri_pointee(pointer_as_pointee));
    }

    std::cout << "Error in returning pointee in project astfri_cpp\n";
    return nullptr;
}
astfri::Type* ClangVisitor::get_astfri_type(clang::QualType QT) {

    auto clangType = QT.getTypePtr();

    // ak je to buildin tak sa vrati rovno to
    if (auto builtin = llvm::dyn_cast<clang::BuiltinType>(clangType)) {
        auto type =  this->get_astfri_type_from_clang_builtintype(builtin);
        return type;
    }

    // ak je to Record tak sa vrati rovno to
    if (clangType->isRecordType()) {
        auto record = clangType->getAsCXXRecordDecl();
        auto type =  this->type_factory_->mk_user(record->getNameAsString());
        return type;
    }

    // ak je to template tak sa vrati rovno to
    if (auto template_type = llvm::dyn_cast<clang::TemplateTypeParmType>(clangType)) {
        auto type =  this->type_factory_->mk_user(template_type->getDecl()->getNameAsString());
        return type;
    }

    // ak je to pointer, bude sa spustat rekurzivne pokym bude pointer
    if (auto pointer = llvm::dyn_cast<clang::PointerType>(clangType)) {
        return this->type_factory_->mk_indirect(this->get_astfri_pointee(pointer));
    }

    std::cout << "Error in finding builtin type: astfri_cpp project\n";
    return nullptr;
}

} // namespace astfri_cpp