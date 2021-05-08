// test084.cpp - comment lines

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csvCommentLines =
    "/ comment line 1\n"
    "/ comment line 2\n"
    "-,A,B,C\n"
    "1,3,9,81\n"
    "2,4,16,256\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csvCommentLines);
  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(), rapidcsv::SeparatorParams(),
                           rapidcsv::ConverterParams(),
                           rapidcsv::LineReaderParams(true, '/'));
    unittest::ExpectEqual(size_t, doc.GetColumn<int>("A").size(), 2);
    unittest::ExpectEqual(size_t, doc.GetColumn<int>("B").size(), 2);
    unittest::ExpectEqual(size_t, doc.GetColumn<int>("C").size(), 2);

    unittest::ExpectEqual(int, doc.GetCell<int>(0, 0), 1);
    unittest::ExpectEqual(int, doc.GetCell<int>(1, 1), 4);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
