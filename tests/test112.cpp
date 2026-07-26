// test112.cpp - read and write UTF-16 with characters outside basic multilingual plane

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  // "-,A,B\n"
  // "1,\xf0\x9f\x98\x80,\xe4\xb8\xad\n"
  // i.e. U+1F600 (surrogate pair) and U+4E2D (single code unit)
  const unsigned char u16le[] =
  {
    0xff, 0xfe,
    0x2d, 0x00, 0x2c, 0x00, 0x41, 0x00, 0x2c, 0x00, 0x42, 0x00, 0x0a, 0x00,
    0x31, 0x00, 0x2c, 0x00, 0x3d, 0xd8, 0x00, 0xde, 0x2c, 0x00, 0x2d, 0x4e,
    0x0a, 0x00
  };
  const unsigned int u16le_len = 28;

  const unsigned char u16be[] =
  {
    0xfe, 0xff,
    0x00, 0x2d, 0x00, 0x2c, 0x00, 0x41, 0x00, 0x2c, 0x00, 0x42, 0x00, 0x0a,
    0x00, 0x31, 0x00, 0x2c, 0xd8, 0x3d, 0xde, 0x00, 0x00, 0x2c, 0x4e, 0x2d,
    0x00, 0x0a
  };
  const unsigned int u16be_len = 28;

  const std::string emoji = "\xf0\x9f\x98\x80"; // U+1F600
  const std::string zhong = "\xe4\xb8\xad"; // U+4E2D

  const std::string csvle(reinterpret_cast<const char*>(u16le), u16le_len);
  const std::string csvbe(reinterpret_cast<const char*>(u16be), u16be_len);

  std::string path = unittest::TempPath();

  try
  {
    // read UTF-16 LE
    unittest::WriteFile(path, csvle);
    {
      rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>("A", "1"), emoji);
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>("B", "1"), zhong);
    }

    // read UTF-16 BE
    unittest::WriteFile(path, csvbe);
    {
      rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>("A", "1"), emoji);
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>("B", "1"), zhong);
    }

    // write UTF-16 LE
    unittest::WriteFile(path, csvle);
    {
      rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));
      doc.SetCell<std::string>("A", "1", emoji);
      doc.Save();
      unittest::ExpectEqual(std::string, unittest::ReadFile(path), csvle);
    }

    // write UTF-16 BE
    unittest::WriteFile(path, csvbe);
    {
      rapidcsv::Document doc(path, rapidcsv::LabelParams(0, 0));
      doc.SetCell<std::string>("A", "1", emoji);
      doc.Save();
      unittest::ExpectEqual(std::string, unittest::ReadFile(path), csvbe);
    }

    // read UTF-16 LE stream
    {
      std::istringstream sstream(csvle);
      rapidcsv::Document doc(sstream, rapidcsv::LabelParams(0, 0));
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>("A", "1"), emoji);
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>("B", "1"), zhong);
    }
  }
  catch (const std::exception& ex)
  {
    std::cout << "exception: " << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
