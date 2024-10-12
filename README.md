# astfri v1.0.0

Táto knižnica je napísaná v jazyku C++ a obsahuje štruktúry pre univerzálnu reprezentáciu zdrojového kódu a nástroje pre vstupnú a výstupnú manipuláciu s týmito štruktúrami.

## Modules

### libastfri

Obsahuje štruktúry pre reprezentáciu zdrojového kódu. Tieto štruktúry sú nezávislé na konkrétnom jazyku a slúžia na univerzálnu reprezentáciu zdrojového kódu. Tiež obashuje definícií rozhraní pre analýzy/transformácie, formátovanie a zápis spracovaných štruktúr.

#### Dependencies
The core module does not have any dependencies -- besides standard library.

#### Examples
TODO

### libastfri-output

Obsahuje implementáciu rozhraní pre výstupné formáty analýz a transformácií. Tento modul je závislý na module `libastfri`.

Implementované **transformačné Visitor-y** (`IOutputVisitor`):
- `CodeVisitor` - prechádza stromom a vykonáva transformácie štruktúr do všeoobecne známeného syntaxu podoobného C++.
- `PseudocodeVisitor` - prechádza stromom a vykonáva transformácie štruktúr do pseudokódu v slovenčine. Špecifikácia pseudokódu vychádza z pseudokódu použitého v predmete AUS1 na FRI UNIZA.

Implementované **formátovače** (`IOutputFormatter`):
- `PlainTextFormatter` - formátuje výstup do čistého textového formátu

Implementované **zapisovače** (`IOutputWriter`):
- `StreamWriter` - zapisuje výstup do `std::ostream` streamu

#### Dependencies
TODO

#### Examples
TODO

### libastfri-cpp

Obsahuje implementáciu parseru pre načítanie zdrojového kódu v jazyku C++ do štruktúr z modulu `libastfri`. 

Tento modul je závislý na module `libastfri` a zároveň na balíčkoch [`LLVM` a `Clang`](https://llvm.org/docs/GettingStartedTutorials.html), pozri  [CMakeLists.txt](./libastfri-cpp/CMakeLists.txt).

#### Dependencies
TODO

#### Examples
TODO

## Build and install
TODO

## For Developers
- C++ standard is C++20  
- Class names follow upper [cammel case](https://en.wikipedia.org/wiki/Camel_case) notation, e.g. `class ForLoop`  
- Function names follow [snake case](https://en.wikipedia.org/wiki/Snake_case) notation, e.g. `create_int`  
- Variable names follow lower [cammel case](https://en.wikipedia.org/wiki/Camel_case) notation, e.g. `variableName`  
- Member variables use underscore as suffix, e.g. `statements_`  
- Code must compile with at least `-Wall -Wextra -Wpedantic` with no warnings  
- Code formatting is ensured by `clang-format` configured in `.clang-format` (for now, it has to be invoked manually -- we will change it to pre-commit hook)