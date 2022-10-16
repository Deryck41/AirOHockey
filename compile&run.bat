@echo off
gcc main.c -o aerohockey.exe -lwinmm -lglu32 -lopengl32 -lgdi32 -lws2_32
start aerohockey.exe
exit