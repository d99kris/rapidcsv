# Copy all fuzzer executables to $OUT/
$CXX $CFLAGS $LIB_FUZZING_ENGINE \
  $SRC/rapidcsv/.clusterfuzzlite/fuzz_parser.cpp \
  -o $OUT/fuzz_parser \
  -I$SRC/rapidcsv/src
