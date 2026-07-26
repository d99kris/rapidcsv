// test111.cpp - optional max row and column count limit

#define RAPIDCSV_MAX_COUNT 4

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  try
  {
    // document at max row and column count is accepted
    {
      std::string csv =
        "A,B,C,D\n"
        "1,2,3,4\n"
        "5,6,7,8\n"
        "9,8,7,6\n"
      ;
      std::istringstream sstream(csv);
      rapidcsv::Document doc(sstream, rapidcsv::LabelParams(-1, -1));

      unittest::ExpectEqual(size_t, doc.GetColumnCount(), 4);
      unittest::ExpectEqual(size_t, doc.GetRowCount(), 4);
    }

    // document exceeding max column count is rejected
    {
      std::string csv =
        "A,B,C,D,E\n"
        "1,2,3,4,5\n"
      ;
      std::istringstream sstream(csv);
      ExpectExceptionMsg(rapidcsv::Document doc(sstream, rapidcsv::LabelParams(-1, -1)),
                         std::out_of_range, "max count exceeded: 4");
    }

    // document exceeding max column count on its last line is rejected
    {
      std::string csv =
        "A,B,C,D\n"
        "1,2,3,4,5"
      ;
      std::istringstream sstream(csv);
      ExpectException(rapidcsv::Document doc(sstream, rapidcsv::LabelParams(-1, -1)),
                      std::out_of_range);
    }

    // document exceeding max row count is rejected
    {
      std::string csv =
        "A,B\n"
        "1,2\n"
        "3,4\n"
        "5,6\n"
        "7,8\n"
      ;
      std::istringstream sstream(csv);
      ExpectExceptionMsg(rapidcsv::Document doc(sstream, rapidcsv::LabelParams(-1, -1)),
                         std::out_of_range, "max count exceeded: 4");
    }

    // document exceeding max row count on its last line is rejected
    {
      std::string csv =
        "A,B\n"
        "1,2\n"
        "3,4\n"
        "5,6\n"
        "7,8"
      ;
      std::istringstream sstream(csv);
      ExpectException(rapidcsv::Document doc(sstream, rapidcsv::LabelParams(-1, -1)),
                      std::out_of_range);
    }

    // skipped lines are not counted towards max row count
    {
      std::string csv =
        "#comment\n"
        "\n"
        "A,B\n"
        "1,2\n"
        "3,4\n"
        "5,6\n"
      ;
      std::istringstream sstream(csv);
      rapidcsv::Document doc(sstream, rapidcsv::LabelParams(-1, -1),
                             rapidcsv::SeparatorParams(), rapidcsv::ConverterParams(),
                             rapidcsv::LineReaderParams(true /* pSkipCommentLines */,
                                                        '#' /* pCommentPrefix */,
                                                        true /* pSkipEmptyLines */));

      unittest::ExpectEqual(size_t, doc.GetRowCount(), 4);
    }

    // separators-only data does not cause memory amplification, ref issue #218
    {
      std::string csv(1000000, ',');
      std::istringstream sstream(csv);
      ExpectException(rapidcsv::Document doc(sstream, rapidcsv::LabelParams(-1, -1)),
                      std::out_of_range);
    }

    // linebreaks-only data does not cause memory amplification, ref issue #218
    {
      std::string csv(1000000, '\n');
      std::istringstream sstream(csv);
      ExpectException(rapidcsv::Document doc(sstream, rapidcsv::LabelParams(-1, -1)),
                      std::out_of_range);
    }
  }
  catch (const std::exception& ex)
  {
    std::cout << "exception: " << ex.what() << std::endl;
    rv = 1;
  }

  return rv;
}
