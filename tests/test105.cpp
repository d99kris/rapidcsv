// test105.cpp - SetColumnName / SetRowName stale label cleanup

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csv =
    "name,value\n"
    "row1,100\n"
    "row2,200\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    // Test 1: SetColumnName removes old name from map
    {
      rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));

      unittest::ExpectEqual(int, doc.GetColumnIdx("value"), 0);
      doc.SetColumnName(0, "price");
      unittest::ExpectEqual(int, doc.GetColumnIdx("price"), 0);
      unittest::ExpectEqual(int, doc.GetColumnIdx("value"), -1);
    }

    // Test 2: SetRowName removes old name from map
    {
      rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));

      unittest::ExpectEqual(int, doc.GetRowIdx("row1"), 0);
      doc.SetRowName(0, "item1");
      unittest::ExpectEqual(int, doc.GetRowIdx("item1"), 0);
      unittest::ExpectEqual(int, doc.GetRowIdx("row1"), -1);
    }

    // Test 3: Data access by new name works correctly
    {
      rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));

      doc.SetColumnName(0, "price");
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>("price", "row1"), "100");

      doc.SetRowName(0, "item1");
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>("price", "item1"), "100");
    }
  }
  catch (const std::exception& ex)
  {
    std::cout << "exception: " << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
