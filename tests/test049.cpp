// test049.cpp - default conversion to custom values

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
    rapidcsv::Document doc(path, rapidcsv::LabelParams(), rapidcsv::SeparatorParams(),
                           true);

    rapidcsv::Converter<int> iConverter(true, 1);
    rapidcsv::Converter<long long> llConverter(true, 1);
    rapidcsv::Converter<unsigned int> uiConverter(true, 1);

    unittest::ExpectEqual(int, doc.GetCell<int>(0, 0, iConverter), 1);
    unittest::ExpectEqual(long long, doc.GetCell<long long>(1, 0, llConverter), 1);
    unittest::ExpectEqual(unsigned int, doc.GetCell<unsigned int>(2, 0, uiConverter), 1);

    rapidcsv::Converter<double> dConverter(true, 0.0);
    rapidcsv::Converter<long double> ldConverter(true, 0.0);
    rapidcsv::Converter<float> fConverter(true, 0.0);

    unittest::ExpectEqual(double, doc.GetCell<double>(0, 1, dConverter), 0.0);
    unittest::ExpectEqual(long double, doc.GetCell<long double>(1, 1, ldConverter), 0.0);
    unittest::ExpectEqual(float, doc.GetCell<float>(2, 1, fConverter), 0.0);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
