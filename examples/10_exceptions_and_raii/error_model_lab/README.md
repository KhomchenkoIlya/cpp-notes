# error_model_lab

Одна операция чтения и записи показана с явным `Status` и с exceptions.
`TemporaryReport` — небольшой non-copyable RAII-owner: он публикует файл только
через `commit()`, а destructor удаляет временный файл на return или unwinding.

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
./build/error_model_lab unwind build
./build/error_model_lab rethrow build
```
