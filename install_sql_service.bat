@echo off
echo =============================================
echo  SQL Server 2008 R2 Express Service Installer
echo =============================================
echo.

REM Check if running as admin
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo ERROR: Please run this script AS ADMINISTRATOR.
    echo Right-click and select "Run as administrator".
    pause
    exit /b 1
)

echo [1/4] Creating MSSQLSERVER service...
sc create MSSQLSERVER binPath= "C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Binn\sqlservr.exe -sMSSQLSERVER" start= auto DisplayName= "SQL Server (MSSQLSERVER)"
if %errorLevel% neq 0 (
    echo WARNING: sc create returned %errorLevel%, service may already exist
) else (
    echo [OK] Service created
)

echo.
echo [2/4] Setting service account (NETWORK SERVICE)...
sc config MSSQLSERVER obj= "NT AUTHORITY\NETWORK SERVICE" start= auto
if %errorLevel% neq 0 (
    echo WARNING: Could not set service account
) else (
    echo [OK] Service account configured
)

echo.
echo [3/4] Starting MSSQLSERVER service...
net start MSSQLSERVER
if %errorLevel% neq 0 (
    echo.
    echo ERROR: Service failed to start! Checking error log...
    if exist "C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Log\ERRORLOG" (
        echo --- Last 20 lines of ERRORLOG ---
        powershell -Command "Get-Content 'C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Log\ERRORLOG' -Tail 20"
    ) else (
        echo ERRORLOG not found.
    )
    pause
    exit /b 1
) else (
    echo [OK] Service started successfully
)

echo.
echo [4/4] Verifying SQL Server is listening...
powershell -Command "Start-Sleep -Seconds 3"
netstat -an | findstr 1433
if %errorLevel% equ 0 (
    echo [OK] SQL Server is listening on port 1433
) else (
    echo WARNING: SQL Server might not be listening on port 1433 yet
)

echo.
echo =============================================
echo  SQL Server installation summary:
echo.
sc query MSSQLSERVER | findstr STATE
echo =============================================

pause
