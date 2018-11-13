@echo off
IF "%1"=="" GOTO Error1






cc65 %1 -o temp\out.s -Or
ca65 temp\out.s -o temp\out.o -t atmos
ld65  -otemp\a.out lib\atmos.o temp\out.o lib\extra.o lib\atmos.lib -t atmos

del temp\out.s
del temp\out.o
header -a1 -h1 temp\a.out breakanoid.tap $600


goto End



:Error1
echo No files on command line

:End
