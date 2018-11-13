@echo off
cc65 breakanoid.c -o temp\out.s -Or
if errorlevel 0 goto ca
echo COMPILER ERROR
goto end
:ca
ca65 temp\out.s -o temp\out.o -t atmos
if errorlevel 0 goto ld
echo ASSEMBLER ERROR
goto end
:ld
ld65  -otemp\a.out lib\atmos.o temp\out.o lib\extra.o lib\atmos.lib -t atmos
if errorlevel 0 goto ne
echo LINKER ERROR
goto end
:ne
del temp\out.s
del temp\out.o
header -a1 -h1 temp\a.out breakanoid.tap $600
pause
D:\Oric\Euphoric\euphoric.exe -a D:\Oric\cc65win\breakanoid.tap
:end
