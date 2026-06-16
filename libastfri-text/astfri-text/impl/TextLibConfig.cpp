#include <astfri-text/TextLibConfig.hpp>
#include <astfri-common/RapidjsonUtils.hpp>

#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>


namespace astfri::text {


Config Config::createFromJson(const rapidjson::Value &node) {
    Config config;
    config.read_code_structure(common::get_object(node, "codeStructure"));
    config.read_pseudocode_structure(common::get_object(node, "pseudocodeStructure"));
    config.read_pseudocode_text(common::get_object(node, "pseudocodeText"));
    config.read_output_settings(common::get_object(node, "outputSettings"));
    return config;
}

Config Config::createFromJson(std::filesystem::path const& path) {
    return Config::createFromJson(common::read_document(path));
}

Config Config::createDefault() {
    Config config;

    // 1) CODE_STRUCTURE
    config.tabulatorLength              = 4;
    config.namespaceBlockBracketNewLine = false;
    config.useNamespaceTabulator        = false;
    config.objectBlockBracketNewLine    = false;
    config.functionBlockBracketNewLine  = false;
    config.loopBlockBracketNewLine      = false;
    config.dowhileConditionNewLine      = false;
    config.conditionBlockBracketNewLine = false;
    config.elseifConditionNewLine       = false;
    config.elseConditionNewLine         = false;
    config.switchBlockBracketNewLine    = false;
    config.trycatchBlockBracketNewLine  = false;
    config.catchConditionNewLine        = false;
    // 2) PSEUDOCODE_STRUCTURE
    config.textMarginLeft     = 3;
    config.rowNumMarginLeft   = 1;
    config.shBrColors         = true;
    config.shRowNum           = true;
    config.shDotAfterRowNum   = true;
    config.shRowNumOnEmptyRow = true;
    config.shGlobVarDeclar    = true;
    config.shTemplateDeclar   = true;
    config.shClassDeclar      = true;
    config.shClassDefin       = true;
    config.shClassDefinInl    = false;
    config.shInterfDeclar     = true;
    config.shInterfDefin      = true;
    config.shMembVarDeclar    = true;
    config.shCoDeMeDeclar     = true;
    config.shCoDeMeDefin      = true;
    config.shCoDeMeOwner      = true;
    config.shCoDeMeTemplate   = true;
    config.shFuncDeclar       = true;
    config.shFuncDefin        = true;
    // 3) PSEUDOCODE_TEXT
    // 3.1 GENERAL
    config.unknownTypeWord    = "UNKNOWN TYPE";
    config.unknownExprWord    = "UNKNOWN EXPRESSION";
    config.unknownStmtWord    = "UNKNOWN STATEMENT";
    config.defaultTextStyle   = "font-family:Consolas;font-size:16px";
    config.unknownPhraseStyle = "";
    config.rowNumStyle        = "";
    // 3.2 SYMBOLS
    // 3.2.1 OPERATORS
    config.pointerWord = "↑";
    config.assignWord  = "=";
    config.moduloWord  = "%";
    config.addressWord = "&";
    config.derefWord   = "*";
    config.opWordStyle = "";
    config.brColors = {"red", "green", "blue"};
    // 3.2.2 SEPARATORS
    config.semicolonWord = ";";
    config.sepWordStyle  = "";
    // 3.3 VALUES
    config.trueWord          = "true";
    config.falseWord         = "false";
    config.nullWord          = "nullptr";
    config.valueStyle        = "";
    config.numericValueStyle = "";
    config.stringValueStyle  = "";
    // 3.4 REFERENCE_NAMES
    config.defaultRefNameStyle = "";
    config.templateNameStyle   = "";
    config.classNameStyle      = "";
    config.interfaceNameStyle  = "";
    config.methodNameStyle     = "";
    config.functionNameStyle   = "";
    config.defaultVarNameStyle = "";
    config.globalVarNameStyle  = "";
    config.memberVarNameStyle  = "";
    config.localVarNameStyle   = "";
    config.paramVarNameStyle   = "";
    // 3.5 SYSTEM_EXPRESSIONS
    // 3.5.1 ACCESS_MODIFIERS
    config.publicWord       = "public";
    config.protectedWord    = "protected";
    config.privateWord      = "private";
    config.internalWord     = "package-private";
    config.attributesWord   = "attributes";
    config.constructorsWord = "constructors";
    config.destructorsWord  = "destructors";
    config.methodsWord      = "methods";
    config.accessModifStyle = "";
    // 3.5.2 DATA_TYPES
    config.dynamicTypeWord      = "auto";
    config.intTypeWord          = "int";
    config.floatTypeWord        = "float";
    config.charTypeWord         = "char";
    config.boolTypeWord         = "bool";
    config.voidTypeWord         = "void";
    config.typeWordStyle        = "";
    config.numericTypeWordStyle = "";
    config.stringTypeWordStyle  = "";
    config.systemTypeWordStyle  = "";
    // 3.5.3 OBJECTS
    config.scopeWord       = "namespace";
    config.templateWord    = "template";
    config.classWord       = "class";
    config.interfaceWord   = "interface";
    config.implementWord   = "implements";
    config.extendWord      = "extends";
    config.virtualWord     = "virtual";
    config.abstractWord    = "abstract";
    config.staticWord      = "static";
    config.overrideWord    = "override";
    config.thisWord        = "this";
    config.objectWordStyle = "";
    // 3.5.4 CONDITIONS
    config.ifWord             = "if";
    config.elseifWord         = "else if";
    config.elseWord           = "else";
    config.switchWord         = "switch";
    config.caseWord           = "case";
    config.defaultWord        = "default";
    config.conditionWordStyle = "";
    // 3.5.5 LOOPS
    config.doWord        = "do";
    config.whileWord     = "while";
    config.forWord       = "for";
    config.foreachWord   = "for";
    config.loopWordStyle = "";
    // 3.5.6 OTHER
    config.returnWord     = "return";
    config.continueWord   = "continue";
    config.breakWord      = "break";
    config.tryWord        = "try";
    config.catchWord      = "catch";
    config.finallyWord    = "finally";
    config.throwWord      = "throw";
    config.newWord        = "new";
    config.deleteWord     = "delete";
    config.otherExprStyle = "";
    // 3.6 SUPPORT_EXPRESSIONS
    config.constructorWord  = "constructor";
    config.destructorWord   = "destructor";
    config.methodWord       = "method";
    config.functionWord     = "function";
    config.lambdaWord       = "λ";
    config.callWord         = "call";
    config.defineWord       = "define";
    config.returnsWord      = "returns ->";
    config.repeatWord       = "repeat";
    config.supportExprStyle = "";
    // 4) OUTPUT_SETTINGS
    config.fileName   = "output";
    config.filePath   = "default";
    config.fileFormat = "txt";

    return config;
}

Config Config::createFromArgs(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    throw std::runtime_error("Not implemented yet.");
}

void Config::write_json(rapidjson::Value &out, rapidjson::Document::AllocatorType &alloc) const {
    this->write_code_structure(common::add_object(out, alloc, "codeStructure"), alloc);
    this->write_pseudocode_structure(common::add_object(out, alloc, "pseudocodeStructure"), alloc);
    this->write_pseudocode_text(common::add_object(out, alloc, "pseudocodeText"), alloc);
    this->write_output_settings(common::add_object(out, alloc, "outputSettings"), alloc);
}

void Config::write_json_file(const std::filesystem::path &path) const {
    rapidjson::Document doc = common::create_document();
    this->write_json(doc, doc.GetAllocator());
    common::write_document(path, doc);
}

void Config::change_to_java_like() {
    // TODO MM:
}

void Config::change_to_cxx_like() {
    tabulatorLength              = 4;
    namespaceBlockBracketNewLine = true;
    useNamespaceTabulator        = true;
    objectBlockBracketNewLine    = true;
    functionBlockBracketNewLine  = true;
    loopBlockBracketNewLine      = true;
    dowhileConditionNewLine      = true;
    conditionBlockBracketNewLine = true;
    elseifConditionNewLine       = true;
    elseConditionNewLine         = true;
    switchBlockBracketNewLine    = true;
    trycatchBlockBracketNewLine  = true;
    catchConditionNewLine        = true;
}

void Config::read_code_structure(const rapidjson::Value &structure) {
    tabulatorLength              = common::get_int(structure, "tabulatorLength");
    namespaceBlockBracketNewLine = common::get_bool(structure, "namespaceBlockBracketNewLine");
    useNamespaceTabulator        = common::get_bool(structure, "useNamespaceTabulator");
    objectBlockBracketNewLine    = common::get_bool(structure, "objectBlockBracketNewLine");
    functionBlockBracketNewLine  = common::get_bool(structure, "functionBlockBracketNewLine");
    loopBlockBracketNewLine      = common::get_bool(structure, "loopBlockBracketNewLine");
    dowhileConditionNewLine      = common::get_bool(structure, "dowhileConditionNewLine");
    conditionBlockBracketNewLine = common::get_bool(structure, "conditionBlockBracketNewLine");
    elseifConditionNewLine       = common::get_bool(structure, "elseifConditionNewLine");
    elseConditionNewLine         = common::get_bool(structure, "elseConditionNewLine");
    switchBlockBracketNewLine    = common::get_bool(structure, "switchBlockBracketNewLine");
    trycatchBlockBracketNewLine  = common::get_bool(structure, "trycatchBlockBracketNewLine");
    catchConditionNewLine        = common::get_bool(structure, "catchConditionNewLine");
}

void Config::read_pseudocode_structure(const rapidjson::Value &structure) {
    textMarginLeft     = common::get_int(structure, "textMarginLeft");
    rowNumMarginLeft   = common::get_int(structure, "rowNumMarginLeft");
    shBrColors         = common::get_bool(structure, "shBrColors");
    shRowNum           = common::get_bool(structure, "shRowNum");
    shDotAfterRowNum   = common::get_bool(structure, "shDotAfterRowNum");
    shRowNumOnEmptyRow = common::get_bool(structure, "shRowNumOnEmptyRow");
    shGlobVarDeclar    = common::get_bool(structure, "shGlobVarDeclar");
    shTemplateDeclar   = common::get_bool(structure, "shTemplateDeclar");
    shClassDeclar      = common::get_bool(structure, "shClassDeclar");
    shClassDefin       = common::get_bool(structure, "shClassDefin");
    shClassDefinInl    = common::get_bool(structure, "shClassDefinInl");
    shInterfDeclar     = common::get_bool(structure, "shInterfDeclar");
    shInterfDefin      = common::get_bool(structure, "shInterfDefin");
    shMembVarDeclar    = common::get_bool(structure, "shMembVarDeclar");
    shCoDeMeDeclar     = common::get_bool(structure, "shCoDeMeDeclar");
    shCoDeMeDefin      = common::get_bool(structure, "shCoDeMeDefin");
    shCoDeMeOwner      = common::get_bool(structure, "shCoDeMeOwner");
    shCoDeMeTemplate   = common::get_bool(structure, "shCoDeMeTemplate");
    shFuncDeclar       = common::get_bool(structure, "shFuncDeclar");
    shFuncDefin        = common::get_bool(structure, "shFuncDefin");
}

void Config::read_pseudocode_text(const rapidjson::Value &text) {
    read_general_text(common::get_object(text, "general"));
    read_symbols(common::get_object(text, "symbols"));
    read_values(common::get_object(text, "values"));
    read_reference_names(common::get_object(text, "referenceNames"));
    read_system_expressions(common::get_object(text, "systemExpressions"));
    read_support_expressions(common::get_object(text, "supportExpressions"));
}

void Config::read_output_settings(const rapidjson::Value &settings) {
    fileName   = common::get_string(settings, "fileName");
    filePath   = common::get_string(settings, "filePath");
    fileFormat = common::get_string(settings, "fileFormat");
}

void Config::read_general_text(const rapidjson::Value &text) {
    unknownTypeWord    = common::get_string(text, "unknownTypeWord");
    unknownExprWord    = common::get_string(text, "unknownExprWord");
    unknownStmtWord    = common::get_string(text, "unknownStmtWord");
    defaultTextStyle   = common::get_string(text, "defaultTextStyle");
    unknownPhraseStyle = common::get_string(text, "unknownPhraseStyle");
    rowNumStyle        = common::get_string(text, "rowNumStyle");
}

void Config::read_symbols(const rapidjson::Value &symbols) {
    const rapidjson::Value &operators = common::get_object(symbols, "operators");
    const rapidjson::Value &separators = common::get_object(symbols, "separators");
    pointerWord = common::get_string(operators, "pointerWord");
    assignWord  = common::get_string(operators, "assignWord");
    moduloWord  = common::get_string(operators, "moduloWord");
    addressWord = common::get_string(operators, "addressWord");
    derefWord   = common::get_string(operators, "derefWord");
    opWordStyle = common::get_string(operators, "opWordStyle");
    std::vector<const rapidjson::Value*> bc = common::get_array(operators, "brColors");
    for (const rapidjson::Value *cVal : bc) {
        brColors.push_back(common::as_string(*cVal));
    }
    semicolonWord = common::get_string(separators, "semicolonWord");
    sepWordStyle  = common::get_string(separators, "sepWordStyle");
}

void Config::read_values(const rapidjson::Value &values) {
    trueWord          = common::get_string(values, "trueWord");
    falseWord         = common::get_string(values, "falseWord");
    nullWord          = common::get_string(values, "nullWord");
    valueStyle        = common::get_string(values, "valueStyle");
    numericValueStyle = common::get_string(values, "numericValueStyle");
    stringValueStyle  = common::get_string(values, "stringValueStyle");
}

void Config::read_reference_names(const rapidjson::Value &names) {
    defaultRefNameStyle = common::get_string(names, "defaultRefNameStyle");
    templateNameStyle   = common::get_string(names, "templateNameStyle");
    classNameStyle      = common::get_string(names, "classNameStyle");
    interfaceNameStyle  = common::get_string(names, "interfaceNameStyle");
    methodNameStyle     = common::get_string(names, "methodNameStyle");
    functionNameStyle   = common::get_string(names, "functionNameStyle");
    defaultVarNameStyle = common::get_string(names, "defaultVarNameStyle");
    globalVarNameStyle  = common::get_string(names, "globalVarNameStyle");
    memberVarNameStyle  = common::get_string(names, "memberVarNameStyle");
    localVarNameStyle   = common::get_string(names, "localVarNameStyle");
    paramVarNameStyle   = common::get_string(names, "paramVarNameStyle");
}

void Config::read_system_expressions(const rapidjson::Value &expressions) {
    read_access_modifiers(common::get_object(expressions, "accessModifiers"));
    read_data_types(common::get_object(expressions, "dataTypes"));
    read_objects(common::get_object(expressions, "objects"));
    read_conditions(common::get_object(expressions, "conditions"));
    read_loops(common::get_object(expressions, "loops"));
    read_other(common::get_object(expressions, "other"));
}

void Config::read_access_modifiers(const rapidjson::Value &modifiers) {
    publicWord       = common::get_string(modifiers, "publicWord");
    protectedWord    = common::get_string(modifiers, "protectedWord");
    privateWord      = common::get_string(modifiers, "privateWord");
    internalWord     = common::get_string(modifiers, "internalWord");
    attributesWord   = common::get_string(modifiers, "attributesWord");
    constructorsWord = common::get_string(modifiers, "constructorsWord");
    destructorsWord  = common::get_string(modifiers, "destructorsWord");
    methodsWord      = common::get_string(modifiers, "methodsWord");
    accessModifStyle = common::get_string(modifiers, "accessModifStyle");
}

void Config::read_data_types(const rapidjson::Value &types) {
    dynamicTypeWord      = common::get_string(types, "dynamicTypeWord");
    intTypeWord          = common::get_string(types, "intTypeWord");
    floatTypeWord        = common::get_string(types, "floatTypeWord");
    charTypeWord         = common::get_string(types, "charTypeWord");
    boolTypeWord         = common::get_string(types, "boolTypeWord");
    voidTypeWord         = common::get_string(types, "voidTypeWord");
    typeWordStyle        = common::get_string(types, "typeWordStyle");
    numericTypeWordStyle = common::get_string(types, "numericTypeWordStyle");
    stringTypeWordStyle  = common::get_string(types, "stringTypeWordStyle");
    systemTypeWordStyle  = common::get_string(types, "systemTypeWordStyle");
}

void Config::read_objects(const rapidjson::Value &objects) {
    scopeWord       = common::get_string(objects, "scopeWord");
    templateWord    = common::get_string(objects, "templateWord");
    classWord       = common::get_string(objects, "classWord");
    interfaceWord   = common::get_string(objects, "interfaceWord");
    implementWord   = common::get_string(objects, "implementWord");
    extendWord      = common::get_string(objects, "extendWord");
    virtualWord     = common::get_string(objects, "virtualWord");
    abstractWord    = common::get_string(objects, "abstractWord");
    staticWord      = common::get_string(objects, "staticWord");
    overrideWord    = common::get_string(objects, "overrideWord");
    thisWord        = common::get_string(objects, "thisWord");
    objectWordStyle = common::get_string(objects, "objectWordStyle");
}

void Config::read_conditions(const rapidjson::Value &conditions) {
    ifWord             = common::get_string(conditions, "ifWord");
    elseifWord         = common::get_string(conditions, "elseifWord");
    elseWord           = common::get_string(conditions, "elseWord");
    switchWord         = common::get_string(conditions, "switchWord");
    caseWord           = common::get_string(conditions, "caseWord");
    defaultWord        = common::get_string(conditions, "defaultWord");
    conditionWordStyle = common::get_string(conditions, "conditionWordStyle");
}

void Config::read_loops(const rapidjson::Value &loops) {
    doWord        = common::get_string(loops, "doWord");
    whileWord     = common::get_string(loops, "whileWord");
    forWord       = common::get_string(loops, "forWord");
    foreachWord   = common::get_string(loops, "foreachWord");
    loopWordStyle = common::get_string(loops, "loopWordStyle");
}

void Config::read_other(const rapidjson::Value &other) {
    returnWord     = common::get_string(other, "returnWord");
    continueWord   = common::get_string(other, "continueWord");
    breakWord      = common::get_string(other, "breakWord");
    tryWord        = common::get_string(other, "tryWord");
    catchWord      = common::get_string(other, "catchWord");
    finallyWord    = common::get_string(other, "finallyWord");
    throwWord      = common::get_string(other, "throwWord");
    newWord        = common::get_string(other, "newWord");
    deleteWord     = common::get_string(other, "deleteWord");
    otherExprStyle = common::get_string(other, "otherExprStyle");
}

void Config::read_support_expressions(const rapidjson::Value &expressions) {
    constructorWord  = common::get_string(expressions, "constructorWord");
    destructorWord   = common::get_string(expressions, "destructorWord");
    methodWord       = common::get_string(expressions, "methodWord");
    functionWord     = common::get_string(expressions, "functionWord");
    lambdaWord       = common::get_string(expressions, "lambdaWord");
    callWord         = common::get_string(expressions, "callWord");
    defineWord       = common::get_string(expressions, "defineWord");
    returnsWord      = common::get_string(expressions, "returnsWord");
    repeatWord       = common::get_string(expressions, "repeatWord");
    supportExprStyle = common::get_string(expressions, "supportExprStyle");
}

void Config::write_code_structure(rapidjson::Value &structure, rapidjson::Document::AllocatorType &alloc) const {
    common::add_int(structure, alloc, "tabulatorLength", tabulatorLength);
    common::add_bool(structure, alloc, "namespaceBlockBracketNewLine", namespaceBlockBracketNewLine);
    common::add_bool(structure, alloc, "useNamespaceTabulator", useNamespaceTabulator);
    common::add_bool(structure, alloc, "objectBlockBracketNewLine", objectBlockBracketNewLine);
    common::add_bool(structure, alloc, "functionBlockBracketNewLine", functionBlockBracketNewLine);
    common::add_bool(structure, alloc, "loopBlockBracketNewLine", loopBlockBracketNewLine);
    common::add_bool(structure, alloc, "dowhileConditionNewLine", dowhileConditionNewLine);
    common::add_bool(structure, alloc, "conditionBlockBracketNewLine", conditionBlockBracketNewLine);
    common::add_bool(structure, alloc, "elseifConditionNewLine", elseifConditionNewLine);
    common::add_bool(structure, alloc, "elseConditionNewLine", elseConditionNewLine);
    common::add_bool(structure, alloc, "switchBlockBracketNewLine", switchBlockBracketNewLine);
    common::add_bool(structure, alloc, "trycatchBlockBracketNewLine", trycatchBlockBracketNewLine);
    common::add_bool(structure, alloc, "catchConditionNewLine", catchConditionNewLine);
}

void Config::write_pseudocode_structure(rapidjson::Value &structure, rapidjson::Document::AllocatorType &alloc) const {
    common::add_int(structure, alloc, "textMarginLeft", textMarginLeft);
    common::add_int(structure, alloc, "rowNumMarginLeft", rowNumMarginLeft);
    common::add_bool(structure, alloc, "shBrColors", shBrColors);
    common::add_bool(structure, alloc, "shRowNum", shRowNum);
    common::add_bool(structure, alloc, "shDotAfterRowNum", shDotAfterRowNum);
    common::add_bool(structure, alloc, "shRowNumOnEmptyRow", shRowNumOnEmptyRow);
    common::add_bool(structure, alloc, "shGlobVarDeclar", shGlobVarDeclar);
    common::add_bool(structure, alloc, "shTemplateDeclar", shTemplateDeclar);
    common::add_bool(structure, alloc, "shClassDeclar", shClassDeclar);
    common::add_bool(structure, alloc, "shClassDefin", shClassDefin);
    common::add_bool(structure, alloc, "shClassDefinInl", shClassDefinInl);
    common::add_bool(structure, alloc, "shInterfDeclar", shInterfDeclar);
    common::add_bool(structure, alloc, "shInterfDefin", shInterfDefin);
    common::add_bool(structure, alloc, "shMembVarDeclar", shMembVarDeclar);
    common::add_bool(structure, alloc, "shCoDeMeDeclar", shCoDeMeDeclar);
    common::add_bool(structure, alloc, "shCoDeMeDefin", shCoDeMeDefin);
    common::add_bool(structure, alloc, "shCoDeMeOwner", shCoDeMeOwner);
    common::add_bool(structure, alloc, "shCoDeMeTemplate", shCoDeMeTemplate);
    common::add_bool(structure, alloc, "shFuncDeclar", shFuncDeclar);
    common::add_bool(structure, alloc, "shFuncDefin", shFuncDefin);
}

void Config::write_pseudocode_text(rapidjson::Value &text, rapidjson::Document::AllocatorType &alloc) const {
    this->write_general_text(common::add_object(text, alloc, "general"), alloc);
    this->write_symbols(common::add_object(text, alloc, "symbols"), alloc);
    this->write_values(common::add_object(text, alloc, "values"), alloc);
    this->write_reference_names(common::add_object(text, alloc, "referenceNames"), alloc);
    this->write_system_expressions(common::add_object(text, alloc, "systemExpressions"), alloc);
    this->write_support_expressions(common::add_object(text, alloc, "supportExpressions"), alloc);
}

void Config::write_general_text(rapidjson::Value &text, rapidjson::Document::AllocatorType &alloc) const {
    common::add_string(text, alloc, "unknownTypeWord", unknownTypeWord);
    common::add_string(text, alloc, "unknownExprWord", unknownExprWord);
    common::add_string(text, alloc, "unknownStmtWord", unknownStmtWord);
    common::add_string(text, alloc, "defaultTextStyle", defaultTextStyle);
    common::add_string(text, alloc, "unknownPhraseStyle",unknownPhraseStyle);
    common::add_string(text, alloc, "rowNumStyle", rowNumStyle);
}

void Config::write_symbols(rapidjson::Value &symbols, rapidjson::Document::AllocatorType &alloc) const {
    rapidjson::Value &operators = common::add_object(symbols, alloc, "operators");
    common::add_string(operators, alloc, "pointerWord", pointerWord);
    common::add_string(operators, alloc, "assignWord", assignWord);
    common::add_string(operators, alloc, "moduloWord", moduloWord);
    common::add_string(operators, alloc, "addressWord", addressWord);
    common::add_string(operators, alloc, "derefWord", derefWord);
    common::add_string(operators, alloc, "opWordStyle", opWordStyle);
    common::add_array(operators, alloc, "brColors", brColors);

    rapidjson::Value &separators = common::add_object(symbols, alloc, "separators");
    common::add_string(separators, alloc, "semicolonWord", semicolonWord);
    common::add_string(separators, alloc, "sepWordStyle", sepWordStyle);
}

void Config::write_values(rapidjson::Value &values, rapidjson::Document::AllocatorType &alloc) const {
    common::add_string(values, alloc, "trueWord", trueWord);
    common::add_string(values, alloc, "falseWord", falseWord);
    common::add_string(values, alloc, "nullWord", nullWord);
    common::add_string(values, alloc, "valueStyle", valueStyle);
    common::add_string(values, alloc, "numericValueStyle", numericValueStyle);
    common::add_string(values, alloc, "stringValueStyle", stringValueStyle);
}

void Config::write_reference_names(rapidjson::Value &names, rapidjson::Document::AllocatorType &alloc) const {
    common::add_string(names, alloc, "defaultRefNameStyle", defaultRefNameStyle);
    common::add_string(names, alloc, "templateNameStyle", templateNameStyle);
    common::add_string(names, alloc, "classNameStyle", classNameStyle);
    common::add_string(names, alloc, "interfaceNameStyle", interfaceNameStyle);
    common::add_string(names, alloc, "methodNameStyle", methodNameStyle);
    common::add_string(names, alloc, "functionNameStyle", functionNameStyle);
    common::add_string(names, alloc, "defaultVarNameStyle", defaultVarNameStyle);
    common::add_string(names, alloc, "globalVarNameStyle", globalVarNameStyle);
    common::add_string(names, alloc, "memberVarNameStyle", memberVarNameStyle);
    common::add_string(names, alloc, "localVarNameStyle", localVarNameStyle);
    common::add_string(names, alloc, "paramVarNameStyle", paramVarNameStyle);
}

void Config::write_system_expressions(rapidjson::Value &expr, rapidjson::Document::AllocatorType &alloc) const {
    this->write_access_modifiers(common::add_object(expr, alloc, "accessModifiers"), alloc);
    this->write_data_types(common::add_object(expr, alloc, "dataTypes"), alloc);
    this->write_objects(common::add_object(expr, alloc, "objects"), alloc);
    this->write_conditions(common::add_object(expr, alloc, "conditions"), alloc);
    this->write_loops(common::add_object(expr, alloc, "loops"), alloc);
    this->write_other(common::add_object(expr, alloc, "other"), alloc);
}

void Config::write_access_modifiers(rapidjson::Value &modifiers, rapidjson::Document::AllocatorType& alloc) const {
    common::add_string(modifiers, alloc, "publicWord", publicWord);
    common::add_string(modifiers, alloc, "protectedWord", protectedWord);
    common::add_string(modifiers, alloc, "privateWord", privateWord);
    common::add_string(modifiers, alloc, "internalWord", internalWord);
    common::add_string(modifiers, alloc, "attributesWord", attributesWord);
    common::add_string(modifiers, alloc, "constructorsWord", constructorsWord);
    common::add_string(modifiers, alloc, "destructorsWord", destructorsWord);
    common::add_string(modifiers, alloc, "methodsWord", methodsWord);
    common::add_string(modifiers, alloc, "accessModifStyle", accessModifStyle);
}

void Config::write_data_types(rapidjson::Value &types, rapidjson::Document::AllocatorType& alloc) const {
    common::add_string(types, alloc, "dynamicTypeWord", dynamicTypeWord);
    common::add_string(types, alloc, "intTypeWord", intTypeWord);
    common::add_string(types, alloc, "floatTypeWord", floatTypeWord);
    common::add_string(types, alloc, "charTypeWord", charTypeWord);
    common::add_string(types, alloc, "boolTypeWord", boolTypeWord);
    common::add_string(types, alloc, "voidTypeWord", voidTypeWord);
    common::add_string(types, alloc, "typeWordStyle", typeWordStyle);
    common::add_string(types, alloc, "numericTypeWordStyle", numericTypeWordStyle);
    common::add_string(types, alloc, "stringTypeWordStyle", stringTypeWordStyle);
    common::add_string(types, alloc, "systemTypeWordStyle", systemTypeWordStyle);
}

void Config::write_objects(rapidjson::Value &objects, rapidjson::Document::AllocatorType& alloc) const {
    common::add_string(objects, alloc, "scopeWord", scopeWord);
    common::add_string(objects, alloc, "templateWord", templateWord);
    common::add_string(objects, alloc, "classWord", classWord);
    common::add_string(objects, alloc, "interfaceWord", interfaceWord);
    common::add_string(objects, alloc, "implementWord", implementWord);
    common::add_string(objects, alloc, "extendWord", extendWord);
    common::add_string(objects, alloc, "virtualWord", virtualWord);
    common::add_string(objects, alloc, "abstractWord", abstractWord);
    common::add_string(objects, alloc, "staticWord", staticWord);
    common::add_string(objects, alloc, "overrideWord", overrideWord);
    common::add_string(objects, alloc, "thisWord", thisWord);
    common::add_string(objects, alloc, "objectWordStyle", objectWordStyle);
}

void Config::write_conditions(rapidjson::Value &conditions, rapidjson::Document::AllocatorType& alloc) const {
    common::add_string(conditions, alloc, "ifWord", ifWord);
    common::add_string(conditions, alloc, "elseifWord", elseifWord);
    common::add_string(conditions, alloc, "elseWord", elseWord);
    common::add_string(conditions, alloc, "switchWord", switchWord);
    common::add_string(conditions, alloc, "caseWord", caseWord);
    common::add_string(conditions, alloc, "defaultWord", defaultWord);
    common::add_string(conditions, alloc, "conditionWordStyle", conditionWordStyle);
}

void Config::write_loops(rapidjson::Value &loops, rapidjson::Document::AllocatorType& alloc) const {
    common::add_string(loops, alloc, "doWord", doWord);
    common::add_string(loops, alloc, "whileWord", whileWord);
    common::add_string(loops, alloc, "forWord", forWord);
    common::add_string(loops, alloc, "foreachWord", foreachWord);
    common::add_string(loops, alloc, "loopWordStyle", loopWordStyle);
}

void Config::write_other(rapidjson::Value &other, rapidjson::Document::AllocatorType& alloc) const {
    common::add_string(other, alloc, "returnWord", returnWord);
    common::add_string(other, alloc, "continueWord", continueWord);
    common::add_string(other, alloc, "breakWord", breakWord);
    common::add_string(other, alloc, "tryWord", tryWord);
    common::add_string(other, alloc, "catchWord", catchWord);
    common::add_string(other, alloc, "finallyWord", finallyWord);
    common::add_string(other, alloc, "throwWord", throwWord);
    common::add_string(other, alloc, "newWord", newWord);
    common::add_string(other, alloc, "deleteWord", deleteWord);
    common::add_string(other, alloc, "otherExprStyle", otherExprStyle);
}

void Config::write_support_expressions(rapidjson::Value &expr, rapidjson::Document::AllocatorType &alloc) const {
    common::add_string(expr, alloc, "constructorWord", constructorWord);
    common::add_string(expr, alloc, "destructorWord", destructorWord);
    common::add_string(expr, alloc, "methodWord", methodWord);
    common::add_string(expr, alloc, "functionWord", functionWord);
    common::add_string(expr, alloc, "lambdaWord", lambdaWord);
    common::add_string(expr, alloc, "callWord", callWord);
    common::add_string(expr, alloc, "defineWord", defineWord);
    common::add_string(expr, alloc, "returnsWord", returnsWord);
    common::add_string(expr, alloc, "repeatWord", repeatWord);
    common::add_string(expr, alloc, "supportExprStyle", supportExprStyle);
}

void Config::write_output_settings(rapidjson::Value &settings, rapidjson::Document::AllocatorType &alloc) const {
    common::add_string(settings, alloc, "fileName", fileName);
    common::add_string(settings, alloc, "filePath", filePath);
    common::add_string(settings, alloc, "fileFormat", fileFormat);
}


} // namespace astfri::text
