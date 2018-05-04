// test050.cpp - read column header / row label by index

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
    rapidcsv::Document doc(path);

    unittest::ExpectEqual(std::string, doc.GetColumnLabel(0), "-");
    unittest::ExpectEqual(std::string, doc.GetColumnLabel(1), "A");
  }
  catch(const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}

