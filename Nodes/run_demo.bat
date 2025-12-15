@echo off
cls
echo ==========================================
echo      MINI FILE SYSTEM - BUILDER
echo ==========================================

:: 1. DEFINIR LA RUTA DEL COMPILADOR (¡PEGA TU RUTA AQUI!)
:: Ejemplo: set COMPILER="C:\Program Files\CodeBlocks\MinGW\bin\g++.exe"
set COMPILER="C:\Ruta\Donde\Encontraste\El\Bin\g++.exe"

:: 2. Limpieza
echo [1/3] Limpiando...
if exist filesystem.json del filesystem.json
if exist filesystem.exe del filesystem.exe

:: 3. Compilacion (Usando la ruta explicita)
echo [2/3] Compilando con ruta especifica...
%COMPILER% main.cpp -o filesystem.exe

if %errorlevel% neq 0 (
    echo [ERROR] No se encontro el compilador o hubo error de codigo.
    echo Verifica que la ruta en 'set COMPILER' sea correcta.
    pause
    exit /b
)

:: 4. Ejecucion
echo [3/3] Iniciando programa...
filesystem.exe
pause