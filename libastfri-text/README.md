Vytvoril som plne funkčnú prehliadku AST s výpisom kódu do konzoly alebo do súboru podľa výberu. Používaj main.cpp v adresári impl. V tomto istom adresári sa vytvorí aj súbor s výstupným "pseudokódom".

Pre spustenie si najprv zostav ľubovoľný strom.

Ak chceš použiť štandardný vstup, vytvor si TranslationUnit, a tú následne pošli ako parameter v metóde visit v CodeVisitor štruktúre.

Ak chceš skúšat rôzne iné vstupy, tak jednotlivé vrcholy musíš "acceptovať" pomocou CodeVisitora. V takom prípade nezabudni medzi jednotlivými accept príkazmi vypísať nový riadok(tzn. do basicOutput stringu vlož \n).



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