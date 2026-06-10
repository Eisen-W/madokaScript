# madokaScript
my custom scripting language because why not

compile on Linux
``` shell
cmake .
make
./mad <file.mad>
```

compile on Windows:
``` shell
cmake -B build-win -DCMAKE_TOOLCHAIN_FILE=toolchain-w64.cmake
cmake --build build-win
cd build-win
mad.exe <file.mad>
```
or just use the prebuilt .exe to avoid windows installation hell

### Commands
- SAY <br>
prints out strings and values
