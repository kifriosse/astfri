
1) Make build. Library is called "astfri-text".
2) Include header file: "#include <libastfri-text/inc/TextLibManager.hpp>".
3) Create TextLibManager object: TextLibManager& man = TextLibManager::get_instance();
4) Now you can use this library.

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DASTFRI_BUILD_TEXT_OUTPUT=ON ..
make
cp compile_commands.json ..