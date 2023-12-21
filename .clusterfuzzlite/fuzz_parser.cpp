#include <fstream>
#include <rapidcsv.h>
#include <string>

using namespace std;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  std::string fuzz_payload(reinterpret_cast<const char *>(data), size);

  ofstream myfile;
  myfile.open("/tmp/fuzz.csv");
  myfile << fuzz_payload;
  myfile.close();

  rapidcsv::Document doc("/tmp/fuzz.csv");

  return 0;
}