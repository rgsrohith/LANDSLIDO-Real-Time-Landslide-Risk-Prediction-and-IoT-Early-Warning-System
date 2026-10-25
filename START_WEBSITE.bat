@echo off
title LANDSLIDO
cd /d "%~dp0"
where node >nul 2>nul
if errorlevel 1 (
  echo Node.js is not installed. Download it from https://nodejs.org and run this again.
  pause
  exit /b
)
rem Change this token, and use the same value in the ESP32 firmware config.
if not defined DEVICE_TOKEN set DEVICE_TOKEN=landslido-esp32-token
start "" cmd /c "timeout /t 2 >nul & start http://localhost:3000"
node server.js
pause
