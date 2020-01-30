@echo off
REM Adjust these first two variables to match the file you want to build
REM Example would be 
REM set ino_path=./tests/example_module_test
REM set ino_name=example_module_test.ino

REM ~~~~~~~~~~ Edit Here ~~~~~~~~~~~~~~~~~~~

set ino_path=./tests/example_module_test
set ino_name=example_module_test.ino

REM ~~~~~~~~~~ Don't edit below this ~~~~~~~

REM if %1 == [] (
REM if exist %1 (
if "%1"=="c" (
  REM This works and closes after
  ".\scripts\build.cmd" %cd%/%ino_path% %ino_name%
  Exit /B 0
)

cmd /K ".\scripts\build.cmd" %cd%/%ino_path% %ino_name%
