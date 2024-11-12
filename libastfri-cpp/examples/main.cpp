#include <clang-c/Index.h>
#include <iostream>
#include "clang-c/CXString.h"
//#include <string>
//#include "libastfri/inc/Stmt.hpp"
//#include <unordered_set>
//#include "libastfri/inc/Expr.hpp"
//#include "libastfri/inc/Stmt.hpp"

#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>

void printCursorInfo(CXCursor cursor, CXClientData client_data) {
    CXString cursorKind = clang_getCursorKindSpelling(clang_getCursorKind(cursor));
    CXString cursorSpelling = clang_getCursorSpelling(cursor);
    CXString cursorType = clang_getTypeSpelling(clang_getCursorType(cursor));
    std::cout << "Cursor kind: " << clang_getCString(cursorKind) << ", spelling: " << clang_getCString(cursorSpelling) << ", type: " << clang_getCString(cursorType) << std::endl;
    clang_disposeString(cursorKind);
    clang_disposeString(cursorSpelling);
}

CXChildVisitResult visitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {
    printCursorInfo(cursor, client_data);
    return CXChildVisit_Recurse;
}


int main(int argc, char** argv) {

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <source-file>" << std::endl;
        return 1;
    }

    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit unit = clang_parseTranslationUnit(index, argv[1], nullptr, 0, nullptr, 0, CXTranslationUnit_None);

    if (unit == nullptr) {
        std::cerr << "Unable to parse translation unit. Quitting." << std::endl;
        return 1;
    }

    CXCursor cursor = clang_getTranslationUnitCursor(unit);
    clang_visitChildren(cursor, visitor, nullptr);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);


    // auto& StmtFactory = astfri::StmtFactory::getInstance();
    // auto& ExprFactory = astfri::ExprFactory::getInstance();
    // auto& TypeFactory = astfri::TypeFactory::get_instance();

    // auto* funkcia = StmtFactory.mk_function_def("spocitaj",
    // std::vector<astfri::ParamVarDefStmt *> {},
    // TypeFactory.mk_int(),
    // StmtFactory.mk_compound(std::vector<astfri::Stmt *> {
    //     StmtFactory.mk_if(
    //         ExprFactory.mk_int_literal(1),
    //         StmtFactory.mk_return(ExprFactory.mk_int_literal(1)),
    //         StmtFactory.mk_return(ExprFactory.mk_int_literal(0))
    //     )
    // }));

    // auto* transUnit = StmtFactory.mk_translation_unit(
    //     std::vector<astfri::ClassDefStmt*> {},
    //     std::vector<astfri::FunctionDefStmt*> { funkcia },
    //     std::vector<astfri::GlobalVarDefStmt*> {});

    return 0;

}