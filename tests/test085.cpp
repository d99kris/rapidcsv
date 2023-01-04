// test085.cpp - read UTF-16 LE file

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  try
  {
    rapidcsv::Document doc("../tests/chi-utf16.csv", rapidcsv::LabelParams(0, 0), rapidcsv::SeparatorParams(';'));

    unittest::ExpectEqual(std::string, doc.GetCell<std::string>("description", "0"), "等待同PLC的Profinet通讯");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>("description", "1"), "辅助关闭");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>("description", "2"), "Z1轴运行中转轮锁定");

    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 7), "设备不正常");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 8), "标准件校对：工装翻转补偿值不正确");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 39), "达到扭矩限制");
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  return rv;
}
