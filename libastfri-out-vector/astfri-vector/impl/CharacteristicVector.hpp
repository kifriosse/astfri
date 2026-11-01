#ifndef LIBASTFRI_OUT_VECTOR_CHARACTERISTIC_VECTOR_HPP
#define LIBASTFRI_OUT_VECTOR_CHARACTERISTIC_VECTOR_HPP

#include <vector>

namespace astfri::vector {

enum class VectorComponent : size_t {
    ClassOrInterface = 0, // ClassDefStmt, InterfaceDefStmt
    CallableDef,          // FunctionDefStmt, ConstructorDefStmt, DestructorDefStmt, LambdaExpr
    VarDef,               // LocalVarDefStmt, ParamVarDefStmt, MemberVarDefStmt, GlobalVarDefStmt
    VarRef,               // LocalVarRefExpr, ParamVarRefExpr, MemberVarRefExpr, GlobalVarRefExpr
    Literal,              // IntLiteralExpr, FloatLiteralExpr, CharLiteralExpr, StringLiteralExpr, BoolLiteralExpr, NullLiteralExpr
    AssignOp,             // BinOpExpr: =, +=, -=, *=, /=, %=, &=, |=, ^=, <<=, >>=
    ArithmeticOp,         // BinOpExpr: +, -, *, /, %, ** | UnaryOpExpr: +, -
    IncrementOp,          // UnaryOpExpr: ++, -- (prefix and postfix)
    ComparisonOp,         // BinOpExpr: ==, !=, <, <=, >, >=
    LogicalBitOp,         // BinOpExpr: &&, ||, &, |, ^, <<, >> | UnaryOpExpr: !, ~
    BranchIf,             // IfStmt, IfExpr (ternary operator ?:)
    BranchSwitch,         // SwitchStmt, CaseStmt, DefaultCaseStmt
    LoopFor,              // ForStmt, ForEachStmt
    LoopWhile,            // WhileStmt, DoWhileStmt
    Call,                 // FunctionCallExpr, MethodCallExpr, ConstructorCallExpr, LambdaCallExpr, Base/SelfInitializerStmt
    Jump,                 // ReturnStmt, BreakStmt, ContinueStmt
    Exception,            // TryStmt, CatchStmt, ThrowStmt
    MemoryOp,             // NewExpr, DeleteExpr | UnaryOpExpr: * (dereference), & (address-of)
};

class CharacteristicVector {
public:
    static constexpr size_t VECTOR_SIZE = 18;    

public:
    CharacteristicVector();

    void increment(VectorComponent comp);
    int get(VectorComponent comp) const;
    int total_token() const;

    CharacteristicVector& operator+=(const CharacteristicVector& rhs);
    CharacteristicVector operator+(const CharacteristicVector& rhs) const;
    bool operator==(const CharacteristicVector& rhs) const;
    bool operator!=(const CharacteristicVector& rhs) const;

private:
    std::vector<int> m_characteristicVector;
};

}

#endif