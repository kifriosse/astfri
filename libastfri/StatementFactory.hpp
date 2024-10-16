#ifndef LIBASTFRI_STATEMENT_FACTORY_HPP
#define LIBASTFRI_STATEMENT_FACTORY_HPP

#include <libastfri/Statement.hpp>

#include <vector>

namespace astfri
{
class StatementFactory
{
public:
    static StatementFactory& getInstance();

public:
    ~StatementFactory();

private:
    std::vector<Stmt*> stmts_;
};
} // namespace astfri

#endif