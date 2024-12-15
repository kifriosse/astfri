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