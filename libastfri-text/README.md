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

Príklad s kompletným konfiguračným súborom(toto všetko sa dá modifikovať).
{
    "general_output":{
        "file_name":"suborik",
        "file_path":"/mnt/c/users/marek/desktop/",
        "view":"vnutorny",
        "data_types":{
            "int":"CELECISLO",
            "float":"FLOAT",
            "char":"ZNAK",
            "bool":"BOOLEAN",
            "void":"VOID"
        },
        "bin_ops":{
            "assign":"<-"
        },
        "access_mods":{
            "public":"PUBLIC",
            "private":"PRIVATE",
            "protected":"PROTECTED"
        },
        "formatting":{
            "font":"Consolas",
            "class_word_color":"red",
            "class_name_color":"forestgreen",
            "method_name_color":"orange",
            "data_type_color":"blue",
            "acc_mods_color":"#53fc18"
        }
    },
    "basic_format":{
        "format":"html",
        "show_row_num":true,
        "show_op_br_new_line":true,
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
        "row_num_size":17
    },
    "system_names":{
        "class":"TRIEDA",
        "this":"THIS",
        "return":"VRÁŤ",
        "throw":"VYHOĎ",
        "if":"AK PLATÍ",
        "else":"INAK VYKONAJ",
        "for":"PRE",
        "while":"KÝM PLATÍ",
        "do":"VYKONAJ",
        "switch":"VYBER",
        "case":"PRÍPAD"
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