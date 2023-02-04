// test091.cpp - writing / reading high-precision floating point numbers

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csv =
    "-,A,B,C\n"
    "f,0,0,0\n"
    "d,0,0,0\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    const float f1 = 3.14159f;
    const float f2 = 3.1415926535f;
    const float f3 = 3.141592653589793f;
    const double d1 = 3.14159;
    const double d2 = 3.141592653589793;
    const double d3 = 3.14159265358979323846;

    {
      rapidcsv::Document doc1(path, rapidcsv::LabelParams(0, 0));

      doc1.SetCell<float>("A", "f", f1);
      doc1.SetCell<float>("B", "f", f2);
      doc1.SetCell<float>("C", "f", f3);

      doc1.SetCell<double>("A", "d", d1);
      doc1.SetCell<double>("B", "d", d2);
      doc1.SetCell<double>("C", "d", d3);

      doc1.Save();
    }

    {
      rapidcsv::Document doc2(path, rapidcsv::LabelParams(0, 0));

      unittest::ExpectEqual(float, doc2.GetCell<float>("A", "f"), f1);
      unittest::ExpectEqual(float, doc2.GetCell<float>("B", "f"), f2);
      unittest::ExpectEqual(float, doc2.GetCell<float>("C", "f"), f3);

      unittest::ExpectEqual(double, doc2.GetCell<double>("A", "d"), d1);
      unittest::ExpectEqual(double, doc2.GetCell<double>("B", "d"), d2);
      unittest::ExpectEqual(double, doc2.GetCell<double>("C", "d"), d3);
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
