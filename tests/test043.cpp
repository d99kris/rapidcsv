// test043.cpp - test GetColumnCount() and GetRowCount() public access

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
    rapidcsv::Document doc1(path);
    unittest::ExpectEqual(int, doc1.GetColumnCount(), 4);
    unittest::ExpectEqual(int, doc1.GetRowCount(), 3);

    rapidcsv::Document doc2(rapidcsv::Properties(path, -1, -1));
    unittest::ExpectEqual(int, doc2.GetColumnCount(), 4);
    unittest::ExpectEqual(int, doc2.GetRowCount(), 3);
  }
  catch(const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
