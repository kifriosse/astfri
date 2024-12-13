#include <iostream>
#include <libastfri/inc/ExprFactory.hpp>
#include <libastfri/inc/StmtFactory.hpp>
#include <libastfri/inc/TypeFactory.hpp>
#include <libastfri/inc/Visitor.hpp>

struct CodeVisitor : astfri::VisitorAdapter {
    int a = 6;
    int getA() {
        return  a;
    }
};