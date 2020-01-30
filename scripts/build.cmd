@echo off
REM echo running build.cmd

set ino_path=%1
set ino_name=%2
REM set serial_port=%3 REM TODO

set arduino_debug="C:\Program Files (x86)\Arduino\arduino_debug.exe"
set arduino="C:\Program Files (x86)\Arduino.exe"

REM Include board information here
set board_info=arduino:mbed:nano33ble

cd %ino_path%
REM cmd /K %arduino_debug% --pref build.path=./build --board %board_info% --verify %ino_name%
%arduino_debug% --pref build.path=./build --board %board_info% --verify %ino_name%

REM return the error code
if errorlevel 0 (
  echo Build Successful
  Exit /B 0
)
if errorlevel 1 (
  echo Build Failed
  Exit /B 1
)
if errorlevel 2 (
  echo Sketch not found
  Exit /B 2
)
if errorlevel 3 (
  echo Invalid Command Line Arg
  Exit /B 3
)
if errorlevel 4 (
  echo Preference passed to --get-pref does not exist
  Exit /B 4
)

