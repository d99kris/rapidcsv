// test005.cpp - read row values

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csv =
    "-,A,B,C\n"
    "1,3,9,81\n"
    "2,4,16,256\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));

    std::vector<int> ints;
    std::vector<std::string> strs;

    ints = doc.GetRow<int>(0);
    unittest::ExpectEqual(size_t, ints.size(), 3);
    unittest::ExpectEqual(int, ints.at(0), 3);
    unittest::ExpectEqual(int, ints.at(1), 9);
    unittest::ExpectEqual(int, ints.at(2), 81);

    strs = doc.GetRow<std::string>("2");
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
