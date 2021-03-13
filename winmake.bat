@echo off

mkdir build-debug
cd build-debug || goto :error
cmake -DRAPIDCSV_BUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug .. || goto :error
msbuild /m /nologo /verbosity:quiet /property:Configuration=Debug rapidcsv.sln || goto :error
ctest -C Debug --output-on-failure || goto :error
cd .. || goto :error

mkdir build-release
cd build-release || goto :error
cmake -DRAPIDCSV_BUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release .. || goto :error
msbuild /m /nologo /verbosity:quiet /property:Configuration=Release rapidcsv.sln || goto :error
ctest -C Release --verbose || goto :error
cd .. || goto :error

goto :EOF

:error
echo Failed with error #%errorlevel%.
exit /b %errorlevel%
