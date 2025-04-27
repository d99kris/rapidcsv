// test099.cpp - exception message removing out-of-range column

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csv =
    "-,A,B,C,D\n"
    "1,3,9,81,6561\n"
    "2,4,16,256\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));

    doc.RemoveColumn("A");
    unittest::ExpectEqual(std::string, doc.GetColumnName(0), "B");

    doc.RemoveColumn(0);
    unittest::ExpectEqual(std::string, doc.GetColumnName(0), "C");

    // doc has columns C (0) and D (1) now, thus 2 is out of range.
    ExpectExceptionMsg(doc.RemoveColumn(2), std::out_of_range,
                       "column out of range: 2 (on row 0)");

    // column D, while existing, has not data on all rows, thus out of range.
    ExpectExceptionMsg(doc.RemoveColumn("D"), std::out_of_range,
                       "column out of range: 1 (on row 2)");
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
