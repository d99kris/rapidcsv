// test079.cpp - perform multiple Load calls on single Document instance

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csv1 =
    "A,B,C,D\n"
    "1,3,9,81\n"
    "2,4,16,256\n"
  ;

  std::string csv2 =
    "A,B,C,D\n"
    "1,3,9,81\n"
  ;

  std::string csv3 =
    "A,B,C,D\n"
    "1,3,9,81\n"
    "2,4,16,256\n"
    "4,16,256,67840\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv3);

  try
  {
    std::istringstream sstream1(csv1);
    rapidcsv::Document doc(sstream1);
    unittest::ExpectEqual(size_t, doc.GetRowCount(), 2);

    std::istringstream sstream2(csv2);
    doc.Load(sstream2);
    unittest::ExpectEqual(size_t, doc.GetRowCount(), 1);

    doc.Load(path);
    unittest::ExpectEqual(size_t, doc.GetRowCount(), 3);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
