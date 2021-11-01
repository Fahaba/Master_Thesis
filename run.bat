@echo off	
ECHO 'Initializing startup'

ECHO 'Starting StreamWriter...'
start ./x64/Debug/StreamWriter.exe

ECHO 'Starting Bokeh Server...'
start cmd.exe /c "python3 ./python/visServer.py"

set /p choice="CUDA - Python [1] or C++ [2]? Enter: "
if %choice% EQU 1 (GOTO python)
if %choice% EQU 2 (GOTO cpp)

:python
ECHO 'Starting pyCudaAudioTracer'
start cmd.exe /c "python3 ./python/pyCudaAudioTracer.py"
GOTO end

:cpp
ECHO 'Starting AudioTracer...'
start ./x64/Debug/AudioTracer.exe
GOTO end

:end
ECHO 'DONE.'