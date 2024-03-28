#include <libastfri/structures/Declaration.hpp>
#include <libastfri/structures/Expression.hpp>
#include <libastfri/structures/Statement.hpp>
#include <libastfri/structures/Type.hpp>

#include <libastfri/utils/Helper.hpp>

#include <libastfri-output/PseudocodeVisitor.hpp>

namespace lsfs = libastfri::structures;

namespace libastfri::output
{
PseudocodeVisitor::PseudocodeVisitor(libastfri::utils::IOutputWriter& writer) :
    writer(writer)
{
}

void PseudocodeVisitor::Output(
    structures::TranslationUnit const& translationUnit
)
{
    this->Visit(translationUnit);
}

utils::IOutputWriter& PseudocodeVisitor::getWriter()
{
    return this->writer;
}

// smtmt
void PseudocodeVisitor::Visit(lsfs::TranslationUnit const& translationUnit)
{
    // declarations
    for (auto functionDef : translationUnit.functions)
    {
        functionDef->OutputVisitable::accept(*this);
    }
}

void PseudocodeVisitor::Visit(lsfs::CompoundStatement const& stmt)
{
    this->writer.printOnNewLine("{", false);
    this->writer.printEndl(false);
    this->writer.indentIncress();
    for (auto statement : stmt.statements)
    {
        statement->accept(*this);
    }
    this->writer.indentDecress();
    this->writer.printOnNewLine("}", false);
    this->writer.printEndl(false);
}

void PseudocodeVisitor::Visit(lsfs::DeclarationStatement const& stmt)
{
    this->writer.printIndent();
    stmt.declaration->accept(*this);
    this->writer.printEndl(false);
}

void PseudocodeVisitor::Visit(lsfs::DeclarationAndAssigmentStatement const& stmt
)
{
    this->writer.printIndent();
    stmt.declaration->accept(*this);
    this->writer.print(" <= ");
    stmt.expression->accept(*this);
    this->writer.printEndl(false);
}

void PseudocodeVisitor::Visit(lsfs::ReturnStatement const& stmt)
{
    this->writer.printIndent();
    this->writer.print("Vráť ");
    stmt.value->accept(*this);
    this->writer.printEndl(false);
}

void PseudocodeVisitor::Visit(lsfs::ExpressionStatement const& stmt)
{
    this->writer.printIndent();
    stmt.expression->accept(*this);
    this->writer.printEndl(false);
}

void PseudocodeVisitor::Visit(lsfs::UnknownStatement const& stmt)
{
    this->writer.printIndent();
    this->writer.print("UNKNOW STATEMNT (" + stmt.message + ")");
    this->writer.printEndl(false);
}

void PseudocodeVisitor::Visit(lsfs::IfStatement const& stmt)
{
    this->writer.printIndent();
    this->writer.print("Ak (");
    stmt.condition->accept(*this);
    this->writer.print(") potom");
    this->writer.printEndl(false);
    stmt.thenBody->accept(*this);
    if (stmt.elseBody != nullptr)
    {
        this->writer.printIndent();
        this->writer.print("Inak");
        this->writer.printEndl(false);
        stmt.elseBody->accept(*this);
    }
}

void PseudocodeVisitor::Visit(lsfs::WhileLoopStatement const& stmt)
{
    this->writer.printIndent();
    this->writer.print("Pokiaľ (");
    stmt.condition->accept(*this);
    this->writer.print(")\n");
    stmt.body->accept(*this);
}

void PseudocodeVisitor::Visit(lsfs::ForLoopStatement const& stmt)
{
    this->writer.printIndent();
    this->writer.print("Pokiaľ (");

    // init
    this->writer.startInlinePrinting();
    stmt.init->accept(*this);
    this->writer.endInlinePrinting();
    this->writer.print("; ");

    //  condition
    stmt.condition->accept(*this);
    this->writer.print("; ");

    // step
    stmt.step->accept(*this);
    this->writer.print(")");
    this->writer.printEndl(false);

    // body
    stmt.body->accept(*this);
}

// decl
void PseudocodeVisitor::Visit(lsfs::FunctionDefinition const& functionDef)
{
    // function signature
    this->writer.print("operáca " + functionDef.name + "(");
    for (int i = 0; i < functionDef.parameters.size(); i++)
    {
        functionDef.parameters[i]->accept(*this);
        if (i < functionDef.parameters.size() - 1)
        {
            this->writer.print(", ");
        }
    }
    
    // retur type
    this->writer.print(") : ");
    functionDef.returnType->accept(*this);
    
    this->writer.printEndl(false);

    // function body
    functionDef.body->accept(*this);
}

void PseudocodeVisitor::Visit(lsfs::ParameterDefinition const& decl)
{
    this->writer.print(decl.name + " : ");
    decl.type->accept(*this);
}

void PseudocodeVisitor::Visit(lsfs::VariableDefintion const& decl)
{
    this->writer.print("definuj premennú " + decl.name + " : ");
    decl.type->accept(*this);
}

// expr

void PseudocodeVisitor::Visit(lsfs::IntLiteral const& expr)
{
    this->writer.print(std::to_string(expr.value));
}

void PseudocodeVisitor::Visit(lsfs::BinaryExpression const& expr)
{
    expr.left->accept(*this);
    this->writer.print(
        " " + utils::Helper::convertBinaryOperator(expr.op) + " "
    );
    expr.right->accept(*this);
}

void PseudocodeVisitor::Visit(lsfs::UnaryExpression const& expr)
{
    this->writer.print(utils::Helper::convertUnaryOperator(expr.op));
    expr.arg->accept(*this);
}

void PseudocodeVisitor::Visit(lsfs::VarRefExpression const& expr)
{
    this->writer.print(expr.variable->name);
}

void PseudocodeVisitor::Visit(lsfs::FunctionCallExpression const& expr)
{
    this->writer.print(expr.functionName + "(");
    for (int i = 0; i < expr.arguments.size(); i++)
    {
        expr.arguments[i]->accept(*this);
        if (i < expr.arguments.size() - 1)
        {
            this->writer.print(", ");
        }
    }
    this->writer.print(")");
}

void PseudocodeVisitor::Visit(lsfs::UnknownExpression const& expr)
{
    this->writer.print("UNKNOW EXPR (" + expr.message + ")");
}

// type
void PseudocodeVisitor::Visit(lsfs::IntType const& type)
{
    this->writer.print(type.name);
}

} // namespace libastfri::output