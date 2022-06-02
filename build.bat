mkdir build
cd build
cmake -G"MinGW Makefiles" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
mingw32-make -j8
cd..
build\src\antSim.exe
pause