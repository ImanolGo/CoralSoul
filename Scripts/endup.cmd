Taskkill /IM powershell.exe /F

PowerShell -Command "Set-ExecutionPolicy Unrestricted" >> "%TEMP%\StartupLog.txt" 2>&1 PowerShell C:\Users\user\CoralSoul\Scripts\CloseAll.ps1 >> "%TEMP%\StartupLog.txt" 2>&1