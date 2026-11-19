@echo off
setlocal
cd /d "%~dp0"
where doxygen >nul 2>nul
if errorlevel 1 (
    echo Erro: instale o Doxygen e adicione-o ao PATH.
    exit /b 1
)
where dot >nul 2>nul
if errorlevel 1 (
    echo Erro: instale o Graphviz e adicione a pasta bin ao PATH.
    exit /b 1
)
doxygen Doxyfile
if errorlevel 1 exit /b 1
if not exist docs\html\index.html (
    echo Erro: a pagina inicial nao foi gerada.
    exit /b 1
)
echo Documentacao: docs\html\index.html
start "" "%~dp0docs\html\index.html"
endlocal
