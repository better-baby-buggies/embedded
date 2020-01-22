REM this also works and is faster but has more dumb cmd output
cmd /K "C:\Program Files (x86)\Arduino\arduino_debug.exe" --pref build.path=./build --verify embedded.ino

REM This works to build from scratch every time
REM cmd /K "C:\Program Files (x86)\Arduino\arduino_debug.exe" --verify -v embedded.ino