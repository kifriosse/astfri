Vytvoril som plne funkčnú prehliadku AST s výpisom kódu do konzoly alebo do súboru. Pri výpise sa použije jednoduchý .json konfiguračný súbor. Používaj main.cpp v adresári impl. V tomto istom adresári musí byť vstupný .json súbor. Výstupný súbor bude umiestnený v priečinku build.

Pre spustenie si najprv zostav ľubovoľný strom v podadresári inl/ASTLoader, a vo funkcií main pošli ASTVisitora ľubovoľnej funkcií.


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