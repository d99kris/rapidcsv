// test090.cpp - custom quote char handling

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string csv =
    "1997,Ford,,Truck\n"
    " 1997, Ford, E350, Truck\n"
    " 1997 , Ford , E350 , Truck \n"
    "'1997','Ford','E350','Truck'\n"
    "1997,Ford,E350,'Super, luxurious truck'\n"
    "1997,Ford,E350,'Super, ''luxurious, fast'' truck'\n"

    "Los Angeles,34°03'N,118°15'W,US\n"
    "New York City,40°42'46\"N,74°00'21\"W,US\n"
    "Paris,48°51'24\"N,2°21'03\"E,France\n"
  ;

  std::string path = unittest::TempPath();
  unittest::WriteFile(path, csv);

  try
  {
    rapidcsv::SeparatorParams separatorParams;
    separatorParams.mQuoteChar = '\'';
    rapidcsv::Document doc(path, rapidcsv::LabelParams(-1, -1), separatorParams);

    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 0), "1997");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(1, 0), "Ford");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(2, 0), "");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(3, 0), "Truck");

    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 1), " 1997");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(1, 1), " Ford");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(2, 1), " E350");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(3, 1), " Truck");

    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 2), " 1997 ");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(1, 2), " Ford ");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(2, 2), " E350 ");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(3, 2), " Truck ");

    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 3), "1997");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(1, 3), "Ford");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(2, 3), "E350");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(3, 3), "Truck");

    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 4), "1997");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(1, 4), "Ford");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(2, 4), "E350");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(3, 4), "Super, luxurious truck");

    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 5), "1997");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(1, 5), "Ford");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(2, 5), "E350");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(3, 5), "Super, 'luxurious, fast' truck");

    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 6), "Los Angeles");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(1, 6), "34°03'N");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(2, 6), "118°15'W");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(3, 6), "US");

    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 7), "New York City");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(1, 7), "40°42'46\"N");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(2, 7), "74°00'21\"W");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(3, 7), "US");

    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 8), "Paris");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(1, 8), "48°51'24\"N");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(2, 8), "2°21'03\"E");
    unittest::ExpectEqual(std::string, doc.GetCell<std::string>(3, 8), "France");
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
    rv = 1;
  }

  unittest::DeleteFile(path);

  return rv;
}
