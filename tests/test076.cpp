// test076.cpp - Load from stream

#include <rapidcsv.h>
#include "unittest.h"

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
    // stream from file
    std::ifstream fstream;
    fstream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fstream.open(path, std::ios::binary | std::ios::ate);
    rapidcsv::Document doc1;
    doc1.Load(fstream, rapidcsv::LabelParams(0, 0));
    fstream.close();

    unittest::ExpectEqual(int, doc1.GetCell<int>(0, 0), 3);
    unittest::ExpectEqual(int, doc1.GetCell<int>(1, 0), 9);
    unittest::ExpectEqual(int, doc1.GetCell<int>(2, 0), 81);

    unittest::ExpectEqual(std::string, doc1.GetCell<std::string>("A", "2"), "4");
    unittest::ExpectEqual(std::string, doc1.GetCell<std::string>("B", "2"), "16");
    unittest::ExpectEqual(std::string, doc1.GetCell<std::string>("C", "2"), "256");

    // stream from string
    std::istringstream sstream(csv);
    rapidcsv::Document doc2("");
    doc2.Load(sstream, rapidcsv::LabelParams(0, 0));

    unittest::ExpectEqual(int, doc2.GetCell<int>(0, 0), 3);
    unittest::ExpectEqual(int, doc2.GetCell<int>(1, 0), 9);
    unittest::ExpectEqual(int, doc2.GetCell<int>(2, 0), 81);

    unittest::ExpectEqual(std::string, doc2.GetCell<std::string>("A", "2"), "4");
    unittest::ExpectEqual(std::string, doc2.GetCell<std::string>("B", "2"), "16");
    unittest::ExpectEqual(std::string, doc2.GetCell<std::string>("C", "2"), "256");
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
