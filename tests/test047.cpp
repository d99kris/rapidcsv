// test047.cpp - exception on invalid conversion

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csv =
    "-,A,B,C\n"
    "1,,x,#\n"
    "2,,y,$\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));

    ExpectException(doc.GetCell<int>(0, 0), std::invalid_argument);
    ExpectException(doc.GetCell<int>(1, 0), std::invalid_argument);
    ExpectException(doc.GetCell<int>(2, 0), std::invalid_argument);

    ExpectException(doc.GetCell<double>(0, 1), std::invalid_argument);
    ExpectException(doc.GetCell<double>(1, 1), std::invalid_argument);
    ExpectException(doc.GetCell<double>(2, 1), std::invalid_argument);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
