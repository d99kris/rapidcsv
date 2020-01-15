// ptest002.cpp - file load

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

      rapidcsv::Document doc("../tests/msft.csv");

      timer.Stop();
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
