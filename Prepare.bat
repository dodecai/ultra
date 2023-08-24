::: Properties
    @echo off
    cls
    cd /d "%~dp0"
    chcp 65001 >nul

::: Main
    pwsh -ExecutionPolicy Bypass -File "%~dp0Scripts\%~n0.ps1" %*

::: Terminate
    exit /b %ErrorLevel%
