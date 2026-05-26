@echo off
title MU Server Launcher
cd /d E:\23\MuServer

echo ============================================
echo  MU Online Private Server Launcher
echo ============================================
echo.

REM Step 0: Check SQL Server
echo [0/4] Checking SQL Server...
sc query MSSQLSERVER | findstr RUNNING > nul
if %errorLevel% neq 0 (
    echo   SQL Server is not running! Attempting to start...
    net start MSSQLSERVER
    timeout /t 5 /nobreak > nul
) else (
    echo   [OK] SQL Server is running.
)
echo.

REM Step 1: Start DataServer
echo [1/4] Starting DataServer (port 55960)...
start "DataServer" /min "E:\23\MuServer\DataServer\DataServer.exe"
timeout /t 3 /nobreak > nul
echo.

REM Step 2: Start JoinServer
echo [2/4] Starting JoinServer (port 55970)...
start "JoinServer" /min "E:\23\MuServer\JoinServer\JoinServer.exe"
timeout /t 3 /nobreak > nul
echo.

REM Step 3: Start ConnectServer
echo [3/4] Starting ConnectServer (port 44405)...
start "ConnectServer" /min "E:\23\MuServer\ConnectServer\ConnectServer.exe"
timeout /t 3 /nobreak > nul
echo.

REM Step 4: Start GameServer
echo [4/4] Starting GameServer (port 55901)...
start "GameServer" "E:\23\MuServer\GameServer\GameServer.exe"
echo.
echo All servers launched. Check each window for status.
echo.
echo Press any key to show running processes...
pause > nul

echo.
echo === Running MU Server Processes ===
tasklist | findstr /i "DataServer JoinServer ConnectServer GameServer"
echo.
pause
