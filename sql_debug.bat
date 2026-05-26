@echo off
set LOGFILE=E:\23\sql_debug.log
echo === SQL Server Debug %date% %time% === > %LOGFILE%

echo Checking DATA directory... >> %LOGFILE%
dir "C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA" >> %LOGFILE% 2>&1

echo. >> %LOGFILE%
echo Checking ERRORLOG... >> %LOGFILE%
if exist "C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Log\ERRORLOG" (
    type "C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Log\ERRORLOG" >> %LOGFILE% 2>&1
) else (
    echo ERRORLOG not found >> %LOGFILE%
)

echo. >> %LOGFILE%
echo Checking registry... >> %LOGFILE%
reg query "HKLM\SYSTEM\CurrentControlSet\Services\MSSQLSERVER" >> %LOGFILE% 2>&1

echo. >> %LOGFILE%
echo Trying to start service... >> %LOGFILE%
net start MSSQLSERVER >> %LOGFILE% 2>&1

echo. >> %LOGFILE%
echo Service state: >> %LOGFILE%
sc query MSSQLSERVER >> %LOGFILE% 2>&1

echo Done >> %LOGFILE%
