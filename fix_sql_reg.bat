@echo off
set LOGFILE=E:\23\sql_reg_fix.log
echo === SQL Server Registry Fix %date% %time% === > %LOGFILE%

REM Check admin
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo Please run as Administrator! >> %LOGFILE%
    echo Please run as Administrator!
    pause
    exit /b 1
)

echo 1. Creating Parameters registry key... >> %LOGFILE%
reg add "HKLM\SOFTWARE\Microsoft\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQLServer\Parameters" /f >> %LOGFILE% 2>&1

echo 2. Adding SQLArg0 (master.mdf path)... >> %LOGFILE%
reg add "HKLM\SOFTWARE\Microsoft\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQLServer\Parameters" /v SQLArg0 /t REG_SZ /d "-dC:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA\master.mdf" /f >> %LOGFILE% 2>&1

echo 3. Adding SQLArg1 (ERRORLOG path)... >> %LOGFILE%
reg add "HKLM\SOFTWARE\Microsoft\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQLServer\Parameters" /v SQLArg1 /t REG_SZ /d "-eC:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Log\ERRORLOG" /f >> %LOGFILE% 2>&1

echo 4. Adding SQLArg2 (mastlog.ldf path)... >> %LOGFILE%
reg add "HKLM\SOFTWARE\Microsoft\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQLServer\Parameters" /v SQLArg2 /t REG_SZ /d "-lC:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA\mastlog.ldf" /f >> %LOGFILE% 2>&1

echo 5. Verifying parameters... >> %LOGFILE%
reg query "HKLM\SOFTWARE\Microsoft\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQLServer\Parameters" >> %LOGFILE% 2>&1

echo 6. Attempting to start service... >> %LOGFILE%
net start MSSQLSERVER >> %LOGFILE% 2>&1
echo. >> %LOGFILE%

echo 7. Final service state: >> %LOGFILE%
sc query MSSQLSERVER >> %LOGFILE% 2>&1

echo. >> %LOGFILE%
echo 8. Last 10 lines of ERRORLOG: >> %LOGFILE%
if exist "C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Log\ERRORLOG" (
    powershell -Command "Get-Content 'C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Log\ERRORLOG' -Tail 10" >> %LOGFILE% 2>&1
)

echo Done >> %LOGFILE%
type %LOGFILE%
pause
