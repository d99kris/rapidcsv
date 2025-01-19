// test042.cpp - test GetColumnCount() and GetRowCount()

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csv1 =
    "-,A,B,C\n"
    "1,3,9,81\n"
    "2,4,16,256\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv1);

  try
  {
    rapidcsv::Document doc1(path, rapidcsv::LabelParams(0, 0));
    unittest::ExpectEqual(size_t, doc1.GetColumnCount(), 3);
    unittest::ExpectEqual(size_t, doc1.GetRowCount(), 2);

    rapidcsv::Document doc2(path, rapidcsv::LabelParams(-1, -1));
    unittest::ExpectEqual(size_t, doc2.GetColumnCount(), 4);
    unittest::ExpectEqual(size_t, doc2.GetRowCount(), 3);

    // test column count when column labels after first row
    std::string csv2 =
      "Sep=,\n"
      "-,A,B,C\n"
      "1,3,9,81\n"
      "2,4,16,256\n"
    ;
    std::istringstream sstream2(csv2);
    rapidcsv::Document doc3(sstream2, rapidcsv::LabelParams(1, -1));
    unittest::ExpectEqual(size_t, doc3.GetColumnCount(), 4);
    unittest::ExpectEqual(size_t, doc3.GetRowCount(), 2);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
