class AccessModifier{};
struct Stmt{};
struct VarDefStmt : Stmt{};
struct Type{};
struct PrimitiveType : Type{};
struct IntType : PrimitiveType{};
struct CharType : PrimitiveType{};
struct FloatType : PrimitiveType{};
struct BoolType : PrimitiveType{};
struct VoidType : PrimitiveType{};
struct UserType : Type{};
struct IndirectionType : Type{};

struct TranslationUnit : Stmt{};
struct ParamVarDefStmt : VarDefStmt{};
struct MemberVarDefStmt : VarDefStmt{};
struct FunctionDefStmt : Stmt{};
struct MethodDefStmt : Stmt{};
struct ConstructorDefStmt : Stmt{};
struct DestructorDefStmt : Stmt{};
struct GenericParam : Stmt{};
struct InterfaceDefStmt : Stmt{};
struct ClassDefStmt : Stmt{};

struct IVisitor
{
    virtual void visit(IntType type)             = 0;
    virtual void visit(FloatType type)           = 0;
    virtual void visit(CharType type)            = 0;
    virtual void visit(BoolType type)            = 0;
    virtual void visit(VoidType type)            = 0;
    virtual void visit(UserType type)            = 0;
    virtual void visit(IndirectionType type)     = 0;
    virtual void visit(TranslationUnit stmt)     = 0;
    virtual void visit(ParamVarDefStmt stmt)     = 0;
    virtual void visit(MemberVarDefStmt stmt)    = 0;
    virtual void visit(FunctionDefStmt stmt)     = 0;
    virtual void visit(MethodDefStmt stmt)       = 0;
    virtual void visit(ConstructorDefStmt stmt)  = 0;
    virtual void visit(DestructorDefStmt stmt)   = 0;
    virtual void visit(GenericParam stmt)        = 0;
    virtual void visit(InterfaceDefStmt stmt)    = 0;
    virtual void visit(ClassDefStmt stmt)        = 0;

    virtual ~IVisitor()                                 = default;
};

struct VisitorAdapter : IVisitor
{
    void visit (IntType constpe) override;
    void visit (FloatType constpe) override;
    void visit (CharType constpe) override;
    void visit (BoolType constpe) override;
    void visit (VoidType constpe) override;
    void visit (UserType constpe) override;
    void visit (IndirectionType constpe) override;
    void visit (ParamVarDefStmt constmt) override;
    void visit (MemberVarDefStmt constmt) override;
    void visit (FunctionDefStmt constmt) override;
    void visit (MethodDefStmt constmt) override;
    void visit (ConstructorDefStmt constmt) override;
    void visit (DestructorDefStmt constmt) override;
    void visit (GenericParam constmt) override;
    void visit (ClassDefStmt constmt) override;
    void visit (InterfaceDefStmt constmt) override;
    void visit (TranslationUnit constmt) override;
};

class RelationType {
};

class UserDefinedType {
};

struct ClassStruct {
    char* name_;
    char* genericParams_;

    void reset();
};

struct VarStruct {
    char* name_;
    char* type_;
    char* init_;
    bool isIndirect_ = false;
    AccessModifier accessMod_;

    void reset();
};

struct MethodStruct {
    char* name_;
    char* retType_;
    bool returnIsIndirect_= false;
    AccessModifier accessMod_;
    VarStruct params_;

    void reset();
};

struct RelationStruct {
    char* from_;
    char* to_;
    RelationType type_;
};

class TypeConvention {
public:
    virtual char* get_string(char* type, char* val, char separator) = 0;
};

class TypeBeforeConvention : public TypeConvention {
public:
    char* get_string(char* type, char* val, char separator) override;
};

class TypeAfterConvention : public TypeConvention {
public:
    char* get_string(char* type, char* val, char separator) override;
};
class RapidjsonValue{};
struct Config {
    // type var, var : type
    TypeConvention* typeConvention_;
    bool innerView_ = true;
    bool writeToFile_ = false;
    bool drawAccessModIcons_ = true;

    char indirectIndicator_ = '*';
    char destructorIndicator_ = '~';
    char separator_ = ' ';
    char accessPrefix_;

    char* relationArrows_;
    char* intTypeName_;
    char* floatTypeName_;
    char* charTypeName_;
    char* boolTypeName_;
    char* voidTypeName_;

    char* diagramBG_;
    char* elementBG_;
    char* elementBorder_;
    char* fontColor_;
    char* arrowColor_;

    char* outputFilePath_;

    bool parse_json(const char* path);
    void use_default_values();
private:
    bool parse_file_info(const RapidjsonValue val);
    bool parse_types_info(const RapidjsonValue val);
    bool parse_access_info(const RapidjsonValue val);
    bool parse_colors_info(const RapidjsonValue val);
    bool parse_relations_info(const RapidjsonValue val);
    bool parse_destructor_info(const RapidjsonValue val);
};

class UMLOutputter {
private:

protected:
    Config* config_;
    char* outputString_;

public:
    void set_config(Config config);

    virtual void write_to_file();
    virtual void write_to_console();

    virtual char* getFileExtension() = 0;

    virtual void open_user_type(ClassStruct c, UserDefinedType t) = 0;
    virtual void close_user_type() = 0;

    virtual void add_data_member(VarStruct v) = 0;
    virtual void add_function_member(MethodStruct m) = 0;

    virtual void add_relation(RelationStruct r) = 0;

};

class PlantUMLOutputter : public UMLOutputter {
private:
    void open(ClassStruct cs);
    void apply_style_from_config();
    void add_tags_and_style();

public:
    virtual void write_to_file() override;
    virtual void write_to_console() override;
    
    virtual char* getFileExtension() override;

    virtual void open_user_type(ClassStruct c, UserDefinedType t) override;
    virtual void close_user_type() override;

    virtual void add_data_member(VarStruct v) override;
    virtual void add_function_member(MethodStruct m) override;

    virtual void add_relation(RelationStruct r) override;
};

class ClassVisitor : public VisitorAdapter
{
private:
    UMLOutputter* outputter_;
    Config* config_;

    ClassStruct currentClass_;
    MethodStruct currentMethod_;
    VarStruct currentVariable_;

    RelationStruct* relations_;
    char* classes_;
    char* interfaces_;

    void create_relation(char* target, RelationType type);
    bool find_relation(RelationStruct rel);
    bool find_class(char* name);
    bool find_interface(char* name);

    void finish();
public:
    void set_config(Config config);
    void set_outputter(UMLOutputter outputter);

    void visit (IntType type) override;
    void visit (FloatType type) override;
    void visit (CharType type) override;
    void visit (BoolType type) override;
    void visit (VoidType type) override;
    void visit (UserType type) override;
    void visit (IndirectionType type) override;
    void visit (ParamVarDefStmt stmt) override;
    void visit (MemberVarDefStmt stmt) override;
    void visit (FunctionDefStmt stmt) override;
    void visit (MethodDefStmt stmt) override;
    void visit (ConstructorDefStmt stmt) override;
    void visit (DestructorDefStmt stmt) override;
    void visit (GenericParam stmt) override;
    void visit (ClassDefStmt stmt) override;
    void visit (InterfaceDefStmt stmt) override;
    void visit (TranslationUnit stmt) override;
};

class UMLLibWrapper {
private:
    ClassVisitor classVisitor_;
public:
    void init(Config* config, UMLOutputter* output, TypeConvention* typeConvention);
    void run(TranslationUnit* translationUnit);
};
