// test097.cpp - trailing comment line without linebreak

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csv =
    "A,B,C\n"
    "1,3,9\n"
    "2,4,16\n"
    "# comment line 1\n"
    "# comment line 2"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);
  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(),
                           rapidcsv::SeparatorParams(),
                           rapidcsv::ConverterParams(),
                           rapidcsv::LineReaderParams(true /* pSkipCommentLines */));

    unittest::ExpectEqual(size_t, doc.GetRowCount(), 2);
    unittest::ExpectEqual(size_t, doc.GetColumn<std::string>("A").size(), 2);
    unittest::ExpectEqual(int, doc.GetCell<int>(0, 0), 1);
    unittest::ExpectEqual(int, doc.GetCell<int>(0, 1), 2);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
