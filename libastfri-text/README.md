Ak chceš použiť knižnicu na textový výstup, sprav nasledovné.

1) Musíš spraviť správny build. Knižnica sa volá "astfri-text".
2) Použi takýto include: "#include <libastfri-text/inc/ASTVisitor.hpp>"
3) Vytvor si inštanciu ASTVisitor-a: "std::unique_ptr<ASTVisitor> visitor = std::make_unique<ASTVisitor>();"
4) Inštancií pošli správu visit s ľubovoľným vrcholom: "visitor.visit(astfri::VoidType);"
5) Zavolaj metódu: "visitor->write_file();"

Program funguje aj bez konfiguračného súboru, vtedy sa použije predvolený formát(txt súbor).
Program vypíše cestu, kde vytvorí exportovaný súbor.
Konfigurácia(formátovanie výstupu) je plne ošetrená voči chybám v konfiguračnom súbore. Konfiguráciu je možné nastaviť pre ľubovoľné parametre, podmienka je zachovanie "hierarchie" tohto súboru. Na samotnom poradí nezáleží.

Vstupný konfiguračný súbor je buď možné použiť predvolený v priečinku impl, alebo si môžeš vytvoriť svoj(program sa ťa opýta na cestu ku .json súboru).
Cestu k výstupu si buď zadáš cez konfigurák, alebo sa použije predvolená cesta. Tá ukazuje buď priamo do priečinka "build", ak taký v ceste ku programu máš, a ak nie, tak sa použije adresár "/home/USER/", kde USER je aktuálne prihlásený používateľ. Pri hocijakej chybe sa taktiež použije predvolená cesta.


"format": txt/html
Takto vyzerá kompletný konfiguračný súbor s defaultnými hodnotami.
{
    "CONFIGURATOR":{
        "FILE":{
            "name":"output",
            "path":"",
            "format":"txt"
        },
        "DEFAULT_STYLE":"font-family:Consolas;font-size:18px",
        "UNKNOWN_WORD":"UNKNOWN EXPRESSION",
        "UNKNOWN_WORD_STYLE":"",
        "INVALID_WORD":"INVALID EXPRESSION",
        "INVALID_WORD_STYLE":"",
        "ACCESS_MOD":{
            "view":"inner",
            "public":"public",
            "private":"private",
            "protected":"protected",
            "internal":"internal",
            "attribs":"attributes",
            "constructors":"constructors",
            "destructors":"destructors",
            "methods":"methods",
            "style":""
        },
        "DATA_TYPE":{
            "dynamic":"auto",
            "int":"int",
            "float":"float",
            "char":"char",
            "bool":"bool",
            "void":"void",
            "STYLE":{
                "general_style":"",
                "dynamic_type_style":"",
                "int_type_style":"",
                "float_type_style":"",
                "char_type_style":"",
                "bool_type_style":"",
                "void_type_style":"",
                "user_type_style":""
            }
        },
        "REF_NAME_STYLE":{
            "def_ref_name_style":"",
            "gen_param_name_style":"",
            "class_name_style":"",
            "interface_name_style":"",
            "method_name_style":"",
            "function_name_style":"",
            "global_var_name_style":"",
            "member_var_name_style":",
            "local_var_name_style":"",
            "param_var_name_style":""
        },
        "OPERATOR":{
            "assign":"=",
            "modulo":"%",
            "address":"&",
            "deref":"*",
            "style":""
        },
        "SEPARATOR":{
            "style":""
        },
        "VALUE":{
            "true_val":"true",
            "false_val":"false",
            "null_val":"NULL",
            "STYLE":{
                "general_val_style":"",
                "int_val_style":"",
                "float_val_style":"",
                "char_val_style":"",
                "string_val_style":"",
                "bool_val_style":"",
                "null_val_style":""
            }
        }
    },
    "TEXT_CONFIGURATOR":{
        "STRUCT_SETTING":{
            "use_cpp_bracket":true,
            "use_br_color":true,
            "show_row":true,
            "show_other_expr":true,
            "show_class_dec":true,
            "show_class_def":true,
            "show_interf_dec":true,
            "show_interf_def":true,
            "show_gener_par":true,
            "show_attribute":true,
            "show_meth_dec":true,
            "show_meth_def":true,
            "show_meth_owner":true,
            "show_meth_templ":true,
            "show_func_dec":true,
            "show_func_def":true,
            "show_global_par":true,
            "tab_length":4,
            "margin_length":3,
            "round_br_colors":[],
            "curl_br_colors":[],
            "row_style":""
        },
        "SYSTEM_EXPR":{
            "class_word":"class",
            "interface_word":"interface",
            "implement_word":"implements",
            "extend_word":"extends",
            "this_word":"this",
            "return_word":"return",
            "continue_word":"continue",
            "break_word":"break",
            "throw_word":"throw",
            "if_word":"if",
            "else_word":"else",
            "do_word":"do",
            "while_word":"while",
            "for_word":"for",
            "repeat_word":"repeat",
            "switch_word":"switch",
            "case_word":"case",
            "default_word":"default",
            "new_word":"new",
            "delete_word":"delete",
            "pointer_word":"↑",
            "virtual_word":"is virtual",
            "abstract_word":"is abstract",
            "template_word":"template",
            "STYLE":{
                "general_style":"",
                "class_word_style":"",
                "interface_word_style":"",
                "implement_word_style":"",
                "extend_word_style":"",
                "this_word_style":"",
                "return_word_style":"",
                "continue_word_style":"",
                "break_word_style":"",
                "throw_word_style":"",
                "if_word_style":"",
                "else_word_style":"",
                "do_word_style":"",
                "while_word_style":"",
                "for_word_style":"",
                "repeat_word_style":"",
                "switch_word_style":"",
                "case_word_style":"",
                "default_word_style":"",
                "new_word_style":"",
                "delete_word_style":"",
                "pointer_word_style":"",
                "virtual_word_style":"",
                "abstract_word_style":"",
                "template_word_style":""
            }
        },
        "OTHER_EXPR":{
            "constructor_word":"constructor",
            "destructor_word":"destructor",
            "method_word":"method",
            "function_word":"function",
            "lambda_word":"λ",
            "call_word":"call",
            "define_word":"define",
            "returns_word":"returns",
            "STYLE":{
                "general_style":"",
                "constructor_style":"",
                "destructor_style":"",
                "method_word_style":"",
                "function_word_style":"",
                "lambda_word_style":"",
                "call_word_style":"",
                "define_word_style":"",
                "returns_word_style":""
            }
        }
    }
}

mkdir build
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DASTFRI_BUILD_TEXT_OUTPUT=ON -DCMAKE_BUILD_TYPE=Debug ..
make
cp compile_commands.json ..

alebo

mkdir build
cd build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DASTFRI_BUILD_TEXT_OUTPUT=ON ..
make
cp compile_commands.json ..