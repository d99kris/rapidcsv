#include <iostream>
#include <vector>
#include <rapidcsv.h>

int main()
{
  rapidcsv::Document doc("../../colhdr.csv");

  std::vector<float> col = doc.GetColumn<float>("Close");
  std::cout << "Read " << col.size() << " values." << std::endl;
}
