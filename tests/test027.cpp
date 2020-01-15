// test027.cpp - read row values, no row/column labels

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csv =
    "3,9,81\n"
    "4,16,256\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(-1, -1));

    std::vector<int> ints;
    std::vector<std::string> strs;

    ints = doc.GetRow<int>(0);
    unittest::ExpectEqual(size_t, ints.size(), 3);
    unittest::ExpectEqual(int, ints.at(0), 3);
    unittest::ExpectEqual(int, ints.at(1), 9);
    unittest::ExpectEqual(int, ints.at(2), 81);

    strs = doc.GetRow<std::string>(1);
    unittest::ExpectEqual(size_t, strs.size(), 3);
    unittest::ExpectEqual(std::string, strs.at(0), "4");
    unittest::ExpectEqual(std::string, strs.at(1), "16");
    unittest::ExpectEqual(std::string, strs.at(2), "256");
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
