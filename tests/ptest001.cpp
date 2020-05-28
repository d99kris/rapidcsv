// ptest001.cpp - file load and get data by column, row and cell

#include <rapidcsv.h>
#include "perftest.h"
#include "unittest.h"

int main()
{
  int rv = 0;

  try
  {
    perftest::Timer timer;

    for (int i = 0; i < 10; ++i)
    {
      timer.Start();

      rapidcsv::Document doc("../tests/msft.csv", rapidcsv::LabelParams(0, 0));
      const std::vector<double>& column = doc.GetColumn<double>("Close");
      const std::vector<double>& row = doc.GetRow<double>("2016-05-23");
      const double cell = doc.GetCell<double>("Close", "2016-05-23");

      timer.Stop();

      // dummy usage of variables
      (void) column;
      (void) row;
      (void) cell;
    }

    timer.ReportMedian();
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  return rv;
}
