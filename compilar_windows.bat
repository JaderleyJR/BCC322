@echo off
rem SaveFood - Sprint 7: compila cada camada como DLL (MinGW-w64) e executa os testes.
rem
rem   Camada 1  Dominio      -> bin\savefood_dominio.dll
rem   Camada 2  Nucleo       -> bin\savefood_nucleo.dll    (depende de dominio)
rem   Camada 3  Servicos     -> bin\savefood_servicos.dll  (depende de nucleo e dominio)
rem   Camada 4  Apresentacao -> bin\savefood.exe
rem
rem Cada DLL gera tambem uma biblioteca de importacao (bin\lib*.dll.a) usada na ligacao.
rem Os .exe encontram as DLLs porque ficam na mesma pasta bin\.
setlocal enabledelayedexpansion
cd /d "%~dp0"

if not exist bin mkdir bin
set FLAGS=-std=c++17 -O2 -Wall -Wextra -Wpedantic -pthread
set LIBS=-Lbin -lsavefood_servicos -lsavefood_nucleo -lsavefood_dominio

echo [1/4] Camada de dominio
g++ %FLAGS% -shared -DSAVEFOOD_DOMINIO_EXPORTA src\dominio\model.cpp ^
    -o bin\savefood_dominio.dll -Wl,--out-implib,bin\libsavefood_dominio.dll.a
if errorlevel 1 exit /b 1

echo [2/4] Camada de nucleo
g++ %FLAGS% -shared -DSAVEFOOD_NUCLEO_EXPORTA src\nucleo\system.cpp src\nucleo\fabrica_sistema.cpp ^
    -Lbin -lsavefood_dominio ^
    -o bin\savefood_nucleo.dll -Wl,--out-implib,bin\libsavefood_nucleo.dll.a
if errorlevel 1 exit /b 1

echo [3/4] Camada de servicos
g++ %FLAGS% -shared -DSAVEFOOD_SERVICOS_EXPORTA src\servicos\services.cpp src\servicos\fabrica_servicos.cpp ^
    -Lbin -lsavefood_nucleo -lsavefood_dominio ^
    -o bin\savefood_servicos.dll -Wl,--out-implib,bin\libsavefood_servicos.dll.a
if errorlevel 1 exit /b 1

echo [4/4] Apresentacao e testes
g++ %FLAGS% src\main.cpp src\apresentacao\flow.cpp %LIBS% -o bin\savefood.exe
if errorlevel 1 exit /b 1

g++ %FLAGS% test\funcional\main.cpp test\funcional\funcional_tests.cpp %LIBS% -o bin\testes_funcionais.exe
if errorlevel 1 exit /b 1

set UNIT=
for %%f in (test\unit\*.cpp) do set UNIT=!UNIT! %%f
g++ %FLAGS% -UNDEBUG %UNIT% src\apresentacao\flow.cpp %LIBS% -o bin\testes_unitarios.exe
if errorlevel 1 exit /b 1

echo Compilacao concluida.
bin\testes_unitarios.exe
if errorlevel 1 exit /b 1
bin\testes_funcionais.exe
if errorlevel 1 exit /b 1

echo Todos os testes passaram.
endlocal
