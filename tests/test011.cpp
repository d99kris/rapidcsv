// test011.cpp - generate new document by cell

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csvref =
    ",A,B,C\n"
    "1,3,9,81\n"
    "2,4,16,256\n"
  ;

  std::string path = unittest::TempPath();

  try
  {
    rapidcsv::Document doc("", rapidcsv::LabelParams(0, 0), rapidcsv::SeparatorParams(',', false, false));

    doc.SetCell<int>(0, 0, 3);
    doc.SetCell<int>(1, 0, 9);
    doc.SetCell<int>(2, 0, 81);

    doc.SetCell<int>(0, 1, 4);
    doc.SetCell<int>(1, 1, 16);
    doc.SetCell<int>(2, 1, 256);

    doc.SetColumnName(0, "A");
    doc.SetColumnName(1, "B");
    doc.SetColumnName(2, "C");

    doc.SetRowName(0, "1");
    doc.SetRowName(1, "2");

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
