// test115.cpp - SetRow and SetCell with row labels grow rows and preserve wider rows

#include <rapidcsv.h>
#include "unittest.h"

static std::string Save(const rapidcsv::Document& pDoc)
{
  std::ostringstream output;
  pDoc.Save(output);
  return output.str();
}

int main()
{
  int rv = 0;

  try
  {
    // SetRow growing a row with row labels.
    {
      std::istringstream input("id,A\nr1,1\n");
      rapidcsv::Document doc(input, rapidcsv::LabelParams(0, 0),
                             rapidcsv::SeparatorParams(',', false, false));
      doc.SetRow<int>(0, { 7, 8 });
      unittest::ExpectEqual(std::string, Save(doc), "id,A,\nr1,7,8\n");
    }

    // SetRow must preserve cells of a row wider than the new row.
    {
      std::istringstream input("id,A\nr1,1,2,3,4\n");
      rapidcsv::Document doc(input, rapidcsv::LabelParams(0, 0),
                             rapidcsv::SeparatorParams(',', false, false));
      doc.SetRow<int>(0, { 7, 8, 9 });
      unittest::ExpectEqual(std::string, Save(doc), "id,A,,\nr1,7,8,9,4\n");
    }

    // SetCell must preserve cells of a row wider than the header.
    {
      std::istringstream input("id,A\nr1,1,2,3\n");
      rapidcsv::Document doc(input, rapidcsv::LabelParams(0, 0),
                             rapidcsv::SeparatorParams(',', false, false));
      doc.SetCell<int>(1, 0, 9);
      unittest::ExpectEqual(std::string, Save(doc), "id,A,\nr1,1,9,3\n");
    }
  }
  catch (const std::exception& ex)
  {
    std::cout << "exception: " << ex.what() << std::endl;
    rv = 1;
  }

  return rv;
}
