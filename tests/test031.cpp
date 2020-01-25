// test031.cpp - generate new document by row, no row/column labels

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csvref =
    "2,4,16,256\n"
    "3,9,81,6561\n"
    "4,16,256,65536\n"
    "5,25,625,390625\n"
  ;

  std::string path = unittest::TempPath();

  try
  {
    rapidcsv::Document doc("", rapidcsv::LabelParams(-1, -1), rapidcsv::SeparatorParams(',', false, false));

    doc.SetRow<int>(0, std::vector<int>({ 2, 4 }));
    doc.SetRow<int>(1, std::vector<int>({ 3, 9, 81, 6561 }));
    doc.SetRow<int>(2, std::vector<int>({ 4, 16, 256, 65536 }));
    doc.SetRow<int>(3, std::vector<int>({ 5, 25, 625, 390625 }));

    doc.SetCell<int>(2, 0, 16);
    doc.SetCell<int>(3, 0, 256);

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
