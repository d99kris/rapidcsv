// test042.cpp - read column values as char

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csv =
    "-,A,B,C\n"
    "1,a,b,c\n"
    "2,x,y,w\n"
    ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::Document doc(path);

    std::vector<char> chars;
    std::vector<std::string> strs;

    chars = doc.GetColumn<char>(0);
    unittest::ExpectEqual(size_t, chars.size(), 2);
    unittest::ExpectEqual(char, chars.at(0), 'a');
    unittest::ExpectEqual(char, chars.at(1), 'x');

    chars = doc.GetColumn<char>("B");
    unittest::ExpectEqual(size_t, chars.size(), 2);
    unittest::ExpectEqual(char, chars.at(0), 'b');
    unittest::ExpectEqual(char, chars.at(1), 'y');

    strs = doc.GetColumn<std::string>(2);
    unittest::ExpectEqual(size_t, strs.size(), 2);
    unittest::ExpectEqual(std::string, strs.at(0), "c");
    unittest::ExpectEqual(std::string, strs.at(1), "w");
  }
  catch(const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
