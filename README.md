# GKOM-2024l

## Instalacja potrzebnych bibliotek

- sudo apt install -y libglfw3-dev libglew-dev libgl-dev

## Kompilacja projektu

1. cd workingDir gdzie workingDir to katalog roboczy projektu z plikiem CMakeLists.txt
2. cmake -S . -B buildDir -DINCLUDE_DIR=ścieżka/do/folderu/z/plikami/nagłówkowymi -DLIB_DIR=ścieżka/do/folderu/z/plikami/biliotek(.lib, .so) gdzie buildDir to ścieżka do katalogu, w którym ma zostać umieszczony plik Makefile, ścieżki poszukiwanych katalogów mozna znaleźć używając sudo dpkg -L nazwa_biblioteki (libglfw3 lub libglew-dev lub libgl-dev)
3. cd buildDir
4. make

## Uruchomienie projektu

1. cd buildDir
2. ./gkom24
