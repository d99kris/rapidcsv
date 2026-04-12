// test104.cpp - narrow integer type range checking

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csv =
    "col\n"
    "127\n"
    "128\n"
    "-128\n"
    "-129\n"
    "255\n"
    "256\n"
    "32767\n"
    "32768\n"
    "65535\n"
    "65536\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path);

    // signed char: valid range [-128, 127]
    unittest::ExpectEqual(int, static_cast<int>(doc.GetCell<signed char>(0, 0)), 127);
    ExpectException(doc.GetCell<signed char>(0, 1), std::out_of_range);
    unittest::ExpectEqual(int, static_cast<int>(doc.GetCell<signed char>(0, 2)), -128);
    ExpectException(doc.GetCell<signed char>(0, 3), std::out_of_range);

    // unsigned char: valid range [0, 255]
    unittest::ExpectEqual(int, static_cast<int>(doc.GetCell<unsigned char>(0, 4)), 255);
    ExpectException(doc.GetCell<unsigned char>(0, 5), std::out_of_range);

    // short: valid range [-32768, 32767]
    unittest::ExpectEqual(int, static_cast<int>(doc.GetCell<short>(0, 6)), 32767);
    ExpectException(doc.GetCell<short>(0, 7), std::out_of_range);

    // unsigned short: valid range [0, 65535]
    unittest::ExpectEqual(int, static_cast<int>(doc.GetCell<unsigned short>(0, 8)), 65535);
    ExpectException(doc.GetCell<unsigned short>(0, 9), std::out_of_range);
  }
  catch (const std::exception& ex)
  {
    std::cout << "exception: " << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
