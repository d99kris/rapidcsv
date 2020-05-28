// test058.cpp - read UTF-16 BE

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  const unsigned char u16be[] =
  {
    0xfe, 0xff,
    0x00, 0x2d, 0x00, 0x2c, 0x00, 0x41, 0x00, 0x2c, 0x00, 0x42, 0x00, 0x2c,
    0x00, 0x43, 0x00, 0x0a, 0x00, 0x31, 0x00, 0x2c, 0x00, 0x33, 0x00, 0x2c,
    0x00, 0x39, 0x00, 0x2c, 0x00, 0x38, 0x00, 0x31, 0x00, 0x0a, 0x00, 0x32,
    0x00, 0x2c, 0x00, 0x34, 0x00, 0x2c, 0x00, 0x31, 0x00, 0x36, 0x00, 0x2c,
    0x00, 0x32, 0x00, 0x35, 0x00, 0x36, 0x00, 0x0a
  };
  const unsigned int u16be_len = 58;

  std::string csv(reinterpret_cast<const char *>(u16be), u16be_len);
  // "-,A,B,C\n"
  // "1,3,9,81\n"
  // "2,4,16,256\n"

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));
    unittest::ExpectEqual(int, doc.GetCell<int>(0, 0), 3);
    unittest::ExpectEqual(int, doc.GetCell<int>(1, 0), 9);
    unittest::ExpectEqual(int, doc.GetCell<int>(2, 0), 81);

    unittest::ExpectEqual(std::string, doc.GetCell<std::string>("A", "2"), "4");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>("B", "2"), "16");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>("C", "2"), "256");
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
