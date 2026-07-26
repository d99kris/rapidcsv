// test108.cpp - row label lookup in document with short rows

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  // second data row is too short to hold a row label
  std::string csv =
    "-,rowname,A,B\n"
    "x\n"
    ",r2,21,22\n"
    ",r3,31,32\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 1));

    unittest::ExpectEqual(size_t, doc.GetRowCount(), 3);

    // the short row is unnamed, but shall not offset the rows following it
    unittest::ExpectEqual(int, doc.GetRowIdx("r2"), 1);
    unittest::ExpectEqual(int, doc.GetRowIdx("r3"), 2);

    unittest::ExpectEqual(std::string, doc.GetCell<std::string>("A", "r2"), "21");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>("B", "r2"), "22");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>("A", "r3"), "31");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>("B", "r3"), "32");

    // lookup by index shall give the same result as lookup by name
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(size_t(0), size_t(2)), "31");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(size_t(1), size_t(2)), "32");
  }
  catch (const std::exception& ex)
  {
    std::cout << "exception: " << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
