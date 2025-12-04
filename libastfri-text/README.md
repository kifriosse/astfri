# astfri-text
 - Converts ast into `txt` (default) or `java` code.
 - Input astfri element must be `TranslationUnit`.
 - Source files are generated in folder `build/text_output/src/`.

## Library usage

```cpp
#include <libastfri-text/inc/TextLibManager.hpp>

using namespace astfri::text;

int main()
{
    TranslationUnit tu; // create ast here
    TextLibManager& manager = TextLibManager::get_instance();
    manager.visit_and_export(tu);
}
```


 - To switch between txt or java use this.
```cpp
    manager.change_output(OutputFormat::Pseudocode);
    manager.change_output(OutputFormat::JavaCode);
```