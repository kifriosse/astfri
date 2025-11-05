#include <libastfri-text/inc/pseudocode/Exporter.hpp>

#include <filesystem>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace astfri::text;

Exporter::Exporter()
{
    configurator_ = &TextConfigurator::get_instance();
    reset();
}

void Exporter::reset()
{
    outputPseudocode_ = std::make_unique<ss>();
    currentIndentationLevel_ = 0;
    isEmptyLine_ = true;
    rowCount_ = 1;
}

void Exporter::execute_export()
{
    std::string suffix;
    suffix = configurator_->output_file_format()->str() == "html" ? ".html" : ".txt";
    std::filesystem::path outputpath = configurator_->output_file_path()->str();
    std::string filename = configurator_->output_file_name()->str();
    outputpath.concat(filename + suffix);
    if (!configurator_->overwrite_file())
    {
        int id = 0;
        while (std::filesystem::exists(outputpath) && std::filesystem::is_regular_file(outputpath))
        {
            outputpath.replace_filename(filename + "(" + std::to_string(id) + ")" + suffix);
            ++id;
        }
    }
    try
    {
        std::filesystem::create_directories(outputpath.parent_path());
        write_pseudocode_into_file(outputpath.string().c_str());
        std::cout << " > You can find output file at path: " << outputpath.string() << "\n";
        std::cout << " > File write completed!\n";
    }
    catch (std::filesystem::filesystem_error&)
    {
        std::cout << " > Ups! Something went wrong when creating output file!\n";
        std::cout << " > Try to change file name or file path.\n";
    }
}

void Exporter::write_pseudocode_into_file(STRING fullfilepath)
{
    std::ofstream file(fullfilepath);
    if (configurator_->sh_row_num())
    {
        int row = 1;
        int delimiter = static_cast<int>(std::log10(rowCount_)) + 1;
        std::string line;
        while (std::getline(*outputPseudocode_,line))
        {
            if (!line.empty() || (line.empty() && configurator_->sh_row_num_empty_row()))
            {
                for (int i = 0; i < configurator_->row_num_margin_left_len(); ++i)
                {
                    file << " ";
                }
                file << std::setw(delimiter) << row;
                if (configurator_->sh_dot_after_row_num())
                {
                    file << ".";
                }
                ++row;
            }
            else if (configurator_->reset_row_num_empty_row())
            {
                row = 1;
            }
            file << line << "\n";
        }
    }
    else
    {
        file << outputPseudocode_->str();
    }
    file.close();
    reset();
}

// GENERAL

void Exporter::increase_indentation()
{
    ++currentIndentationLevel_;
}

void Exporter::decrease_indentation()
{
    --currentIndentationLevel_;
}

void Exporter::write_text(std::string const& text)
{
    if (isEmptyLine_)
    {
        isEmptyLine_ = false;
        for (int i = 0; i < configurator_->text_margin_left_len(); ++i)
        {
            write_space();
        }
        for (int i = 0; i < currentIndentationLevel_; ++i)
        {
            for (int j = 0; j < configurator_->tabulator_len(); ++j)
            {
                write_space();
            }
        }
    }
    *outputPseudocode_ << std::move(text);
}

void Exporter::write_new_line()
{
    if (configurator_->output_file_format()->str() == "html")
    {
        *outputPseudocode_ << std::move("<br>");
    }
    *outputPseudocode_ << std::move('\n');
    isEmptyLine_ = true;
    ++rowCount_;
}

void Exporter::write_space()
{
    if (configurator_->output_file_format()->str() == "html")
    {
        write_text("&nbsp;");
    }
    else
    {
        write_text(" ");
    }
}

// BRACKETS

void Exporter::write_left_bracket(STRING br)
{
    write_text(std::move(br));
}

void Exporter::write_right_bracket(STRING br)
{
    write_text(std::move(br));
}

// UNKNOWN_PHRASES

void Exporter::write_unknown_type_word()
{
    write_text(configurator_->unknown_type_word()->str());
}

void Exporter::write_unknown_expr_word()
{
    write_text(configurator_->unknown_expr_word()->str());
}

void Exporter::write_unknown_stmt_word()
{
    write_text(configurator_->unknown_stmt_word()->str());
}

void Exporter::write_invalid_type_word()
{
    write_text(configurator_->invalid_type_word()->str());
}

void Exporter::write_invalid_expr_word()
{
    write_text(configurator_->invalid_expr_word()->str());
}

void Exporter::write_invalid_stmt_word()
{
    write_text(configurator_->invalid_stmt_word()->str());
}

// ACCESS_MODIFIERS

void Exporter::write_public_word()
{
    write_text(configurator_->public_word()->str());
}

void Exporter::write_protected_word()
{
    write_text(configurator_->protected_word()->str());
}

void Exporter::write_private_word()
{
    write_text(configurator_->private_word()->str());
}

void Exporter::write_internal_word()
{
    write_text(configurator_->internal_word()->str());
}

void Exporter::write_attributes_word()
{
    write_text(configurator_->attributes_word()->str());
}

void Exporter::write_constructors_word()
{
    write_text(configurator_->constructors_word()->str());
}

void Exporter::write_destructors_word()
{
    write_text(configurator_->destructors_word()->str());
}

void Exporter::write_methods_word()
{
    write_text(configurator_->methods_word()->str());
}

// DATA_TYPES

void Exporter::write_dynamic_type_word()
{
    write_text(configurator_->dynamic_type_word()->str());
}

void Exporter::write_int_type_word()
{
    write_text(configurator_->int_type_word()->str());
}

void Exporter::write_float_type_word()
{
    write_text(configurator_->float_type_word()->str());
}

void Exporter::write_char_type_word()
{
    write_text(configurator_->char_type_word()->str());
}

void Exporter::write_bool_type_word()
{
    write_text(configurator_->bool_type_word()->str());
}

void Exporter::write_void_type_word()
{
    write_text(configurator_->void_type_word()->str());
}

void Exporter::write_user_type(std::string usertype)
{
    write_text(std::move(usertype));
}

// REFERENCE_NAMES

void Exporter::write_gen_param_name(std::string name)
{
    write_text(std::move(name));
}

void Exporter::write_class_name(std::string name)
{
    write_text(std::move(name));
}

void Exporter::write_interface_name(std::string name)
{
    write_text(std::move(name));
}

void Exporter::write_method_name(std::string name)
{
    write_text(std::move(name));
}

void Exporter::write_function_name(std::string name)
{
    write_text(std::move(name));
}

void Exporter::write_global_var_name(std::string name)
{
    write_text(std::move(name));
}

void Exporter::write_member_var_name(std::string name)
{
    write_text(std::move(name));
}

void Exporter::write_local_var_name(std::string name)
{
    write_text(std::move(name));
}

void Exporter::write_param_var_name(std::string name)
{
    write_text(std::move(name));
}

// SIGNS

void Exporter::write_operator_sign(STRING sign)
{
    write_text(std::move(sign));
}

void Exporter::write_assign_op_word()
{
    write_text(configurator_->assign_op_word()->str());
}

void Exporter::write_modulo_op_word()
{
    write_text(configurator_->modulo_op_word()->str());
}

void Exporter::write_address_op_word()
{
    write_text(configurator_->address_op_word()->str());
}

void Exporter::write_deref_op_word()
{
    write_text(configurator_->deref_op_word()->str());
}

void Exporter::write_separator_sign(STRING sign)
{
    write_text(std::move(sign));
}

// VALUES

void Exporter::write_int_val(int val)
{
    write_text(std::to_string(std::move(val)));
}

void Exporter::write_float_val(float val)
{
    write_text(std::to_string(std::move(val)));
}

void Exporter::write_char_val(char val)
{
    write_text(std::string(1, std::move(val)));
}

void Exporter::write_string_val(std::string val)
{
    write_text(std::move(val));
}

void Exporter::write_bool_val(bool val)
{
    if (val)
    {
        write_text(configurator_->true_val_word()->str());
    }
    else
    {
        write_text(configurator_->false_val_word()->str());
    }
}

void Exporter::write_null_val()
{
    write_text(configurator_->null_val_word()->str());
}

// SYSTEM_EXPRESSIONS

void Exporter::write_class_word()
{
    write_text(configurator_->class_word()->str());
}

void Exporter::write_interface_word()
{
    write_text(configurator_->interface_word()->str());
}

void Exporter::write_implement_word()
{
    write_text(configurator_->implement_word()->str());
}

void Exporter::write_extend_word()
{
    write_text(configurator_->extend_word()->str());
}

void Exporter::write_this_word()
{
    write_text(configurator_->this_word()->str());
}

void Exporter::write_return_word()
{
    write_text(configurator_->return_word()->str());
}

void Exporter::write_continue_word()
{
    write_text(configurator_->continue_word()->str());
}

void Exporter::write_break_word()
{
    write_text(configurator_->break_word()->str());
}

void Exporter::write_throw_word()
{
    write_text(configurator_->throw_word()->str());
}

void Exporter::write_if_word()
{
    write_text(configurator_->if_word()->str());
}

void Exporter::write_else_word()
{
    write_text(configurator_->else_word()->str());
}

void Exporter::write_do_word()
{
    write_text(configurator_->do_word()->str());
}

void Exporter::write_while_word()
{
    write_text(configurator_->while_word()->str());
}

void Exporter::write_for_word()
{
    write_text(configurator_->for_word()->str());
}

void Exporter::write_repeat_word()
{
    write_text(configurator_->repeat_word()->str());
}

void Exporter::write_switch_word()
{
    write_text(configurator_->switch_word()->str());
}

void Exporter::write_case_word()
{
    write_text(configurator_->case_word()->str());
}

void Exporter::write_default_word()
{
    write_text(configurator_->default_word()->str());
}

void Exporter::write_new_word()
{
    write_text(configurator_->new_word()->str());
}

void Exporter::write_delete_word()
{
    write_text(configurator_->delete_word()->str());
}

void Exporter::write_pointer_word()
{
    write_text(configurator_->pointer_word()->str());
}

void Exporter::write_virtual_word()
{
    write_text(configurator_->virtual_word()->str());
}

void Exporter::write_abstract_word()
{
    write_text(configurator_->abstract_word()->str());
}

void Exporter::write_template_word()
{
    write_text(configurator_->template_word()->str());
}

// OTHER_EXPRESSIONS

void Exporter::write_constructor_word()
{
    write_text(configurator_->constructor_word()->str());
}

void Exporter::write_destructor_word()
{
    write_text(configurator_->destructor_word()->str());
}

void Exporter::write_method_word()
{
    write_text(configurator_->method_word()->str());
}

void Exporter::write_function_word()
{
    write_text(configurator_->function_word()->str());
}

void Exporter::write_lambda_word()
{
    write_text(configurator_->lambda_word()->str());
}

void Exporter::write_call_word()
{
    write_text(configurator_->call_word()->str());
}

void Exporter::write_define_word()
{
    write_text(configurator_->define_word()->str());
}

void Exporter::write_returns_word()
{
    write_text(configurator_->returns_word()->str());
}