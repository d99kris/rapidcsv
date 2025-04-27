// test098.cpp - empty cells

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  try
  {
    // empty cells without trailing linebreak on last line
    {
      std::string csv =
        ",\n"
        ",\n"
        ","
      ;

      std::istringstream sstream(csv);
      rapidcsv::Document doc(sstream, rapidcsv::LabelParams(-1, -1));
      unittest::ExpectEqual(size_t, doc.GetRowCount(), 3);
      unittest::ExpectEqual(size_t, doc.GetRow<std::string>(0).size(), 2);
      unittest::ExpectEqual(size_t, doc.GetRow<std::string>(1).size(), 2);
      unittest::ExpectEqual(size_t, doc.GetRow<std::string>(2).size(), 2);
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>(1 /* col */, 0 /* row */), "");
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>(1 /* col */, 1 /* row */), "");
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>(1 /* col */, 2 /* row */), "");
    }

    // empty cells with trailing linebreak on last line
    {
      std::string csv =
        ",\n"
        ",\n"
        ",\n"
      ;

      std::istringstream sstream(csv);
      rapidcsv::Document doc(sstream, rapidcsv::LabelParams(-1, -1));
      unittest::ExpectEqual(size_t, doc.GetRowCount(), 3);
      unittest::ExpectEqual(size_t, doc.GetRow<std::string>(0).size(), 2);
      unittest::ExpectEqual(size_t, doc.GetRow<std::string>(1).size(), 2);
      unittest::ExpectEqual(size_t, doc.GetRow<std::string>(2).size(), 2);
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0 /* col */, 0 /* row */), "");
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0 /* col */, 1 /* row */), "");
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0 /* col */, 2 /* row */), "");
    }
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  return rv;
}
