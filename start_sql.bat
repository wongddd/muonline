@echo off
echo Starting SQL Server (MSSQLSERVER) service...
net start MSSQLSERVER
if %errorLevel% equ 0 (
    echo [OK] SQL Server started successfully
    echo.
    echo Checking port 1433...
    netstat -an | findstr 1433
) else (
    echo.
    echo [FAILED] Could not start service
    echo Checking ERRORLOG for details...
    if exist "C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Log\ERRORLOG" (
        type "C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Log\ERRORLOG"
    ) else (
        echo ERRORLOG not found.
    )
)
pause
