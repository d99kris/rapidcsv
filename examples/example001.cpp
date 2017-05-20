#include <iostream>
#include <vector>
#include <rapidcsv.h>

int main()
{
  rapidcsv::Document doc("../tests/msft.csv");

  std::vector<float> close = doc.GetColumn<float>("Close");
  std::cout << "Read " << close.size() << " values." << std::endl;

  long long volume = doc.GetCell<long long>("Volume", "2011-03-09");
  std::cout << "Volume " << volume << " on 2011-03-09." << std::endl;

  // ...
}
