#include <astfri-text/impl/pseudocode/HtmlTextBuilder.hpp>

#include <cmath>

using namespace astfri::text;

HtmlTextBuilder::HtmlTextBuilder(Config const& config) :
    PseudocodeBuilder(config),
    m_htmlString(""),
    m_maxBracketIndex(config.brColors.size()),
    m_currentBrIndex(0)
{
}

void HtmlTextBuilder::reset_builder()
{
    m_buildedText.clear();
    m_indentationLevel = 0;
    m_isEmptyLine      = true;
    m_rowCount         = 1;
    m_htmlString.clear();
    m_currentBrIndex   = 0;
}

std::string& HtmlTextBuilder::get_builded_text()
{
    m_htmlString.append("<!DOCTYPE html>\n");
    m_htmlString.append("<html lang=\"sk\">\n");
    m_htmlString.append("<head>\n");
    m_htmlString.append("<meta charset=\"UTF-8\">\n");
    m_htmlString.append("<title>").append(m_config.fileName).append("</title>\n");
    m_htmlString.append("<style>\n");
    // 3.1 GENERAL
    m_htmlString.append("body{").append(m_config.defaultTextStyle).append("}\n");
    m_htmlString.append(".row-number-style{").append(m_config.rowNumStyle).append("}\n");
    m_htmlString.append(".unknown-phrase-style{").append(m_config.unknownPhraseStyle).append("}\n");
    // 3.2 SYMBOLS
    m_htmlString.append(".operator-style{").append(m_config.opWordStyle).append("}\n");
    m_htmlString.append(".separator-style{").append(m_config.sepWordStyle).append("}\n");
    // 3.3 VALUES
    m_htmlString.append(".value-style{").append(m_config.valueStyle).append("}\n");
    m_htmlString.append(".numeric-value-style{").append(m_config.numericValueStyle).append("}\n");
    m_htmlString.append(".string-value-style{").append(m_config.stringValueStyle).append("}\n");
    // 3.4 REFERENCE_NAMES
    m_htmlString.append(".ref-name-style{").append(m_config.defaultRefNameStyle).append("}\n");
    m_htmlString.append(".template-name-style{").append(m_config.templateNameStyle).append("}\n");
    m_htmlString.append(".class-name-style{").append(m_config.classNameStyle).append("}\n");
    m_htmlString.append(".interface-name-style{").append(m_config.interfaceNameStyle).append("}\n");
    m_htmlString.append(".method-name-style{").append(m_config.methodNameStyle).append("}\n");
    m_htmlString.append(".function-name-style{").append(m_config.functionNameStyle).append("}\n");
    m_htmlString.append(".var-name-style{").append(m_config.defaultVarNameStyle).append("}\n");
    m_htmlString.append(".global-var-name-style{").append(m_config.globalVarNameStyle).append("}\n");
    m_htmlString.append(".member-var-name-style{").append(m_config.memberVarNameStyle).append("}\n");
    m_htmlString.append(".local-var-name-style{").append(m_config.localVarNameStyle).append("}\n");
    m_htmlString.append(".param-var-name-style{").append(m_config.paramVarNameStyle).append("}\n");
    // 3.5 SYSTEM_EXPRESSIONS
    m_htmlString.append(".access-mod-style{").append(m_config.accessModifStyle).append("}\n");
    m_htmlString.append(".type-word-style{").append(m_config.typeWordStyle).append("}\n");
    m_htmlString.append(".numeric-type-word-style{").append(m_config.numericTypeWordStyle).append("}\n");
    m_htmlString.append(".string-type-word-style{").append(m_config.stringTypeWordStyle).append("}\n");
    m_htmlString.append(".system-type-word-style{").append(m_config.systemTypeWordStyle).append("}\n");
    m_htmlString.append(".object-word-style{").append(m_config.objectWordStyle).append("}\n");
    m_htmlString.append(".condition-word-style{").append(m_config.conditionWordStyle).append("}\n");
    m_htmlString.append(".loop-word-style{").append(m_config.loopWordStyle).append("}\n");
    m_htmlString.append(".other-expr-word-style{").append(m_config.otherExprStyle).append("}\n");
    // 3.6 SUPPORT_EXPRESSIONS
    m_htmlString.append(".support-phrase-style{").append(m_config.supportExprStyle).append("}\n");
    //
    m_htmlString.append("</style>\n");
    m_htmlString.append("</head>\n");
    //
    m_htmlString.append("<body>\n");
    add_row_numbers();
    m_htmlString.append("</body>\n");
    m_htmlString.append("</html>\n");
    return m_htmlString;
}

/**
 * Write builded text into HTML body (stored in m_htmlString),
 * and add row numbers is required.
 */
void HtmlTextBuilder::add_row_numbers()
{
    m_htmlString.reserve(m_htmlString.size() + m_buildedText.size());
    if (!m_config.shRowNum)
    {
        m_htmlString.append(m_buildedText);
        return;
    }
    int row = 1;
    int delimiter = static_cast<int>(std::log10(m_rowCount)) + 1;
    std::string line;
    std::stringstream input(m_buildedText);
    while (std::getline(input, line))
    {
        if (line != "<br>" || m_config.shRowNumOnEmptyRow)
        {
            for (int i = 0; i < m_config.rowNumMarginLeft; ++i)
            {
                m_htmlString.append("&nbsp;");
            }
            int curDelim = static_cast<int>(std::log10(row)) + 1;
            int spaces = delimiter - curDelim;
            for (int i = 0; i < spaces; ++i)
            {
                m_htmlString.append("&nbsp;");
            }
            m_htmlString.append("<span class=\"row-number-style\">").append(std::to_string(row));
            if (m_config.shDotAfterRowNum)
            {
                m_htmlString.append(".");
            }
            m_htmlString.append("</span>");
            ++row;
        }
        m_htmlString.append(line).push_back('\n');
    }
}

// 3.1 GENERAL

void HtmlTextBuilder::write_new_line()
{
    m_buildedText.append("<br>\n");
    m_isEmptyLine = true;
    ++m_rowCount;
}

void HtmlTextBuilder::write_space()
{
    m_buildedText.append("&nbsp;");
}

void HtmlTextBuilder::write_unknown_type()
{
    use_unknown_phrase_style(m_config.unknownTypeWord);
}

void HtmlTextBuilder::write_unknown_expr()
{
    use_unknown_phrase_style(m_config.unknownExprWord);
}

void HtmlTextBuilder::write_unknown_stmt()
{
    use_unknown_phrase_style(m_config.unknownStmtWord);
}

// 3.2 SYMBOLS
// 3.2.1 OPERATORS

void HtmlTextBuilder::write_operator(std::string_view op)
{
    use_operator_style(op);
}

void HtmlTextBuilder::write_left_bracket(std::string_view br)
{
    if (m_config.shBrColors && m_maxBracketIndex > 0)
    {
        std::string tmp;
        tmp.append("<span style=\"color:").append(m_config.brColors.at(m_currentBrIndex));
        tmp.append("\">").append(br).append("</span>");
        ++m_currentBrIndex;
        if (m_currentBrIndex == m_maxBracketIndex)
        {
            m_currentBrIndex = 0;
        }
        use_operator_style(tmp);
    }
    else
    {
        use_operator_style(br);
    }
}

void HtmlTextBuilder::write_right_bracket(std::string_view br)
{
    if (m_config.shBrColors && m_maxBracketIndex > 0)
    {
        if (m_currentBrIndex == 0)
        {
            m_currentBrIndex = m_maxBracketIndex - 1;
        }
        else
        {
            --m_currentBrIndex;
        }
        std::string tmp;
        tmp.append("<span style=\"color:").append(m_config.brColors.at(m_currentBrIndex));
        tmp.append("\">").append(br).append("</span>");
        use_operator_style(tmp);
    }
    else
    {
        use_operator_style(br);
    }
}

void HtmlTextBuilder::write_pointer_operator()
{
    use_operator_style(m_config.pointerWord);
}

void HtmlTextBuilder::write_assign_operator()
{
    use_operator_style(m_config.assignWord);
}

void HtmlTextBuilder::write_modulo_operator()
{
    use_operator_style(m_config.moduloWord);
}

void HtmlTextBuilder::write_address_operator()
{
    use_operator_style(m_config.addressWord);
}

void HtmlTextBuilder::write_deref_operator()
{
    use_operator_style(m_config.derefWord);
}

// 3.2.2 SEPARATORS

void HtmlTextBuilder::write_separator(std::string_view sep)
{
    use_separator_style(sep);
}

void HtmlTextBuilder::write_semicolon_separator()
{
    use_separator_style(m_config.semicolonWord);
}

// 3.3 VALUES

void HtmlTextBuilder::write_bool_val(bool const& val)
{
    if (val)
    {
        use_numeric_value_style(m_config.trueWord);
    }
    else
    {
        use_numeric_value_style(m_config.falseWord);
    }
}

void HtmlTextBuilder::write_null_val()
{
    use_numeric_value_style(m_config.nullWord);
}

void HtmlTextBuilder::write_int_val(int const& val)
{
    use_numeric_value_style(std::to_string(val));
}

void HtmlTextBuilder::write_float_val(float const& val)
{
    use_numeric_value_style(std::to_string(val));
}

void HtmlTextBuilder::write_char_val(char const& val)
{
    use_string_value_style(std::string(1, val));
}

void HtmlTextBuilder::write_string_val(std::string_view val)
{
    use_string_value_style(val);
}

// 3.4 REFERENCE_NAMES

void HtmlTextBuilder::write_template_name(std::string_view name)
{
    std::string s;
    s.append("<span class=\"template-name-style\">").append(name).append("</span>");
    use_ref_name_style(s);
}

void HtmlTextBuilder::write_class_name(std::string_view name)
{
    std::string s;
    s.append("<span class=\"class-name-style\">").append(name).append("</span>");
    use_ref_name_style(s);
}

void HtmlTextBuilder::write_interface_name(std::string_view name)
{
    std::string s;
    s.append("<span class=\"interface-name-style\">").append(name).append("</span>");
    use_ref_name_style(s);
}

void HtmlTextBuilder::write_method_name(std::string_view name)
{
    std::string s;
    s.append("<span class=\"method-name-style\">").append(name).append("</span>");
    use_ref_name_style(s);
}

void HtmlTextBuilder::write_function_name(std::string_view name)
{
    std::string s;
    s.append("<span class=\"function-name-style\">").append(name).append("</span>");
    use_ref_name_style(s);
}

void HtmlTextBuilder::write_global_var_name(std::string_view name)
{
    std::string s;
    s.append("<span class=\"global-var-name-style\">").append(name).append("</span>");
    use_var_ref_name_style(s);
}

void HtmlTextBuilder::write_member_var_name(std::string_view name)
{
    std::string s;
    s.append("<span class=\"member-var-name-style\">").append(name).append("</span>");
    use_var_ref_name_style(s);
}

void HtmlTextBuilder::write_local_var_name(std::string_view name)
{
    std::string s;
    s.append("<span class=\"local-var-name-style\">").append(name).append("</span>");
    use_var_ref_name_style(s);
}

void HtmlTextBuilder::write_param_var_name(std::string_view name)
{
    std::string s;
    s.append("<span class=\"param-var-name-style\">").append(name).append("</span>");
    use_var_ref_name_style(s);
}

// 3.5 SYSTEM_EXPRESSIONS
// 3.5.1 ACCESS_MODIFIERS

void HtmlTextBuilder::write_public_word()
{
    use_access_mod_style(m_config.publicWord);
}

void HtmlTextBuilder::write_protected_word()
{
    use_access_mod_style(m_config.protectedWord);
}

void HtmlTextBuilder::write_private_word()
{
    use_access_mod_style(m_config.privateWord);
}

void HtmlTextBuilder::write_internal_word()
{
    use_access_mod_style(m_config.internalWord);
}

void HtmlTextBuilder::write_attributes_word()
{
    use_access_mod_style(m_config.attributesWord);
}

void HtmlTextBuilder::write_constructors_word()
{
    use_access_mod_style(m_config.constructorsWord);
}

void HtmlTextBuilder::write_destructors_word()
{
    use_access_mod_style(m_config.destructorsWord);
}

void HtmlTextBuilder::write_methods_word()
{
    use_access_mod_style(m_config.methodsWord);
}

// 3.5.2 DATA_TYPES

void HtmlTextBuilder::write_dynamic_type_word()
{
    use_system_type_style(m_config.dynamicTypeWord);
}

void HtmlTextBuilder::write_int_type_word()
{
    use_numeric_type_style(m_config.intTypeWord);
}

void HtmlTextBuilder::write_float_type_word()
{
    use_numeric_type_style( m_config.floatTypeWord);
}

void HtmlTextBuilder::write_char_type_word()
{
    use_string_type_style(m_config.charTypeWord);
}

void HtmlTextBuilder::write_bool_type_word()
{
    use_numeric_type_style(m_config.boolTypeWord);
}

void HtmlTextBuilder::write_void_type_word()
{
    use_numeric_type_style(m_config.voidTypeWord);
}

void HtmlTextBuilder::write_system_type(std::string_view name)
{
    use_system_type_style(name);
}

// 3.5.3 OBJECTS

void HtmlTextBuilder::write_scope_word()
{
    use_object_phrase_style(m_config.scopeWord);
}

void HtmlTextBuilder::write_template_word()
{
    use_object_phrase_style(m_config.templateWord);
}

void HtmlTextBuilder::write_class_word()
{
    use_object_phrase_style(m_config.classWord);
}

void HtmlTextBuilder::write_interface_word()
{
    use_object_phrase_style(m_config.interfaceWord);
}

void HtmlTextBuilder::write_implement_word()
{
    use_object_phrase_style(m_config.implementWord);
}

void HtmlTextBuilder::write_extend_word()
{
    use_object_phrase_style(m_config.extendWord);
}

void HtmlTextBuilder::write_virtual_word()
{
    use_object_phrase_style(m_config.virtualWord);
}

void HtmlTextBuilder::write_abstract_word()
{
    use_object_phrase_style(m_config.abstractWord);
}

void HtmlTextBuilder::write_static_word()
{
    use_object_phrase_style(m_config.staticWord);
}

void HtmlTextBuilder::write_override_word()
{
    use_object_phrase_style(m_config.overrideWord);
}

void HtmlTextBuilder::write_this_word()
{
    use_object_phrase_style(m_config.thisWord);
}

// 3.5.4 CONDITIONS

void HtmlTextBuilder::write_if_word()
{
    use_condition_phrase_style(m_config.ifWord);
}

void HtmlTextBuilder::write_elseif_word()
{
    use_condition_phrase_style(m_config.elseifWord);
}

void HtmlTextBuilder::write_else_word()
{
    use_condition_phrase_style(m_config.elseWord);
}

void HtmlTextBuilder::write_switch_word()
{
    use_condition_phrase_style(m_config.switchWord);
}

void HtmlTextBuilder::write_case_word()
{
    use_condition_phrase_style(m_config.caseWord);
}

void HtmlTextBuilder::write_default_word()
{
    use_condition_phrase_style(m_config.defaultWord);
}

// 3.5.5 LOOPS

void HtmlTextBuilder::write_do_word()
{
    use_loop_phrase_style(m_config.doWord);
}

void HtmlTextBuilder::write_while_word()
{
    use_loop_phrase_style(m_config.whileWord);
}

void HtmlTextBuilder::write_for_word()
{
    use_loop_phrase_style(m_config.forWord);
}

void HtmlTextBuilder::write_foreach_word()
{
    use_loop_phrase_style(m_config.foreachWord);
}

// 3.5.6 OTHER

void HtmlTextBuilder::write_return_word()
{
    use_other_phrase_style(m_config.returnWord);
}

void HtmlTextBuilder::write_continue_word()
{
    use_other_phrase_style(m_config.continueWord);
}

void HtmlTextBuilder::write_break_word()
{
    use_other_phrase_style(m_config.breakWord);
}

void HtmlTextBuilder::write_try_word()
{
    use_other_phrase_style(m_config.tryWord);
}

void HtmlTextBuilder::write_catch_word()
{
    use_other_phrase_style(m_config.catchWord);
}

void HtmlTextBuilder::write_finally_word()
{
    use_other_phrase_style(m_config.finallyWord);
}

void HtmlTextBuilder::write_throw_word()
{
    use_other_phrase_style(m_config.throwWord);
}

void HtmlTextBuilder::write_new_word()
{
    use_other_phrase_style(m_config.newWord);
}

void HtmlTextBuilder::write_delete_word()
{
    use_other_phrase_style(m_config.deleteWord);
}

// 3.6 SUPPORT_EXPRESSIONS

void HtmlTextBuilder::write_constructor_word()
{
    use_support_phrase_style(m_config.constructorWord);
}

void HtmlTextBuilder::write_destructor_word()
{
    use_support_phrase_style(m_config.destructorWord);
}

void HtmlTextBuilder::write_method_word()
{
    use_support_phrase_style(m_config.methodWord);
}

void HtmlTextBuilder::write_function_word()
{
    use_support_phrase_style(m_config.functionWord);
}

void HtmlTextBuilder::write_lambda_word()
{
    use_support_phrase_style(m_config.lambdaWord);
}

void HtmlTextBuilder::write_call_word()
{
    use_support_phrase_style(m_config.callWord);
}

void HtmlTextBuilder::write_define_word()
{
    use_support_phrase_style(m_config.defineWord);
}

void HtmlTextBuilder::write_returns_word()
{
    use_support_phrase_style(m_config.returnsWord);
}

void HtmlTextBuilder::write_repeat_word()
{
    use_support_phrase_style(m_config.repeatWord);
}

// WRAPPING_CLASSES

void HtmlTextBuilder::use_unknown_phrase_style(std::string_view phrase)
{
    write_text("<span class=\"unknown-phrase-style\">");
    write_text(phrase);
    write_text("</span>");
}

void HtmlTextBuilder::use_operator_style(std::string_view op)
{
    write_text("<span class=\"operator-style\">");
    write_text(op);
    write_text("</span>");
}

void HtmlTextBuilder::use_separator_style(std::string_view sep)
{
    write_text("<span class=\"separator-style\">");
    write_text(sep);
    write_text("</span>");
}

void HtmlTextBuilder::use_numeric_value_style(std::string_view val)
{
    write_text("<span class=\"value-style\">");
    write_text("<span class=\"numeric-value-style\">");
    write_text(val);
    write_text("</span>");
    write_text("</span>");
}

void HtmlTextBuilder::use_string_value_style(std::string_view val)
{
    write_text("<span class=\"value-style\">");
    write_text("<span class=\"string-value-style\">");
    write_text(val);
    write_text("</span>");
    write_text("</span>");
}

void HtmlTextBuilder::use_ref_name_style(std::string_view name)
{
    write_text("<span class=\"ref-name-style\">");
    write_text(name);
    write_text("</span>");
}

void HtmlTextBuilder::use_var_ref_name_style(std::string_view name)
{
    write_text("<span class=\"ref-name-style\">");
    write_text("<span class=\"var-name-style\">");
    write_text(name);
    write_text("</span>");
    write_text("</span>");
}

void HtmlTextBuilder::use_access_mod_style(std::string_view accmod)
{
    write_text("<span class=\"access-mod-style\">");
    write_text(accmod);
    write_text("</span>");
}

void HtmlTextBuilder::use_numeric_type_style(std::string_view datatype)
{
    write_text("<span class=\"type-word-style\">");
    write_text("<span class=\"numeric-type-word-style\">");
    write_text(datatype);
    write_text("</span>");
    write_text("</span>");
}

void HtmlTextBuilder::use_string_type_style(std::string_view datatype)
{
    write_text("<span class=\"type-word-style\">");
    write_text("<span class=\"string-type-word-style\">");
    write_text(datatype);
    write_text("</span>");
    write_text("</span>");
}

void HtmlTextBuilder::use_system_type_style(std::string_view datatype)
{
    write_text("<span class=\"type-word-style\">");
    write_text("<span class=\"system-type-word-style\">");
    write_text(datatype);
    write_text("</span>");
    write_text("</span>");
}

void HtmlTextBuilder::use_object_phrase_style(std::string_view phrase)
{
    write_text("<span class=\"object-word-style\">");
    write_text(phrase);
    write_text("</span>");
}

void HtmlTextBuilder::use_condition_phrase_style(std::string_view phrase)
{
    write_text("<span class=\"condition-word-style\">");
    write_text(phrase);
    write_text("</span>");
}

void HtmlTextBuilder::use_loop_phrase_style(std::string_view phrase)
{
    write_text("<span class=\"loop-word-style\">");
    write_text(phrase);
    write_text("</span>");
}

void HtmlTextBuilder::use_other_phrase_style(std::string_view phrase)
{
    write_text("<span class=\"other-expr-word-style\">");
    write_text(phrase);
    write_text("</span>");
}

void HtmlTextBuilder::use_support_phrase_style(std::string_view phrase)
{
    write_text("<span class=\"support-phrase-style\">");
    write_text(phrase);
    write_text("</span>");
}