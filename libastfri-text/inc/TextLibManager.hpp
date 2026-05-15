#ifndef LIBASTFRI_TEXT_TEXT_LIB_MANAGER
#define LIBASTFRI_TEXT_TEXT_LIB_MANAGER

#include <libastfri-text/inc/pseudocode/PseudocodeVisitor.hpp>

#include <iostream>

namespace astfri::text {
enum class OutputFormat {
    CxxCode,
    JavaCode,
    Pseudocode
};

//
// -----
//

class TextLibManager {
    AbstractVisitor* visitor_;

public:
    static TextLibManager& get_instance();
    TextLibManager(const TextLibManager&)            = delete;
    TextLibManager(TextLibManager&&)                 = delete;
    TextLibManager& operator=(const TextLibManager&) = delete;
    TextLibManager& operator=(TextLibManager&&)      = delete;

private:
    explicit TextLibManager();
    ~TextLibManager() = default;

public:
    void change_output(const OutputFormat& format);
    void execute_export(std::ostream& ostream);
    void execute_export();
    void clear_builder();
    void append_text(const std::string& text);
    void append_new_line();
    void append_space();
    void update_configuration();
    void reload_configuration();
    // -----
    template<typename Node>
    void visit_and_export(const Node& node, std::ostream& ostream);
    // -----
    template<typename Node>
    void visit_and_export(const Node& node);
    // -----
    template<typename Node>
    void visit(const Node& node);
};

//
// -----
//

template<typename Node>
void TextLibManager::visit_and_export(const Node& node, std::ostream& ostream) {
    visit(node);
    execute_export(ostream);
}

template<typename Node>
void TextLibManager::visit_and_export(const Node& node) {
    visit(node);
    execute_export();
}

// -----

template<typename Node>
void TextLibManager::visit(const Node& node) {
    auto* tu = dynamic_cast<const TranslationUnit*>(&node);
    if (!tu) {
        std::cout << " > Instance of TranslationUnit required to generate code.\n";
        return;
    }
    const_cast<Node&>(node).accept(*visitor_);
}
}

#endif