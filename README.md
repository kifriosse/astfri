# astfri v1.0.0

Táto knižnica je napísaná v jazyku C++ a obsahuje štruktúry pre univerzálnu reprezentáciu zdrojového kódu a nástroje pre vstupnú a výstupnú manipuláciu s týmito štruktúrami.

## Moduly knižnice

### libastfri

Obsahuje štruktúry pre reprezentáciu zdrojového kódu. Tieto štruktúry sú nezávislé na konkrétnom jazyku a slúžia na univerzálnu reprezentáciu zdrojového kódu. Tiež obashuje definícií rozhraní pre analýzy/transformácie, formátovanie a zápis spracovaných štruktúr.

Pre prácu s týmto modulom nie je potrebné inštalovať žiadne ďalšie závislosti.

#### dependencies
The core module does not have any dependencies -- besides standard library.

### libastfri-output

Obsahuje implementáciu rozhraní pre výstupné formáty analýz a transformácií. Tento modul je závislý na module `libastfri`.

Implementované **transformačné Visitor-y** (`IOutputVisitor`):
- `CodeVisitor` - prechádza stromom a vykonáva transformácie štruktúr do všeoobecne známeného syntaxu podoobného C++.
- `PseudocodeVisitor` - prechádza stromom a vykonáva transformácie štruktúr do pseudokódu v slovenčine. Špecifikácia pseudokódu vychádza z pseudokódu použitého v predmete AUS1 na FRI UNIZA.

Implementované **formátovače** (`IOutputFormatter`):
- `PlainTextFormatter` - formátuje výstup do čistého textového formátu

Implementované **zapisovače** (`IOutputWriter`):
- `StreamWriter` - zapisuje výstup do `std::ostream` streamu

#### dependencies

TODO

### libastfri-cpp

Obsahuje implementáciu parseru pre načítanie zdrojového kódu v jazyku C++ do štruktúr z modulu `libastfri`. 

Tento modul je závislý na module `libastfri` a zároveň na balíčkoch [`LLVM` a `Clang`](https://llvm.org/docs/GettingStartedTutorials.html), pozri  [CMakeLists.txt](./libastfri-cpp/CMakeLists.txt).

#### dependencies

TODO

### examples

Obsahuje príklady použitia knižnice:
- `manual_structure.cpp` - príklad manuálneho vytvorenia štruktúr zdrojového kódu a ich výpisu
- `cpp_frontend.cpp` - príklad načítania zdrojového kódu v jazyku C++ a jeho výpisu. Pre spustenie je potrebné mať nainštalované závislosti pre modul `libastfri-cpp`

#### dependencies

TODO

## Spustenie projektu

### Skompilovanie a spustenie príkladu `cpp_frontend.cpp`:
```bash
# Vytvorenie build adresára
mkdir build
cd build

# Konfigurácia projektu
cmake -DCMAKE_BUILD_TYPE=Release -DASTFRI_BUILD_CPP_FRONTEND=ON ..

make astfri-example-cpp

# Spustenie príkladu
./examples/astfri-example-cpp ../examples/input/math.cpp
```
### Skompilovanie a spustenie príkladu `manual_structure.cpp`:
```bash
# Vytvorenie build adresára
mkdir build
cd build

# Konfigurácia projektu
cmake -DCMAKE_BUILD_TYPE=Release ..

make astfri-example-manual

# Spustenie príkladu
./examples/astfri-example-manual
```