// test088.cpp - test parsing of files larger than 64 KB

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  const std::string path = unittest::TempPath();

  try
  {
    const int dataRows = 5000;

    // write test file
    {
      std::ofstream out(path);
      out << "Foo,Bar,Baz" << std::endl;
      for (int i = 0; i < dataRows; i++)
      {
        out << i << "," << i * 2 << "," << i * 3 << std::endl;
      }
    }

    // read using filename, check row count
    {
      rapidcsv::Document doc(path);
      unittest::ExpectEqual(size_t, doc.GetRowCount(), dataRows);
    }

    // read via non-binary ifstream, check row count
    {
      std::ifstream in(path);
      rapidcsv::Document doc(in);
      unittest::ExpectEqual(size_t, doc.GetRowCount(), dataRows);
    }

    // parse msft.csv using filename, sanity check certain values
    const size_t msftDataRows = 7804;
    const long long msftVolumeTopRow = 21705200;
    const long long msftVolumeBottomRow = 1031788800;
    const long long msftMinVolume = 2304000;
    const long long msftMaxVolume = 1031788800;

    {
      rapidcsv::Document doc("../tests/msft.csv");
      std::vector<float> close = doc.GetColumn<float>("Close");
      std::vector<long long> volume = doc.GetColumn<long long>("Volume");
      unittest::ExpectEqual(size_t, doc.GetRowCount(), msftDataRows);
      unittest::ExpectEqual(size_t, close.size(), msftDataRows);
      unittest::ExpectEqual(size_t, volume.size(), msftDataRows);
      unittest::ExpectEqual(long long, volume.front(), msftVolumeTopRow);
      unittest::ExpectEqual(long long, volume.back(), msftVolumeBottomRow);
      const auto minMaxVolume = std::minmax_element(volume.begin(), volume.end());
      unittest::ExpectEqual(long long, *minMaxVolume.first, msftMinVolume);
      unittest::ExpectEqual(long long, *minMaxVolume.second, msftMaxVolume);
    }

    // parse msft.csv via non-binary ifstream, sanity check certain values
    {
      std::ifstream in("../tests/msft.csv");
      rapidcsv::Document doc(in);
      std::vector<float> close = doc.GetColumn<float>("Close");
      std::vector<long long> volume = doc.GetColumn<long long>("Volume");
      unittest::ExpectEqual(size_t, doc.GetRowCount(), msftDataRows);
      unittest::ExpectEqual(size_t, close.size(), msftDataRows);
      unittest::ExpectEqual(size_t, volume.size(), msftDataRows);
      unittest::ExpectEqual(long long, volume.front(), msftVolumeTopRow);
      unittest::ExpectEqual(long long, volume.back(), msftVolumeBottomRow);
      const auto minMaxVolume = std::minmax_element(volume.begin(), volume.end());
      unittest::ExpectEqual(long long, *minMaxVolume.first, msftMinVolume);
      unittest::ExpectEqual(long long, *minMaxVolume.second, msftMaxVolume);
    }
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
