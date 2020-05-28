// test034.cpp - copy document

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csv =
    "-,A,B,C\n"
    "1,3,9,81\n"
    "2,4,16,256\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  std::string pathcopy = unittest::TempPath();

  try
  {
    rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));

    rapidcsv::Document doccopy(doc);

    doccopy.Save(pathcopy);

    std::string csvread = unittest::ReadFile(pathcopy);

    unittest::ExpectEqual(std::string, csv, csvread);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);
  unittest::DeleteFile(pathcopy);

  return rv;
}
