// test116.cpp - quoted cell starting with the comment prefix is data, not a comment line

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  try
  {
    // Rows whose first cell is quoted are kept, unquoted comment lines are still skipped.
    {
      std::istringstream input(
        "A,B\n"
        "# comment\n"
        "\"#1 item\",2\n"
        "\"#2 item\"\n"
        "plain,3\n"
        "#trailing,comment\n"
        "\"#last\",4");
      rapidcsv::Document doc(input, rapidcsv::LabelParams(0, -1),
                             rapidcsv::SeparatorParams(),
                             rapidcsv::ConverterParams(),
                             rapidcsv::LineReaderParams(true /* pSkipCommentLines */));

      unittest::ExpectEqual(size_t, doc.GetRowCount(), 4);
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 0), "#1 item");
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>(1, 0), "2");
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 1), "#2 item");
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 2), "plain");
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 3), "#last");
    }

    // With trimming, leading whitespace before the comment prefix is still a comment line,
    // while whitespace before a quoted cell is not.
    {
      std::istringstream input(
        "A,B\n"
        "  # comment\n"
        "  \"#1 item\" ,2\n"
        "x,y\n");
      rapidcsv::Document doc(input, rapidcsv::LabelParams(0, -1),
                             rapidcsv::SeparatorParams(',', true /* pTrim */),
                             rapidcsv::ConverterParams(),
                             rapidcsv::LineReaderParams(true /* pSkipCommentLines */));

      unittest::ExpectEqual(size_t, doc.GetRowCount(), 2);
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 0), "#1 item");
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 1), "x");
    }
  }
  catch (const std::exception& ex)
  {
    std::cout << "exception: " << ex.what() << std::endl;
    rv = 1;
  }

  return rv;
}
