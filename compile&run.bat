@echo off
gcc main.c physics.c drawing.c -o aerohockey.exe -static-libstdc++ -static-libgcc -static -lwinmm -lglu32 -lopengl32 -lgdi32 -lws2_32
pause
start aerohockey.exe
exit