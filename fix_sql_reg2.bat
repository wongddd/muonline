@echo off
set LOGFILE=E:\23\sql_reg_fix2.log
echo === SQL Server Registry Fix v2 %date% %time% === > %LOGFILE%

net session >nul 2>&1
if %errorLevel% neq 0 (
    echo Please run as Administrator!
    pause
    exit /b 1
)

REM Delete old parameters first
echo 1. Removing old parameters... >> %LOGFILE%
reg delete "HKLM\SOFTWARE\Microsoft\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQLServer\Parameters" /va /f >> %LOGFILE% 2>&1

REM Create with quoted paths
echo 2. Adding SQLArg0 with quoted path... >> %LOGFILE%
reg add "HKLM\SOFTWARE\Microsoft\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQLServer\Parameters" /v SQLArg0 /t REG_SZ /d "-dC:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA\master.mdf" /f >> %LOGFILE% 2>&1

echo 3. Adding SQLArg1 with quoted path... >> %LOGFILE%
reg add "HKLM\SOFTWARE\Microsoft\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQLServer\Parameters" /v SQLArg1 /t REG_SZ /d "-eC:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Log\ERRORLOG" /f >> %LOGFILE% 2>&1

echo 4. Adding SQLArg2 with quoted path... >> %LOGFILE%
reg add "HKLM\SOFTWARE\Microsoft\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQLServer\Parameters" /v SQLArg2 /t REG_SZ /d "-lC:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA\mastlog.ldf" /f >> %LOGFILE% 2>&1

echo 5. Verifying parameters... >> %LOGFILE%
reg query "HKLM\SOFTWARE\Microsoft\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQLServer\Parameters" >> %LOGFILE% 2>&1

REM Check if the service reads parameters from the ImagePath in the service config
echo. >> %LOGFILE%
echo 6. Checking service ImagePath registry... >> %LOGFILE%
reg query "HKLM\SYSTEM\CurrentControlSet\Services\MSSQLSERVER" /v ImagePath >> %LOGFILE% 2>&1

REM Also set the ImagePath with full arguments as a fallback
echo. >> %LOGFILE%
echo 7. Updating service ImagePath with explicit parameters... >> %LOGFILE%
sc config MSSQLSERVER binPath= "C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Binn\sqlservr.exe -sMSSQLSERVER -dC:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA\master.mdf -eC:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Log\ERRORLOG -lC:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA\mastlog.ldf" >> %LOGFILE% 2>&1

echo. >> %LOGFILE%
echo 8. Attempting to start service... >> %LOGFILE%
net start MSSQLSERVER >> %LOGFILE% 2>&1
echo. >> %LOGFILE%

echo 9. Final service state: >> %LOGFILE%
sc query MSSQLSERVER >> %LOGFILE% 2>&1

echo. >> %LOGFILE%
echo 10. Checking ERRORLOG for new entries: >> %LOGFILE%
if exist "C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Log\ERRORLOG" (
    powershell -Command "Get-Content 'C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Log\ERRORLOG' -Tail 15" >> %LOGFILE% 2>&1
)

echo Done >> %LOGFILE%
type %LOGFILE%
pause
