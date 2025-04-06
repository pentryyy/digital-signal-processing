# Параметры запуска

Пример аргументов в `tasks.json`.

```
"args": [
    "-fdiagnostics-color=always",
    "-g",
    "${file}",
    "-o",
    "${workspaceFolder}\\executable\\${fileBasenameNoExtension}.exe",
    "-I", "C:\\Users\\pentryyy\\AppData\\Local\\Programs\\Python\\Python311\\include",
    "-I", "C:\\Users\\pentryyy\\AppData\\Local\\Programs\\Python\\Python311\\Lib\\site-packages\\numpy\\_core\\include",
    "-L", "C:\\Users\\pentryyy\\AppData\\Local\\Programs\\Python\\Python311\\libs",
    "-lpython311"
],
```
