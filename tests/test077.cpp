// test077.cpp - copy constructor

#include <rapidcsv.h>
#include "unittest.h"

rapidcsv::Document LoadDoc(const std::string& p_Path)
{
  rapidcsv::Document doc = rapidcsv::Document(p_Path, rapidcsv::LabelParams(0, 0));
  return doc;
}

int main()
{
  int rv = 0;

  std::string csv =
    "-,A,B,C\n"
    "1,3,9,81\n"
    "2,4,16,256\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc1 = LoadDoc(path);
    unittest::ExpectEqual(int, doc1.GetCell<int>(0, 0), 3);
    unittest::ExpectEqual(int, doc1.GetCell<int>(1, 0), 9);
    unittest::ExpectEqual(int, doc1.GetCell<int>(2, 0), 81);

    const rapidcsv::Document& doc2 = LoadDoc(path);
    unittest::ExpectEqual(std::string, doc2.GetCell<std::string>("A", "2"), "4");
    unittest::ExpectEqual(std::string, doc2.GetCell<std::string>("B", "2"), "16");
    unittest::ExpectEqual(std::string, doc2.GetCell<std::string>("C", "2"), "256");

    rapidcsv::Document doc3 = doc2;
    unittest::ExpectEqual(int, doc1.GetCell<int>(0, 0), 3);
    unittest::ExpectEqual(int, doc1.GetCell<int>(1, 0), 9);
    unittest::ExpectEqual(int, doc1.GetCell<int>(2, 0), 81);
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
