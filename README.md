# StrataBro

## Introduction
StrataBro is a simple tool built with Qt Widgets, designed for the game Helldivers 2.  
The purpose of the tool is to automate stratagem callouts in the game, by sending keypresses to the game.  

![StrataBro_MainWindow](https://github.com/user-attachments/assets/d45e43c5-238d-4a7f-8bdc-fc118c846108)

https://github.com/user-attachments/assets/81c4ccf1-c0dd-4b0b-bf3a-e7b774d0585c

TODO

## How to build
### Prerequisites
TODO

### Compile
Assume **QT_ROOT_DIR=C:\Qt\6.8.2\msvc2022_64** in my case. This is your QT root directory.
1. Clone the repo 
```bash 
git clone https://github.com/stdNullPtr/StrataBro.git 
```
2. Cmake generate (using "build" dir for the example)
```bash
cmake -DCMAKE_PREFIX_PATH=${QT_ROOT_DIR} -S . -B build
```
3. Cmake build
```bash
cmake --build build --config Release
```
4. Cmake deploy dependencies
```bash
${QT_ROOT_DIR}\bin\windeployqt.exe --release --no-compiler-runtime --no-translations build\Release\StrataBro.exe
```
5. You can find the executable at *build\Release\StrataBro.exe*

Note: have a look at [this workflow](.github/workflows/build.yml) for reference on how a build is done.

## How to use
TODO
