Fully functional library for generating pseudocode from AST. Each input is printed into separated output file. If file exists, it will create new file with index.
Example: If file "output.txt" exists, it will create files "output(1).txt" and so on.

Before use, you need to do the following:
1) Make build. Library is called "astfri-text".
2) Include header file: "#include <libastfri-text/inc/ASTVisitor.hpp>".
3) Create an instantiation of ASTVisitor: "ASTVisitor* visitor = new ASTVisitor();".
4) It will also create configuration, you will be asked for input json file.
5) Call "visit" method with any AST peak that is visitable: "visitor->visit(...);".
6) After visit you need to call method "write_file": "visitor->write_file();".

There is no need to create multiple instantiations of ASTVisitor. After any write_file is done, program will be ready for next input. So basically it is alternation of just 2 methods - visit and write_file. If you visit peak and you do not want to write it anywhere, just call method "visitor->reset_exporter()". And again: visit -> write ... and so on. Also you can change your input configuration .json file while program is running by simply call method "visitor->reset_configurator()".

You don't have to use input json file, default output settings will be used. If you set [ "format":"html" ] in json file, output will be created as .html file. In any other case it will be .txt file. Bear in mind that any '/' or ' ' (spaces) in [ "name":"..." ] will be removed. Do not use file suffix in there also.
For output path there are multiple options: ... [ "path":"..." ], any ' ' (spaces) and multiple '//...' will be removed.
1) Default path is either a) .../build/text_output/ - if build directory is present or b) /home/USER/text_output/ - if build dir is not present. USER refers to current logged user in linux.
2) If "path" is not present or "path":"build" -> output path will be same as default path.
3) If "path":"desktop" -> path will be /mnt/c/Users/USER/Desktop/text_output/, where USER refers to current logged user in windows.
4) If "path" starts with '/', output file will be placed exactly where it refers if it is possible. If not, default path will be used. There will not be any text_output folder created.
5) If "path":"" or does not start with '/', it is relative path from where the program runs. It will also create text_output folder.
6) If there is any error in path, default path will be used.
Don't worry about finding output file, it will print to console where it is being created.

Feel free to create your own json configuration file.

This is example of json file with default settings.
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