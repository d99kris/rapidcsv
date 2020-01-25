// test033.cpp - generate new document by cell, no row/column labels

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csvref =
    "3,9,81\n"
    "4,16,256\n"
  ;

  std::string path = unittest::TempPath();

  try
  {
    rapidcsv::Document doc("", rapidcsv::LabelParams(-1, -1), rapidcsv::SeparatorParams(',', false, false));

    doc.SetCell<int>(0, 0, 3);
    doc.SetCell<int>(1, 0, 9);
    doc.SetCell<int>(2, 0, 81);

    doc.SetCell<int>(0, 1, 4);
    doc.SetCell<int>(1, 1, 16);
    doc.SetCell<int>(2, 1, 256);

    doc.Save(path);

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
