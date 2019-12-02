#if 0
TMP=$(mktemp -d)
c++ -std=c++11 -I src -o ${TMP}/a.out ${0} && ${TMP}/a.out ${@:1} ; RV=${?}
rm -rf ${TMP}
exit ${RV}
#endif

#include "rapidcsv.h"

namespace rapidcsv
{
  template<typename T>
  class CustomConverter : public Converter<T>
  {
  public:
    explicit CustomConverter(bool pHasDefaultConverter, const T& pDefaultVal)
      : Converter<T>(pHasDefaultConverter, pDefaultVal)
    {
    }

    void ToVal(const std::string& pStr, T& pVal) const;
    void ToStr(const T& pVal, std::string& pStr) const;
  };

  template<>
  void CustomConverter<int>::ToVal(const std::string& pStr, int& pVal) const
  {
    Converter<int>::ToVal(pStr, pVal);
    pVal /= 4;
  }

  template<>
  void CustomConverter<int>::ToStr(const int& pVal, std::string& pStr) const
  {
    const auto val = pVal * 4;
    Converter<int>::ToStr(val, pStr);
  }
}

int main()
{
  std::stringstream sstream("1,10,,1000,X\n");
  rapidcsv::Document doc(sstream, rapidcsv::LabelParams(-1, -1));

  // Create a custom converter that will force empty/invalid values
  // to 4000. The converter will convert all inputs to integer values
  // and then divide them by 4.
  rapidcsv::CustomConverter<int> customConverter(true, 4000);

  for (auto i = 0; i < 5; ++i)
  {
    std::cout << doc.GetCell<int>(i, 0, customConverter) << std::endl;
  }

  // Output:
  // 0        // int(1 / 4) = 0
  // 2        // int(10 / 4) = 2
  // 1000     // int(4000 / 4) = 1000, as '' -> 4000 (default).
  // 250      // int(1000 / 4) = 250
  // 1000     // int(4000 / 4) = 1000, as 'X' -> 4000 (default).
}
