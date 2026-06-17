#ifndef CSHARP_AST_BUILDER_INL
#define CSHARP_AST_BUILDER_INL

namespace astfri {

template<typename Source>
TranslationUnit CSharpOutput::load(csharp::Config& cfg, Source& source) {
    csharp::ASTBuilder builder;
    for (auto& ext : cfg.externalTypeSources) {
        builder.load_source_of_external_types(ext);
    }
    builder.load_src(source);
    return *builder.mk_ast(cfg.profile);
}

} // namespace astfri

#endif // CSHARP_AST_BUILDER_INL