// test078.cpp - include Windows.h before rapidcsv.h

#if defined(_MSC_VER)
#include <Windows.h>
#endif

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

    // below functions should use all template functions
    unittest::ExpectEqual(int, doc.GetColumn<int>(0).at(0), 3);
    unittest::ExpectEqual(int, doc.GetColumn<int>("A").at(0), 3);

    doc.SetColumn(0, std::vector<int>({ 4, 5 }));
    unittest::ExpectEqual(int, doc.GetColumn<int>(0).at(0), 4);
    doc.SetColumn("A", std::vector<int>({ 5, 6 }));
    unittest::ExpectEqual(int, doc.GetColumn<int>("A").at(0), 5);

    doc.InsertColumn(0, std::vector<int>({ 7, 8 }), "A2");
    unittest::ExpectEqual(int, doc.GetColumn<int>("A2").at(0), 7);

    unittest::ExpectEqual(int, doc.GetRow<int>(0).at(0), 7);
    unittest::ExpectEqual(int, doc.GetRow<int>("1").at(0), 7);

    doc.SetRow(0, std::vector<int>({ 9, 3, 9, 81 }));
    unittest::ExpectEqual(int, doc.GetRow<int>(0).at(0), 9);

    doc.SetRow("1", std::vector<int>({ 9, 3, 9, 81 }));
    unittest::ExpectEqual(int, doc.GetRow<int>("1").at(0), 9);

    doc.InsertRow(0, std::vector<int>({ 1, 2, 3, 4 }), "1B");
    unittest::ExpectEqual(int, doc.GetRow<int>("1B").at(0), 1);

    unittest::ExpectEqual(int, doc.GetCell<int>(0, 0), 1);
    unittest::ExpectEqual(int, doc.GetCell<int>("A2", "1B"), 1);
    unittest::ExpectEqual(int, doc.GetCell<int>(0, "1B"), 1);
    unittest::ExpectEqual(int, doc.GetCell<int>("A2", 0), 1);

    doc.SetCell(1, 1, 111);
    unittest::ExpectEqual(int, doc.GetCell<int>(1, 1), 111);
    doc.SetCell("A", "2", 222);
    unittest::ExpectEqual(int, doc.GetCell<int>("A", "2"), 222);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
