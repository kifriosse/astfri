#include <astfri-ted/Config.hpp>
#include <astfri-common/RapidjsonUtils.hpp>

#include <stdexcept>

namespace astfri::ted {

Config Config::create_default() {
    return Config();
}

Config Config::create_from_json(const std::filesystem::path& path) {
    return Config::create_from_json(common::read_document(path));
}

AttributeConfig Config::read_attributes(const rapidjson::Value& node) {
    AttributeConfig retConfig;

    // Expr
    const rapidjson::Value& intLiteralExpr = common::get_object(node, "IntLiteralExpr");
    retConfig.intLiteralVal = common::get_bool(intLiteralExpr, "val");

    const rapidjson::Value& floatLiteralExpr = common::get_object(node, "FloatLiteralExpr");
    retConfig.floatLiteralVal = common::get_bool(floatLiteralExpr, "val");

    const rapidjson::Value& charLiteralExpr = common::get_object(node, "CharLiteralExpr");
    retConfig.charLiteralVal = common::get_bool(charLiteralExpr, "val");

    const rapidjson::Value& stringLiteralExpr = common::get_object(node, "StringLiteralExpr");
    retConfig.stringLiteralVal = common::get_bool(stringLiteralExpr, "val");

    const rapidjson::Value& boolLiteralExpr = common::get_object(node, "BoolLiteralExpr");
    retConfig.boolLiteralVal = common::get_bool(boolLiteralExpr, "val");

    const rapidjson::Value& binOpExpr = common::get_object(node, "BinOpExpr");
    retConfig.binOpOp = common::get_bool(binOpExpr, "op");

    const rapidjson::Value& unaryOpExpr = common::get_object(node, "UnaryOpExpr");
    retConfig.unaryOpOp = common::get_bool(unaryOpExpr, "op");

    const rapidjson::Value& paramVarRefExpr = common::get_object(node, "ParamVarRefExpr");
    retConfig.paramVarRefName = common::get_bool(paramVarRefExpr, "name");

    const rapidjson::Value& localVarRefExpr = common::get_object(node, "LocalVarRefExpr");
    retConfig.localVarRefName = common::get_bool(localVarRefExpr, "name");

    const rapidjson::Value& memberVarRefExpr = common::get_object(node, "MemberVarRefExpr");
    retConfig.memberVarRefName = common::get_bool(memberVarRefExpr, "name");

    const rapidjson::Value& globalVarRefExpr = common::get_object(node, "GlobalVarRefExpr");
    retConfig.globalVarRefName = common::get_bool(globalVarRefExpr, "name");

    const rapidjson::Value& classRefExpr = common::get_object(node, "ClassRefExpr");
    retConfig.classRefName = common::get_bool(classRefExpr, "name");

    const rapidjson::Value& functionCallExpr = common::get_object(node, "FunctionCallExpr");
    retConfig.functionCallName = common::get_bool(functionCallExpr, "name");

    const rapidjson::Value& methodCallExpr = common::get_object(node, "MethodCallExpr");
    retConfig.methodCallName = common::get_bool(methodCallExpr, "name");

    // Stmt
    const rapidjson::Value& localVarDefStmt = common::get_object(node, "LocalVarDefStmt");
    retConfig.localVarDefName = common::get_bool(localVarDefStmt, "name");

    const rapidjson::Value& paramVarDefStmt = common::get_object(node, "ParamVarDefStmt");
    retConfig.paramVarDefName = common::get_bool(paramVarDefStmt, "name");

    const rapidjson::Value& globalVarDefStmt = common::get_object(node, "GlobalVarDefStmt");
    retConfig.globalVarDefName = common::get_bool(globalVarDefStmt, "name");

    const rapidjson::Value& memberVarDefStmt = common::get_object(node, "MemberVarDefStmt");
    retConfig.memberVarDefName      = common::get_bool(memberVarDefStmt, "name");
    retConfig.memberVarDefAccess    = common::get_bool(memberVarDefStmt, "access");
    retConfig.memberVarDefStaticity = common::get_bool(memberVarDefStmt, "staticity");

    const rapidjson::Value& functionDefStmt = common::get_object(node, "FunctionDefStmt");
    retConfig.functionDefName = common::get_bool(functionDefStmt, "name");

    const rapidjson::Value& methodDefStmt = common::get_object(node, "MethodDefStmt");
    retConfig.methodDefAccess     = common::get_bool(methodDefStmt, "access");
    retConfig.methodDefVirtuality = common::get_bool(methodDefStmt, "virtuality");
    retConfig.methodDefStaticity  = common::get_bool(methodDefStmt, "staticity");

    const rapidjson::Value& constructorDefStmt = common::get_object(node, "ConstructorDefStmt");
    retConfig.constructorDefAccess = common::get_bool(constructorDefStmt, "access");

    const rapidjson::Value& genericParam = common::get_object(node, "GenericParam");
    retConfig.genericParamName       = common::get_bool(genericParam, "name");
    retConfig.genericParamConstraint = common::get_bool(genericParam, "constraint");

    const rapidjson::Value& classDefStmt = common::get_object(node, "ClassDefStmt");
    retConfig.classDefName = common::get_bool(classDefStmt, "name");

    const rapidjson::Value& interfaceDefStmt = common::get_object(node, "InterfaceDefStmt");
    retConfig.interfaceDefName = common::get_bool(interfaceDefStmt, "name");

    const rapidjson::Value& baseInitializerStmt = common::get_object(node, "BaseInitializerStmt");
    retConfig.baseInitializerName = common::get_bool(baseInitializerStmt, "name");

    const rapidjson::Value& memberInitializerStmt = common::get_object(node, "MemberInitializerStmt");
    retConfig.memberInitializerName = common::get_bool(memberInitializerStmt, "name");

    // Type
    const rapidjson::Value& classType = common::get_object(node, "ClassType");
    retConfig.classTypeName  = common::get_bool(classType, "name");
    retConfig.classTypeScope = common::get_bool(classType, "scope");

    const rapidjson::Value& interfaceType = common::get_object(node, "InterfaceType");
    retConfig.interfaceTypeName  = common::get_bool(interfaceType, "name");
    retConfig.interfaceTypeScope = common::get_bool(interfaceType, "scope");

    const rapidjson::Value& lambdaType = common::get_object(node, "LambdaType");
    retConfig.lambdaTypeName = common::get_bool(lambdaType, "name");

    const rapidjson::Value& incompleteType = common::get_object(node, "IncompleteType");
    retConfig.incompleteTypeName = common::get_bool(incompleteType, "name");

    return retConfig;
}

Config Config::create_from_json(const rapidjson::Value& node) {
    Config config;
    Costs costs;

    const rapidjson::Value& costsNode = common::get_object(node, "costs");

    costs.remove = common::get_int(costsNode, "remove");
    costs.insert = common::get_int(costsNode, "insert");
    costs.rename = common::get_int(costsNode, "rename");

    config.costs = costs;
    config.attributes = read_attributes(common::get_object(node, "attributes"));

    return config;
}

Config Config::create_from_args(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    throw std::logic_error("create_from_args: not implemented yet!");
}

void Config::write_json(const std::filesystem::path& path) const {
    (void)path;
    throw std::logic_error("write_json: not implemented yet!");
}



} // namespace astfri::ted