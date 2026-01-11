// test103.cpp - set supported datatypes

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csvref =
    "int,32767\n"
    "long,2147483647\n"
    "long long,9223372036854775807\n"
    "unsigned,65535\n"
    "unsigned long,4294967295\n"
    "unsigned long long,18446744073709551615\n"
    "float,3.40282347e+38\n"
    "double,1.6e+308\n"
    "long double,1.6e+308\n"
    "signed char,-128\n"
    "unsigned char,255\n"
    "short,32767\n"
    "unsigned short,65535\n"
  ;

  std::string csv =
    "int,0\n"
    "long,0\n"
    "long long,0\n"
    "unsigned,0\n"
    "unsigned long,0\n"
    "unsigned long long,0\n"
    "float,0\n"
    "double,0\n"
    "long double,0\n"
    "signed char,0\n"
    "unsigned char,0\n"
    "short,0\n"
    "unsigned short,0\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::LabelParams labelParams(-1, -1);
    rapidcsv::SeparatorParams separatorParams(',', false, rapidcsv::sPlatformHasCR, false, false /*pAutoQuote*/);
    rapidcsv::Document doc(path, labelParams, separatorParams);

    // set cells
    doc.SetCell<int>(1, 0, 32767);
    doc.SetCell<long>(1, 1, 2147483647);
    doc.SetCell<long long>(1, 2, 9223372036854775807);

    doc.SetCell<unsigned>(1, 3, 65535);
    doc.SetCell<unsigned long>(1, 4, 4294967295);
    doc.SetCell<unsigned long long>(1, 5, 18446744073709551615llu);

    doc.SetCell<float>(1, 6, 3.40282347e+38);
    doc.SetCell<double>(1, 7, 1.6E308);
    doc.SetCell<long double>(1, 8, 1.6E308);

    doc.SetCell<signed char>(1, 9, -128);
    doc.SetCell<unsigned char>(1, 10, 255);
    doc.SetCell<short>(1, 11, 32767);
    doc.SetCell<unsigned short>(1, 12, 65535);

    // read back
    unittest::ExpectEqual(int, doc.GetCell<int>(1, 0), 32767);
    unittest::ExpectEqual(long, doc.GetCell<long>(1, 1), 2147483647);
    unittest::ExpectEqual(long long, doc.GetCell<long long>(1, 2), 9223372036854775807);

    unittest::ExpectEqual(unsigned, doc.GetCell<unsigned>(1, 3), 65535);
    unittest::ExpectEqual(unsigned long, doc.GetCell<unsigned long>(1, 4), 4294967295);
    unittest::ExpectEqual(unsigned long long, doc.GetCell<unsigned long long>(1, 5), 18446744073709551615llu);

    float floatval = doc.GetCell<float>(1, 6);
    unittest::ExpectTrue((floatval > 3.40E38) && (floatval < 3.45E38));

    double doubleval = doc.GetCell<double>(1, 7);
    unittest::ExpectTrue((doubleval > 1.5E308) && (doubleval < 1.7E308));

    long double longdoubleval = doc.GetCell<long double>(1, 8);
    unittest::ExpectTrue((longdoubleval > 1.5E308) && (longdoubleval < 1.7E308));

    unittest::ExpectEqual(signed char, doc.GetCell<signed char>(1, 9), -128);
    unittest::ExpectEqual(unsigned char, doc.GetCell<unsigned char>(1, 10), 255);
    unittest::ExpectEqual(short, doc.GetCell<short>(1, 11), 32767);
    unittest::ExpectEqual(unsigned short, doc.GetCell<unsigned short>(1, 12), 65535);

    // check full doc
    doc.Save();
    std::string csvread = unittest::ReadFile(path);
    unittest::ExpectEqual(std::string, csvref, csvread);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
