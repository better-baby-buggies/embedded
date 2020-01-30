@echo off
REM Adjust these first two variables to match the file you want to upload
REM Example would be 
REM set ino_path=./tests/example_module_test
REM set ino_name=example_module_test.ino

REM ~~~~~~~~~~ Edit Here ~~~~~~~~~~~~~~~~~~~

set ino_path=./tests/example_module_test
set ino_name=example_module_test.ino

REM ~~~~~~~~~~ Don't edit below this ~~~~~~~

if "%1"=="c" (
  REM This works and closes after
  ".\scripts\upload.cmd" %cd%/%ino_path% %ino_name%
  Exit /B 0
)

cmd /K ".\scripts\upload.cmd" %cd%/%ino_path% %ino_name%
