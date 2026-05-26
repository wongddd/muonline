@echo off
echo ============================================
echo Checking SQL Server Error Log
echo ============================================

echo 1. Attempting to start service...
net start MSSQLSERVER 2>&1
echo.
echo 2. Checking ERRORLOG...
if exist "C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Log\ERRORLOG" (
    echo --- ERRORLOG (last 30 lines) ---
    powershell -Command "Get-Content 'C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Log\ERRORLOG' -Tail 30"
) else (
    echo ERRORLOG file not found.
)
echo.
echo 3. Checking DATA directory...
dir "C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA" 2>&1
echo.
echo 4. Checking install status...
sc query MSSQLSERVER 2>&1
echo.
echo 5. Attempting to start in console mode to see errors...
"C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Binn\sqlservr.exe" -sMSSQLSERVER -c 2>&1
pause
