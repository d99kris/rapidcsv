// test032.cpp - generate new document by column, no row/column labels

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

    doc.SetColumn<int>(0, std::vector<int>({ 2, 3 }));
    doc.SetColumn<int>(1, std::vector<int>({ 4, 9, 16, 25 }));
    doc.SetColumn<int>(2, std::vector<int>({ 16, 81, 256, 625 }));
    doc.SetColumn<int>(3, std::vector<int>({ 256, 6561, 65536, 390625 }));

    doc.SetCell<int>(0, 2, 4);
    doc.SetCell<int>(0, 3, 5);

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
