// test070.cpp - get row index with column labels

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
    // Row Label
    // ↓
    // -,  A,  B,  C   ← Column Label
    // 1,  3,  9,  81
    // 2,  4,  16, 256
    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));

    unittest::ExpectEqual(ssize_t, doc.GetRowIdx("-"), -1);
    unittest::ExpectEqual(ssize_t, doc.GetRowIdx("1"), 0);
    unittest::ExpectEqual(ssize_t, doc.GetRowIdx("2"), 1);
    unittest::ExpectEqual(ssize_t, doc.GetRowIdx("3"), -1);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
