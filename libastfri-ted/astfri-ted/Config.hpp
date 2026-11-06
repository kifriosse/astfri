#ifndef ASTFRI_TED_CONFIG_HPP
#define ASTFRI_TED_CONFIG_HPP

#include <astfri-ted/Costs.hpp>
#include <astfri/impl/Concepts.hpp>

#include <rapidjson/document.h>

#include <filesystem>

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

struct Config {
    Costs costs;
    AttributeConfig attributes;

public:
    static Config create_default();
    static Config create_from_json(const rapidjson::Value& node);
    static Config create_from_json(const std::filesystem::path& path);
    static Config create_from_args(int argc, char* argv[]);

    Config(const Config& other) = default;
    void write_json(const std::filesystem::path& path) const;

private:
    Config() = default;

    static AttributeConfig read_attributes(const rapidjson::Value& node);
};

static_assert(astfri::detail::IsConfig<astfri::ted::Config, rapidjson::Value>);

} // namespace astfri::ted

#endif
