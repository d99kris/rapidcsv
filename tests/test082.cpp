// test081.cpp - generate new document with insert row

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csvref =
    ",A,B,C,D\n"
    "0,2,4,16,256\n"
    "1,3,9,81,6561\n"
    "2,4,16,256,65536\n"
    "3,5,25,625,390625\n"
  ;

  std::string path = unittest::TempPath();

  try
  {
    rapidcsv::Document doc("", rapidcsv::LabelParams(0, 0), rapidcsv::SeparatorParams(',', false, false));

    doc.InsertRow(0, std::vector<int>({ 3, 9, 81, 6561 }), "1");
    doc.InsertRow(0, std::vector<int>({ 2, 4, 16, 256 }), "0");

    doc.InsertRow<int>(2);
    doc.SetRow(2, std::vector<int>({ 4, 16, 256, 65536 }));
    doc.SetRowName(2, "2");

    doc.InsertRow(3, std::vector<int>({ 5, 25, 625, 390625 }), "3");

    doc.SetColumnName(0, "A");
    doc.SetColumnName(1, "B");
    doc.SetColumnName(2, "C");
    doc.SetColumnName(3, "D");

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
