Vytvoril som plne funkčnú prehliadku AST s výpisom kódu do konzoly alebo do súboru. Pri výpise sa použije jednoduchý .json konfiguračný súbor. Používaj main.cpp v adresári impl. V tomto istom adresári musí byť vstupný .json súbor. Výstupný súbor bude umiestnený v priečinku build.

Pre spustenie si najprv zostav ľubovoľný strom v podadresári inl/ASTLoader, a vo funkcií main pošli ASTVisitora ľubovoľnej funkcií.

Konfigurácia(formátovanie výstupu) je plne ošetrená voči chybám v konfiguračnom súbore. Konfiguráciu je možné nastaviť pre ľubovoľné parametre, podmienka je zachovanie "hierarchie" tohto súboru. Na samotnom poradí nezáleží.

Zatiaľ je plne funkčný iba výpis do txt súborov.


Príklad s kompletným konfiguračným súborom(toto všetko sa dá modifikovať).
{
    "general_output":{
        "file_name":"suborik",
        "type":"text",
        "view":"vnutorny",
        "data_types":{
            "int":"CELECISLO",
            "float":"DESATINA",
            "char":"ZNAK",
            "bool":"BOOLEAN",
            "void":"VOID"
        },
        "bin_ops":{
            "assign":"<-"
        },
        "access_mods":{
            "private":"súkromné",
            "public":"verejné"
        }
    },
    "basic_format":{
        "format":"txt",
        "indentation":"   ",
        "show_class_body":true,
        "show_method_body":true,
        "show_method_definition":true,
        "show_function_body":true,
        "open_bracket_new_line":true
    },
    "system_names":{
        "class":"TRIEDA",
        "if":"AK",
        "else":"INAK",
        "for":"PRE",
        "while":"POKIAL",
        "do":"ROB",
        "return":"VRÁŤ",
        "throw":"VYHOĎ",
        "switch":"VYBER",
        "case":"PRÍPAD",
        "this":"THIS"
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