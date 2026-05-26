@echo off
echo Setting MSSQLSERVER to Automatic start...
sc config MSSQLSERVER start= auto
echo.
echo Done. Service will now start automatically on boot.
pause
