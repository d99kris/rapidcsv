// test065.cpp - Row count on empty file with header

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csv = "";

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    // empty doc, column header index 0
    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));
    unittest::ExpectEqual(size_t, doc.GetRowCount(), 0);
    unittest::ExpectEqual(size_t, doc.GetColumnCount(), 0);

    // doc with three rows, column header index 10
    std::string csv2 =
      "-,A,B,C\n"
      "1,3,9,81\n"
      "2,4,16,256\n"
    ;
    std::istringstream sstream(csv2);
    rapidcsv::Document doc2(sstream, rapidcsv::LabelParams(10, 10));
    unittest::ExpectEqual(size_t, doc2.GetRowCount(), 0);
    unittest::ExpectEqual(size_t, doc2.GetColumnCount(), 0);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
