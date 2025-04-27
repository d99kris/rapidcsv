// test100.cpp - exception message inserting column out-of-range

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csv =
    "A,B,C\n"
    "1,3,9\n"
    "2,4,16\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path);

    // insert at end (append)
    doc.InsertColumn(3, std::vector<int>({ 1, 2 }), "D");

    // insert at end (append)
    doc.InsertColumn(4, std::vector<int>({ 3, 4 }), "E");

    // insert out-of-range
    ExpectExceptionMsg(doc.InsertColumn(6, std::vector<int>({ 5, 6 }), "F"),
                       std::out_of_range, "column out of range: 6 (on row 0)");
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
