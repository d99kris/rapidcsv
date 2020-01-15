#if 0
TMP=$(mktemp -d)
c++ -std=c++11 -I src -o ${TMP}/a.out ${0} && ${TMP}/a.out ${@:1} ; RV=${?}
rm -rf ${TMP}
exit ${RV}
#endif

#include <iostream>
#include <vector>
#include "rapidcsv.h"

namespace rapidcsv
{
  template<>
  void Converter<int>::ToVal(const std::string& pStr, int& pVal) const
  {
    pVal = roundf(100.0 * stof(pStr));
  }
}

int main()
{
  rapidcsv::Document doc("examples/colrowhdr.csv");

  std::vector<int> close = doc.GetColumn<int>("Close");
  std::cout << "close[0]  = " << close[0] << std::endl;
  std::cout << "close[1]  = " << close[1] << std::endl;
}
