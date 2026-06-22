@echo off

if not exist build mkdir build
cd build
cmake .. -DPURRAY_BUILD_EXAMPLES=ON
cmake --build . --config Release
