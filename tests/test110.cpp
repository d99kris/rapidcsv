// test110.cpp - unsigned integer type range checking

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csv =
    "col\n"
    "-1\n"
    "-0\n"
    "4294967295\n"
    "4294967296\n"
    "18446744073709551615\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path);

    // negative values shall not wrap around to large unsigned values
    ExpectException(doc.GetCell<unsigned char>(0, 0), std::out_of_range);
    ExpectException(doc.GetCell<unsigned short>(0, 0), std::out_of_range);
    ExpectException(doc.GetCell<unsigned>(0, 0), std::out_of_range);
    ExpectException(doc.GetCell<unsigned long>(0, 0), std::out_of_range);
    ExpectException(doc.GetCell<unsigned long long>(0, 0), std::out_of_range);

    // negative zero is zero
    unittest::ExpectEqual(unsigned, doc.GetCell<unsigned>(0, 1), 0u);
    unittest::ExpectEqual(unsigned long long, doc.GetCell<unsigned long long>(0, 1), 0llu);

    // unsigned: valid range [0, 4294967295]
    unittest::ExpectEqual(unsigned, doc.GetCell<unsigned>(0, 2), 4294967295u);
    ExpectException(doc.GetCell<unsigned>(0, 3), std::out_of_range);

    // unsigned long long: valid range [0, 18446744073709551615]
    unittest::ExpectEqual(unsigned long long, doc.GetCell<unsigned long long>(0, 4),
                          18446744073709551615llu);
  }
  catch (const std::exception& ex)
  {
    std::cout << "exception: " << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
