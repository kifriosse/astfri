Ak chceš použiť knižnicu na textový výstup, sprav nasledovné.

1) Musíš spraviť správny build. Knižnica sa volá "astfri-text".
2) Použi takýto include: "#include <libastfri-text/inc/ASTVisitor.hpp>"
3) Vytvor si inštanciu ASTVisitor-a: "std::unique_ptr<ASTVisitor> visitor = std::make_unique<ASTVisitor>();"
4) Inštancií pošli správu visit s ľubovoľným vrcholom: "visitor.visit(astfri::VoidType);"
5) Zavolaj metódu: "visitor->write_file();"

Program funguje aj bez konfiguračného súboru, vtedy sa použije predvolený formát.
Konfigurácia(formátovanie výstupu) je plne ošetrená voči chybám v konfiguračnom súbore. Konfiguráciu je možné nastaviť pre ľubovoľné parametre, podmienka je zachovanie "hierarchie" tohto súboru. Na samotnom poradí nezáleží.

Vstupný konfiguračný súbor je buď možné použiť predvolený v priečinku impl, alebo si môžeš vytvoriť svoj(program sa ťa opýta na cestu ku .json súboru).
Cestu k výstupu si buď zadáš cez konfigurák, alebo sa použije predvolená cesta. Tá ukazuje buď priamo do priečinka "build", ak taký v ceste ku programu máš, a ak nie, tak sa použije adresár "/home/USER/", kde USER je aktuálne prihlásený používateľ. Pri hocijakej chybe sa taktiež použije predvolená cesta.

Takto vyzerá kompletný konfiguračný súbor. Všetko toto sa dá modifikovať.
{
    "CONFIGURATOR":{
        "FILE":{
            "name":"suborik",
            "path":"/mnt/c/users/marek/desktop/",
            "format":"html"
        },
        "DEFAULT_STYLE":"line-height:1.1;font-family:Consolas;font-size:23px;background-color:#f7f9f9",
        "UNKNOWN_WORD":"NEZNÁMY VÝRAZ",
        "UNKNOWN_WORD_STYLE":"color:",
        "ACCESS_MOD":{
            "view":"inner",
            "public":"public",
            "private":"private",
            "protected":"protected",
            "style":"color:#1d8348"
        },
        "DATA_TYPE":{
            "dynamic":"dynamický typ",
            "int":"celé číslo",
            "float":"desatinné číslo",
            "char":"znak",
            "bool":"logická hodnota",
            "void":"prázdny typ",
            "STYLE":{
                "general_style":"color:blue",
                "dynamic_type_style":"color:",
                "int_type_style":"color:",
                "float_type_style":"color:",
                "char_type_style":"color:",
                "bool_type_style":"color:",
                "void_type_style":"color:",
                "user_type_style":"color:brown"
            }
        },
        "REF_NAME_STYLE":{
            "def_ref_name_style":"color:",
            "gen_param_name_style":"color:",
            "gen_param_constr_style":"color:",
            "class_name_style":"color:forestgreen",
            "method_name_style":"color:orange",
            "function_name_style":"color:",
            "global_var_name_style":"color:",
            "member_var_name_style":"color:#a04000",
            "local_var_name_style":"color:",
            "param_var_name_style":"color:#9c640c"
        },
        "OPERATOR":{
            "assign":"<-",
            "style":"font-weight:bold"
        },
        "SEPARATOR":{
            "style":"color:"
        },
        "VALUE":{
            "true_val":"pravda",
            "false_val":"nepravda",
            "null_val":"NULL",
            "STYLE":{
                "general_val_style":"color:#d2b4de",
                "int_val_style":"color:#7d3c98",
                "float_val_style":"color:",
                "char_val_style":"color:",
                "string_val_style":"color:",
                "bool_val_style":"color:",
                "null_val_style":"color:"
            }
        }
    },
    "TEXT_CONFIGURATOR":{
        "STRUCT_SETTING":{
            "show_row_num":true,
            "show_op_br_new_line":false,
            "show_other_exprs":true,
            "show_class_decl":true,
            "show_class_body":true,
            "show_attribute":true,
            "show_method_body":true,
            "show_method_decl":true,
            "show_method_defin":true,
            "show_method_defin_inline":false,
            "show_function_decl":true,
            "show_function_body":true,
            "tab_word_length":8,
            "margin_left":6,
            "row_num_style":"color:grey"
        },
        "SYSTEM_EXPR":{
            "class_word":"trieda",
            "this_word":"tento objekt",
            "return_word":"vráť",
            "throw_word":"vyhoď výnimku",
            "if_word":"ak platí",
            "else_word":"inak vykonaj",
            "do_word":"vykonaj",
            "while_word":"kým platí",
            "for_word":"pre",
            "repeat_word":"opakuj",
            "switch_word":"ak má",
            "case_word":"hodnotu",
            "STYLE":{
                "general_style":"color:",
                "class_word_style":"color:red",
                "this_word_style":"color:",
                "return_word_style":"color:#943126;font-weight:bold",
                "throw_word_style":"color:",
                "if_word_style":"color:",
                "else_word_style":"color:",
                "do_word_style":"color:",
                "while_word_style":"color:",
                "for_word_style":"color:",
                "repeat_word_style":"color:",
                "switch_word_style":"color:",
                "case_word_style":"color:"
            }
        },
        "OTHER_EXPR":{
            "method_word":"metóda",
            "function_word":"funkcia",
            "lambda_word":"lambda",
            "call_word":"zavolaj operáciu",
            "define_word":"definuj",
            "returns_word":"navracia",
            "STYLE":{
                "general_style":"color:",
                "method_word_style":"color:",
                "function_word_style":"color:",
                "lambda_word_style":"color:",
                "call_word_style":"color:",
                "define_word_style":"color:",
                "returns_word_style":"color:"
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