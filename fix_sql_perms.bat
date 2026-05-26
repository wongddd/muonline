@echo off
set LOGFILE=E:\23\sql_fix.log
echo === SQL Server Permissions Fix %date% %time% === > %LOGFILE%

echo 1. Checking current permissions on DATA directory... >> %LOGFILE%
icacls "C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA" >> %LOGFILE% 2>&1
echo. >> %LOGFILE%

echo 2. Checking current permissions on LOG directory... >> %LOGFILE%
icacls "C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Log" >> %LOGFILE% 2>&1
echo. >> %LOGFILE%

echo 3. Granting NETWORK SERVICE access to DATA directory... >> %LOGFILE%
icacls "C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA" /grant "NT AUTHORITY\NETWORK SERVICE:(OI)(CI)F" /T >> %LOGFILE% 2>&1
echo. >> %LOGFILE%

echo 4. Granting NETWORK SERVICE access to LOG directory... >> %LOGFILE%
icacls "C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Log" /grant "NT AUTHORITY\NETWORK SERVICE:(OI)(CI)F" /T >> %LOGFILE% 2>&1
echo. >> %LOGFILE%

echo 5. Granting NETWORK SERVICE access to Binn directory... >> %LOGFILE%
icacls "C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Binn" /grant "NT AUTHORITY\NETWORK SERVICE:(OI)(CI)RX" /T >> %LOGFILE% 2>&1
echo. >> %LOGFILE%

echo 6. Attempting to start service... >> %LOGFILE%
net start MSSQLSERVER >> %LOGFILE% 2>&1
echo. >> %LOGFILE%

echo 7. Final service state: >> %LOGFILE%
sc query MSSQLSERVER >> %LOGFILE% 2>&1
echo. >> %LOGFILE%

echo 8. Checking ERRORLOG for new entries... >> %LOGFILE%
if exist "C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Log\ERRORLOG" (
    type "C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\Log\ERRORLOG" >> %LOGFILE% 2>&1
)
echo. >> %LOGFILE%
echo Done >> %LOGFILE%
