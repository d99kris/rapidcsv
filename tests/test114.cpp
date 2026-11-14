// test114.cpp - auto-quote cells containing whitespace

#include <rapidcsv.h>
#include "unittest.h"

static std::string LoadSave(const std::string& pCsv, bool pTrim)
{
  std::istringstream input(pCsv);
  rapidcsv::Document doc(input, rapidcsv::LabelParams(-1, -1),
                         rapidcsv::SeparatorParams(',', pTrim, false));
  std::ostringstream output;
  doc.Save(output);
  return output.str();
}

int main()
{
  int rv = 0;

  try
  {
    // Whitespace anywhere in a cell must stay quoted, with or without trimming.
    for (bool trim : { true, false })
    {
      unittest::ExpectEqual(std::string, LoadSave("\"\tvalue\",a\n", trim), "\"\tvalue\",a\n");
      unittest::ExpectEqual(std::string, LoadSave("\"value\v\",a\n", trim), "\"value\v\",a\n");
      unittest::ExpectEqual(std::string, LoadSave("\"left\fright\",a\n", trim), "\"left\fright\",a\n");
    }

    // Empty cells need no quotes.
    unittest::ExpectEqual(std::string, LoadSave("\"\",a\n", true), ",a\n");
  }
  catch (const std::exception& ex)
  {
    std::cout << "exception: " << ex.what() << std::endl;
    rv = 1;
  }

  return rv;
}
