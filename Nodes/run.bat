@echo off
cls
echo ==========================================
echo      MINI FILE SYSTEM - BUILDER
echo ==========================================

:: 1. Limpieza de archivos anteriores
echo [1/3] Limpiando archivos temporales...
if exist filesystem.json del filesystem.json
if exist filesystem.exe del filesystem.exe

:: 2. Compilacion
echo [2/3] Compilando codigo fuente...
g++ main.cpp -o filesystem.exe

:: Verificar si hubo error al compilar
if %errorlevel% neq 0 (
    echo [ERROR] La compilacion fallo. Revisa tu codigo.
    pause
    exit /b
)

:: 3. Ejecucion
echo [3/3] Iniciando programa...
echo.
filesystem.exe
pause