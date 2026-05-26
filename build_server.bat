@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" x86
msbuild "E:\23\Source Server\GameServer\GameServer.sln" /p:Configuration=Release_EX502 /p:Platform=Win32 /v:minimal
