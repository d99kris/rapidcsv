#if 0
TMP=$(mktemp -d)
c++ -std=c++11 -I src -o ${TMP}/a.out ${0} && ${TMP}/a.out ${@:1} ; RV=${?}
rm -rf ${TMP}
exit ${RV}
#endif

#include <iostream>
#include <vector>
#include "rapidcsv.h"

int main()
{
  rapidcsv::Document doc("examples/colrowhdr.csv", rapidcsv::LabelParams(0, 0));

  std::cout << doc.GetCell<std::string>("Volume", "2017-02-22") << std::endl;
  std::cout << doc.GetCell<int>("Volume", "2017-02-22") << std::endl;
  std::cout << doc.GetCell<long>("Volume", "2017-02-22") << std::endl;
  std::cout << doc.GetCell<long long>("Volume", "2017-02-22") << std::endl;
  std::cout << doc.GetCell<unsigned>("Volume", "2017-02-22") << std::endl;
  std::cout << doc.GetCell<unsigned long>("Volume", "2017-02-22") << std::endl;
  std::cout << doc.GetCell<unsigned long long>("Volume", "2017-02-22") << std::endl;
  std::cout << doc.GetCell<float>("Volume", "2017-02-22") << std::endl;
  std::cout << doc.GetCell<double>("Volume", "2017-02-22") << std::endl;
  std::cout << doc.GetCell<long double>("Volume", "2017-02-22") << std::endl;
  std::cout << doc.GetCell<char>("Volume", "2017-02-22") << std::endl;
}
