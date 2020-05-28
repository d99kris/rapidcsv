// test053.cpp - write numbers with scientific notation

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csvref =
    "-,A,B,C\n"
    "1,1.2e10,2.00E-07,1e100\n"
  ;

  std::string csv =
    "-,A,B,C\n"
    "1,0,0,0\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc1(path, rapidcsv::LabelParams(0, 0));

    doc1.SetCell<double>("A", "1", 1.2e10);
    doc1.SetCell<double>("B", "1", 2.00E-07);
    doc1.SetCell<double>("C", "1", 1e100);

    doc1.Save();

    rapidcsv::Document doc2(path, rapidcsv::LabelParams(0, 0));

    double eps1 = 1.0e7;
    double exp1 = 1.2e10;
    double val1 = doc2.GetCell<double>("A", "1");
    unittest::ExpectTrue(fabs(val1 - exp1) < eps1);

    double eps2 = 1.0e-10;
    double exp2 = 2.00e-07;
    double val2 = doc2.GetCell<double>("B", "1");
    unittest::ExpectTrue(fabs(val2 - exp2) < eps2);

    double eps3 = 1e97;
    double exp3 = 1e100;
    double val3 = doc2.GetCell<double>("C", "1");
    unittest::ExpectTrue(fabs(val3 - exp3) < eps3);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
