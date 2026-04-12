// test106.cpp - write round-trip with cells containing quote characters

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  try
  {
    // Test 1: Cell containing quote character but no separator/space/newline
    {
      std::string path = unittest::TempPath();
      std::string csv =
        "a,b,c\n"
        "1,he said \"hello\",3\n"
        "4,5,6\n"
      ;
      unittest::WriteFile(path, csv);

      rapidcsv::Document doc1(path, rapidcsv::LabelParams(-1, -1));
      unittest::ExpectEqual(std::string, doc1.GetCell<std::string>(1, 1),
                            "he said \"hello\"");

      // Save and reload - data must survive round-trip
      doc1.Save(path);
      rapidcsv::Document doc2(path, rapidcsv::LabelParams(-1, -1));

      unittest::ExpectEqual(size_t, doc2.GetRowCount(), size_t(3));
      unittest::ExpectEqual(size_t, doc2.GetColumnCount(), size_t(3));
      unittest::ExpectEqual(std::string, doc2.GetCell<std::string>(1, 1),
                            "he said \"hello\"");

      unittest::DeleteFile(path);
    }

    // Test 2: Cell containing only a quote character
    {
      std::string path = unittest::TempPath();

      std::istringstream ss("a,b\nx,\"y\"\n");
      rapidcsv::Document doc1(ss, rapidcsv::LabelParams(-1, -1));
      doc1.SetCell<std::string>(1, 0, "\"");
      doc1.Save(path);

      rapidcsv::Document doc2(path, rapidcsv::LabelParams(-1, -1));
      unittest::ExpectEqual(std::string, doc2.GetCell<std::string>(1, 0), "\"");

      unittest::DeleteFile(path);
    }

    // Test 3: Empty quoted cells round-trip
    {
      std::string path = unittest::TempPath();
      std::string csv = "\"\",\"\",\"\"\n";
      unittest::WriteFile(path, csv);

      rapidcsv::Document doc1(path, rapidcsv::LabelParams(-1, -1));
      unittest::ExpectEqual(std::string, doc1.GetCell<std::string>(0, 0), "");
      unittest::ExpectEqual(std::string, doc1.GetCell<std::string>(1, 0), "");
      unittest::ExpectEqual(std::string, doc1.GetCell<std::string>(2, 0), "");

      doc1.Save(path);
      rapidcsv::Document doc2(path, rapidcsv::LabelParams(-1, -1));
      unittest::ExpectEqual(size_t, doc2.GetColumnCount(), size_t(3));
      unittest::ExpectEqual(std::string, doc2.GetCell<std::string>(0, 0), "");
      unittest::ExpectEqual(std::string, doc2.GetCell<std::string>(1, 0), "");
      unittest::ExpectEqual(std::string, doc2.GetCell<std::string>(2, 0), "");

      unittest::DeleteFile(path);
    }
  }
  catch (const std::exception& ex)
  {
    std::cout << "exception: " << ex.what() << std::endl;
    rv = 1;
  }

  return rv;
}
