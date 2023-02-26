// test093.cpp - set cell values, no row labels, column labels offset, comments not ignored

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csvref =
    "# Comment line\n"
    "A,B,C\n"
    "3,9,81\n"
    "4,16,256\n"
  ;

  std::string csv =
    "# Comment line\n"
    "A,B,C\n"
    "3,9,81\n"
    "4,16,256\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    {
      rapidcsv::LabelParams labelParams;
      labelParams.mColumnNameIdx = 1; // line index 0 is comment
      labelParams.mRowNameIdx = -1;
      rapidcsv::SeparatorParams separatorParams;
      separatorParams.mAutoQuote = false;
      rapidcsv::Document doc(path, labelParams, separatorParams);

      doc.SetCell<int>(0, 1, 4);

      doc.Save();

      std::string csvread = unittest::ReadFile(path);

      unittest::ExpectEqual(std::string, csvref, csvread);
    }

    {
      rapidcsv::LabelParams labelParams;
      labelParams.mColumnNameIdx = 1; // line index 0 is comment
      labelParams.mRowNameIdx = -1;
      rapidcsv::SeparatorParams separatorParams;
      separatorParams.mAutoQuote = false;
      rapidcsv::Document doc(path, labelParams, separatorParams);

      doc.SetCell<int>(1, 1, 16);

      doc.Save();

      std::string csvread = unittest::ReadFile(path);

      unittest::ExpectEqual(std::string, csvref, csvread);
    }

    {
      rapidcsv::LabelParams labelParams;
      labelParams.mColumnNameIdx = 1; // line index 0 is comment
      labelParams.mRowNameIdx = -1;
      rapidcsv::SeparatorParams separatorParams;
      separatorParams.mAutoQuote = false;
      rapidcsv::Document doc(path, labelParams, separatorParams);

      doc.SetCell<int>(2, 1, 256);

      doc.Save();

      std::string csvread = unittest::ReadFile(path);

      unittest::ExpectEqual(std::string, csvref, csvread);
    }
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
