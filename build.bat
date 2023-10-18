@echo on
g++ -c -I dependencies/include -I dependencies/Bezier Main.cpp
g++ Main.o dependencies/Bezier/PTTransform.o -o program.exe -L dependencies/lib -lmingw32 -lSDL2main -lSDL2