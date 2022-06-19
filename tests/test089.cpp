// test089.cpp - get data by label after delete column and row by label

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csvref =
    "-,B,D\n"
    "1,9,6561\n"
    "3,25,390625\n"
  ;

  std::string csv =
    "-,A,B,C,D\n"
    "0,2,4,16,256\n"
    "1,3,9,81,6561\n"
    "2,4,16,256,65536\n"
    "3,5,25,625,390625\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));

    doc.RemoveColumn("A");
    doc.RemoveColumn("C");

    std::vector<int> ints;

    ints = doc.GetColumn<int>("B");
    unittest::ExpectEqual(size_t, ints.size(), 4);
    unittest::ExpectEqual(int, ints.at(0), 4);
    unittest::ExpectEqual(int, ints.at(1), 9);

    doc.RemoveRow("0");
    doc.RemoveRow("2");

    unittest::ExpectEqual(std::string, doc.GetCell<std::string>("B", "1"), "9");

    ints = doc.GetRow<int>("1");
    unittest::ExpectEqual(size_t, ints.size(), 2);
    unittest::ExpectEqual(int, ints.at(0), 9);
    unittest::ExpectEqual(int, ints.at(1), 6561);

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
