#if 0
TMP=$(mktemp -d)
c++ -std=c++11 -I src -o ${TMP}/a.out ${0} && ${TMP}/a.out ${@:1} ; RV=${?}
rm -rf ${TMP}
exit ${RV}
#endif

#include <iostream>
#include <vector>
#include "rapidcsv.h"

void ConvFixPoint(const std::string& pStr, int& pVal)
{
  pVal = static_cast<int>(roundf(100.0f * std::stof(pStr)));
}

struct MyStruct
{
  int val = 0;
};

void ConvMyStruct(const std::string& pStr, MyStruct& pVal)
{
  pVal.val = static_cast<int>(roundf(100.0f * std::stof(pStr)));
}

int main()
{
  rapidcsv::Document doc("examples/colrowhdr.csv", rapidcsv::LabelParams(0, 0));

  std::cout << "regular         = " << doc.GetCell<int>("Close", "2017-02-21") << "\n";
  std::cout << "fixpointfunc    = " << doc.GetCell<int>("Close", "2017-02-21", ConvFixPoint) << "\n";

  auto convFixLambda = [](const std::string& pStr, int& pVal) { pVal = static_cast<int>(roundf(100.0f * stof(pStr))); };
  std::cout << "fixpointlambda  = " << doc.GetCell<int>("Close", "2017-02-21", convFixLambda) << "\n";

  std::cout << "mystruct        = " << doc.GetCell<MyStruct>("Close", "2017-02-21", ConvMyStruct).val << "\n";
}
