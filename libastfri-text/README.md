# astfri-text
 - Converts ast into `java` code.
 - Input astfri element must be `TranslationUnit`.
 - Source files are generated in folder `build/text_output/src/`.

## Library usage

```cpp
#include <libastfri-text/inc/TextLibManager.hpp>

using namespace astfri::text;

int main()
{
    TextLibManager& manager = TextLibManager::get_instance();
    TranslationUnit tu; // create ast here
    manager.visit(tu);
}
```