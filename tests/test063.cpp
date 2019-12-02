// test063.cpp - custom conversions

#include <rapidcsv.h>
#include "unittest.h"

class StrLenConverter : public rapidcsv::Converter<size_t>
{
public:
  void ToVal(const std::string& str, size_t& val) const
  {
    val = str.length();
  }
};

class ModConverter : public rapidcsv::Converter<int>
{
public:
  void ToVal(const std::string& str, int& val) const
  {
    val = 0;
    for (size_t i = 0; i < str.length(); ++i)
    {
      val += str.at(i);
      val %= 0xFF;
    }
  }
};

class SexConverter : public rapidcsv::Converter<int>
{
public:
  void ToStr(int val, std::string& str) const
  {
    if (val == 1)
    {
      str = "male";
    }
    else if (val == 2)
    {
      str = "female";
    }
    else
    {
      str = "";
    }
  }

  void ToVal(const std::string& str, int& val) const
  {
    if (str == "male")
    {
      val = 1;
    }
    else if (str == "female")
    {
      val = 2;
    }
    else
    {
      val = 0;
    }
  }
};

class BoolConverter : public rapidcsv::Converter<bool>
{
public:
  void ToStr(bool val, std::string& str) const
  {
    if (val)
    {
      str = "Yes";
    }
    else
    {
      str = "No";
    }
  }

  void ToVal(const std::string& str, bool& val) const
  {
    if (str == "Yes")
    {
      val = true;
    }
    else
    {
      val = false;
    }
  }
};

int main()
{
  int rv = 0;

  std::string csv =
    "Id,Name,Species,Sex,Endangered,Extinct,AvgAge\n"
    "1,Slowness,Snail,,false,No,1\n"
    "2,Dan,Dog,male,false,No,6\n"
    "3,Caty,Cat,female,false,No,7\n"
    "4,Rich,Rino,male,true,No,45\n"
    "5,Dodo,Bird,male,,Yes,\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path);

    StrLenConverter strLenConverter;
    const auto name = doc.GetColumn<size_t>("Name", strLenConverter);
    unittest::ExpectEqual(size_t, name.at(0), 8);
    unittest::ExpectEqual(size_t, name.at(1), 3);
    unittest::ExpectEqual(size_t, name.at(2), 4);
    unittest::ExpectEqual(size_t, name.at(3), 4);
    unittest::ExpectEqual(size_t, name.at(4), 4);

    ModConverter ModConverter;
    const auto species = doc.GetColumn<int>("Species", ModConverter);
    unittest::ExpectEqual(int, species.at(0), 248);
    unittest::ExpectEqual(int, species.at(1), 27);
    unittest::ExpectEqual(int, species.at(2), 25);
    unittest::ExpectEqual(int, species.at(3), 153);
    unittest::ExpectEqual(int, species.at(4), 130);

    const auto sex = doc.GetColumn<std::string>("Sex");
    unittest::ExpectEqual(std::string, sex.at(0), "");
    unittest::ExpectEqual(std::string, sex.at(1), "male");
    unittest::ExpectEqual(std::string, sex.at(2), "female");
    unittest::ExpectEqual(std::string, sex.at(3), "male");
    unittest::ExpectEqual(std::string, sex.at(4), "male");

    SexConverter sexConverter;
    const auto numericSex = doc.GetColumn<int>("Sex", sexConverter);
    std::string tmp;
    for (size_t i = 0; i < sex.size(); i++)
    {
      sexConverter.ToStr(numericSex.at(i), tmp);
      unittest::ExpectEqual(std::string, sex.at(i), tmp);
    }

    const auto endangered = doc.GetColumn<bool>("Endangered");
    unittest::ExpectEqual(bool, endangered.at(0), false);
    unittest::ExpectEqual(bool, endangered.at(1), false);
    unittest::ExpectEqual(bool, endangered.at(2), false);
    unittest::ExpectEqual(bool, endangered.at(3), true);
    unittest::ExpectEqual(bool, endangered.at(4), false);

    BoolConverter boolConverter;
    const auto extinct = doc.GetColumn<bool>("Extinct", boolConverter);
    unittest::ExpectEqual(bool, extinct.at(0), false);
    unittest::ExpectEqual(bool, extinct.at(1), false);
    unittest::ExpectEqual(bool, extinct.at(2), false);
    unittest::ExpectEqual(bool, extinct.at(3), false);
    unittest::ExpectEqual(bool, extinct.at(4), true);

    // Use the default Converter, but with the default
    // converter parameters set.
    // This will cause the missing avg ages to be set to 0.
    const rapidcsv::Converter<int> avgAgeConverter(true, 0);
    const auto avgAge = doc.GetColumn<int>("AvgAge", avgAgeConverter);
    unittest::ExpectEqual(int32_t, avgAge.at(0), 1);
    unittest::ExpectEqual(int32_t, avgAge.at(1), 6);
    unittest::ExpectEqual(int32_t, avgAge.at(2), 7);
    unittest::ExpectEqual(int32_t, avgAge.at(3), 45);
    unittest::ExpectEqual(int32_t, avgAge.at(4), 0);

    // Use the default Converter, but with the default
    // converter parameters and values set.
    // This will cause the missing avg ages to be set to 100.
    const rapidcsv::Converter<int> avgAge100Converter(true, 100);
    const auto avgAge100 = doc.GetColumn<int>("AvgAge", avgAge100Converter);
    unittest::ExpectEqual(int32_t, avgAge100.at(0), 1);
    unittest::ExpectEqual(int32_t, avgAge100.at(1), 6);
    unittest::ExpectEqual(int32_t, avgAge100.at(2), 7);
    unittest::ExpectEqual(int32_t, avgAge100.at(3), 45);
    unittest::ExpectEqual(int32_t, avgAge100.at(4), 100);

    // Use the default Converter, but with the default
    // converter parameters and values set.
    // This will cause the missing avg ages to be set to -1.
    const rapidcsv::Converter<int> avgAge_1Converter(true, -1);
    const auto avgAge_1 = doc.GetColumn<int>("AvgAge", avgAge_1Converter);
    unittest::ExpectEqual(int32_t, avgAge_1.at(0), 1);
    unittest::ExpectEqual(int32_t, avgAge_1.at(1), 6);
    unittest::ExpectEqual(int32_t, avgAge_1.at(2), 7);
    unittest::ExpectEqual(int32_t, avgAge_1.at(3), 45);
    unittest::ExpectEqual(int32_t, avgAge_1.at(4), -1);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
