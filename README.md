# GKOM-2024l

## Instalacja potrzebnych bibliotek

Dla Linux

- sudo apt install -y libglfw3-dev libglew-dev libgl-dev libglm-dev

Dla Windows potrzebne biblioteki są w folderze gkom_libs

## Kompilacja projektu

1. cd workingDir gdzie workingDir to katalog roboczy projektu z plikiem CMakeLists.txt
2. cmake -S . -B buildDir -DINCLUDE_DIR=ścieżka/do/folderu/z/plikami/nagłówkowymi -DLIB_DIR=ścieżka/do/folderu/z/plikami/biliotek(.lib, .so) gdzie buildDir to ścieżka do katalogu, w którym ma zostać umieszczony plik Makefile (musi być umieszczony w podkatalogu katalogu zawierającego folder resources, najlepiej wykorzystać istniejący folder build w katalogu z CMakeLists.txt), ścieżki poszukiwanych katalogów mozna znaleźć używając sudo dpkg -L nazwa_biblioteki (libglfw3 lub libglew-dev lub libgl-dev)

W przypadku systemu Windows należy użyć polecenia: cmake -S . -B build -DINCLUDE*DIR="pozostała_część*ścieżki/gkom*includes" -DLIB_DIR="pozostała_część*ścieżki/gkom_libs" -G "MinGW Makefiles"

3. cd buildDir
4. make

Na systemie Windows trzeba zadbać aby użyć polecenia make kompilującego projekt w trybie 32 bitowym. Do tego celu należy wykorzystać MinGW dla systemu Windows w wersji 32 bitowej (w innym przypadku pojawią się błędy na etapie linkowania).

## Uruchomienie projektu

1. cd buildDir
2. ./gkom24l lub .\gkom24l.exe na systemie Windows, przy czym dodatkowo należy pamiętać o przekopiowania bibliotek \*.dll do folderu buildDir
