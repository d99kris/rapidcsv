// test018.cpp - delete columns, no row labels

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csvref =
    "B,D\n"
    "4,256\n"
    "9,6561\n"
    "16,65536\n"
    "25,390625\n"
  ;

  std::string csv =
    "A,B,C,D\n"
    "2,4,16,256\n"
    "3,9,81,6561\n"
    "4,16,256,65536\n"
    "5,25,625,390625\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path);

    doc.RemoveColumn("C");
    doc.RemoveColumn(0);

    doc.Save();

    std::string csvread = unittest::ReadFile(path);

    unittest::ExpectEqual(std::string, csvref, csvread);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
