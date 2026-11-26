@echo off
setlocal
cd /d "%~dp0"

if not exist bin mkdir bin
set FLAGS=-std=c++17 -O2 -Wall -Wextra -Wpedantic -pthread
set FONTES=src\model.cpp src\system.cpp src\services.cpp src\flow.cpp

g++ %FLAGS% src\main.cpp %FONTES% -o bin\savefood.exe
if errorlevel 1 exit /b 1

g++ %FLAGS% test\funcional\main.cpp test\funcional\funcional_tests.cpp %FONTES% -o bin\testes_funcionais.exe
if errorlevel 1 exit /b 1

g++ %FLAGS% -UNDEBUG test\unit\*.cpp %FONTES% -o bin\testes_unitarios.exe
if errorlevel 1 exit /b 1

echo Compilacao concluida.
bin\testes_unitarios.exe
if errorlevel 1 exit /b 1
bin\testes_funcionais.exe
if errorlevel 1 exit /b 1

echo Todos os testes passaram.
endlocal
