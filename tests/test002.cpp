// test002.cpp - set cell value

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csvref =
    "-,A,B,C\n"
    "1,3,9,81\n"
    "2,4,16,256\n"
  ;

  std::string csv =
    "-,A,B,C\n"
    "1,0,0,0\n"
    "2,0,0,0\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));

    doc.SetCell<int>(0, 0, 3);
    doc.SetCell<int>(1, 0, 9);
    doc.SetCell<int>(2, 0, 81);

    doc.SetCell<std::string>("A", "2", "4");
    doc.SetCell<std::string>("B", "2", "16");
    doc.SetCell<std::string>("C", "2", "256");

    unittest::ExpectEqual(int, doc.GetCell<int>(0, 0), 3);
    unittest::ExpectEqual(int, doc.GetCell<int>(1, 0), 9);
    unittest::ExpectEqual(int, doc.GetCell<int>(2, 0), 81);

    unittest::ExpectEqual(std::string, doc.GetCell<std::string>("A", "2"), "4");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>("B", "2"), "16");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>("C", "2"), "256");

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
