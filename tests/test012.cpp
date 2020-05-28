// test012.cpp - read cell value, no row labels

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csv =
    "A,B,C\n"
    "3,9,81\n"
    "4,16,256\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path);
    unittest::ExpectEqual(int, doc.GetCell<int>(0, 0), 3);
    unittest::ExpectEqual(int, doc.GetCell<int>(1, 0), 9);
    unittest::ExpectEqual(int, doc.GetCell<int>(2, 0), 81);

    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 1), "4");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(1, 1), "16");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(2, 1), "256");
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
