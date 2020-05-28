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
  const std::string& csv =
    "Date,Open,High,Low,Close,Volume,Adj Close\n"
    "2017-02-24,64.529999,64.800003,64.139999,64.620003,21705200,64.620003\n"
    "2017-02-23,64.419998,64.730003,64.190002,64.620003,20235200,64.620003\n"
    "2017-02-22,64.330002,64.389999,64.050003,64.360001,19259700,64.360001\n"
    "2017-02-21,64.610001,64.949997,64.449997,64.489998,19384900,64.489998\n"
    "2017-02-17,64.470001,64.690002,64.300003,64.620003,21234600,64.620003\n"
  ;

  std::stringstream sstream(csv);
  rapidcsv::Document doc(sstream, rapidcsv::LabelParams(0, 0));

  std::vector<float> close = doc.GetColumn<float>("Close");
  std::cout << "Read " << close.size() << " values." << std::endl;

  long long volume = doc.GetCell<long long>("Volume", "2017-02-22");
  std::cout << "Volume " << volume << " on 2017-02-22." << std::endl;
}
