#ifndef ASTFRI_TED_IMPL_ATTRIBUTE_CONFIG_HPP
#define ASTFRI_TED_IMPL_ATTRIBUTE_CONFIG_HPP

namespace astfri::ted {
    
struct AttributeConfig {
    // Expr
    bool intLiteralVal          = true;
    bool floatLiteralVal        = true;
    bool charLiteralVal         = true;
    bool stringLiteralVal       = true;
    bool boolLiteralVal         = true;
    bool binOpOp                = true;
    bool unaryOpOp              = true;
    bool paramVarRefName        = false;
    bool localVarRefName        = false;
    bool memberVarRefName       = false;
    bool globalVarRefName       = false;
    bool classRefName           = false;
    bool functionCallName       = false;
    bool methodCallName         = false;
    // Stmt
    bool localVarDefName        = false;
    bool paramVarDefName        = false;
    bool globalVarDefName       = false;
    bool memberVarDefName       = false;
    bool memberVarDefAccess     = true;
    bool memberVarDefStaticity  = true;
    bool functionDefName        = false;
    bool methodDefAccess        = true;
    bool methodDefVirtuality    = true;
    bool methodDefStaticity     = true;
    bool constructorDefAccess   = true;
    bool genericParamName       = false;
    bool genericParamConstraint = false;
    bool classDefName           = false;
    bool interfaceDefName       = false;
    bool baseInitializerName    = false;
    bool memberInitializerName  = false;
    // Type
    bool classTypeName          = false;
    bool classTypeScope         = false;
    bool interfaceTypeName      = false;
    bool interfaceTypeScope     = false;
    bool lambdaTypeName         = false;
    bool incompleteTypeName     = false;
};

} // namespace astfri::ted

#endif

