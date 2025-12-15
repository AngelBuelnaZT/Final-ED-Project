@echo off
cls
echo ==========================================
echo   MINI FILE SYSTEM - DEMO AUTOMATICA
echo ==========================================

:: 1. Limpieza
if exist filesystem.json del filesystem.json
if exist filesystem.exe del filesystem.exe

:: 2. Compilacion
echo Compilando...
g++ main.cpp -o filesystem.exe

if %errorlevel% neq 0 (
    echo Error de compilacion.
    pause
    exit /b
)

:: 3. Ejecucion con Redireccion de Entrada
echo Ejecutando guion de prueba desde input.txt...
echo ---------------------------------------------
:: El signo '<' hace que el programa lea del archivo en vez del teclado
filesystem.exe < input.txt
echo ---------------------------------------------
echo Demo finalizada.
pause