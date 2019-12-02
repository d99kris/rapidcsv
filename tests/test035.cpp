// test035.cpp - custom conversion

#include <iomanip>
#include <math.h>
#include <rapidcsv.h>
#include "unittest.h"

// Data requested as ints to be converted to fixed-point two decimal numbers
namespace rapidcsv
{
  template<typename T>
  class CustomConverter : public Converter<T>
  {
  public:
    void ToVal(const std::string& pStr, T& pVal) const;
    void ToStr(const T& pVal, std::string& pStr) const;
  };

  template<>
  void CustomConverter<int>::ToVal(const std::string& pStr, int& pVal) const
  {
    pVal = static_cast<int>(roundf(100.0f * std::stof(pStr)));
  }

  template<>
  void CustomConverter<int>::ToStr(const int& pVal, std::string& pStr) const
  {
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << static_cast<float>(pVal) / 100.0f;
    pStr = out.str();
  }

  template<>
  void CustomConverter<std::string>::ToVal(const std::string& pStr, std::string& pVal) const
  {
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << static_cast<int>(roundf(100.0f * std::stof(pStr)));
    pVal = out.str();
  }

  template<>
  void CustomConverter<std::string>::ToStr(const std::string& pVal, std::string& pStr) const
  {
    std::ostringstream out;
    out << std::fixed << std::setprecision(2) << static_cast<float>(std::stof(pVal)) / 100.0f;
    pStr = out.str();
  }
}

int main()
{
  int rv = 0;

  std::string csv =
    "1,10,100,1000\n"
    "0.1,0.01,0.001,0.006\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::CustomConverter<int> intConverter;
    rapidcsv::CustomConverter<std::string> stringConverter;
    rapidcsv::Document doc(path, rapidcsv::LabelParams(-1, -1));

    unittest::ExpectEqual(int, doc.GetCell<int>(0, 0, intConverter), 100);
    unittest::ExpectEqual(int, doc.GetCell<int>(1, 0, intConverter), 1000);
    unittest::ExpectEqual(int, doc.GetCell<int>(2, 0, intConverter), 10000);
    unittest::ExpectEqual(int, doc.GetCell<int>(3, 0, intConverter), 100000);

    unittest::ExpectEqual(int, doc.GetCell<int>(0, 1, intConverter), 10);
    unittest::ExpectEqual(int, doc.GetCell<int>(1, 1, intConverter), 1);
    unittest::ExpectEqual(int, doc.GetCell<int>(2, 1, intConverter), 0);
    unittest::ExpectEqual(int, doc.GetCell<int>(3, 1, intConverter), 1);

    doc.SetCell<int>(0, 0, 12345, intConverter);

    unittest::ExpectEqual(int, doc.GetCell<int>(0, 0, intConverter), 12345);
    unittest::ExpectEqual(double, doc.GetCell<double>(0, 0), 123.45);
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 0, stringConverter), "12345");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 0), "123.45");


    doc.SetCell<std::string>(0, 0, "12345", stringConverter);

    unittest::ExpectEqual(int, doc.GetCell<int>(0, 0, intConverter), 12345);
    unittest::ExpectEqual(double, doc.GetCell<double>(0, 0), 123.45);
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 0, stringConverter), "12345");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 0), "123.45");
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
