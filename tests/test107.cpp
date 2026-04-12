// test107.cpp - get column labels of empty label row

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  try
  {
    // Empty document, specify column and row labels available
    std::string csv = "";
    std::istringstream s(csv);
    rapidcsv::Document doc(s, rapidcsv::LabelParams(0, 0));

    // InsertRow create a row with 0 columns
    doc.InsertRow<std::string>(0);

    // Get column names
    std::vector<std::string> names = doc.GetColumnNames();
    unittest::ExpectEqual(size_t, names.size(), size_t(0));
  }
  catch (const std::exception& ex)
  {
    std::cout << "exception: " << ex.what() << std::endl;
    rv = 1;
  }

  return rv;
}
