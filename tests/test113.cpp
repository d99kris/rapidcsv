// test113.cpp - growing a column applies the row label offset only once

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  try
  {
    for (int rowNameIdx : { -1, 0, 2 })
    {
      for (int columnNameIdx : { -1, 0, 2 })
      {
        rapidcsv::Document doc("", rapidcsv::LabelParams(columnNameIdx, rowNameIdx),
                               rapidcsv::SeparatorParams(',', false, false));

        doc.SetColumn(0, std::vector<int>({ 1, 2 }));
        unittest::ExpectEqual(size_t, doc.GetColumnCount(), 1);
        unittest::ExpectEqual(size_t, doc.GetRowCount(), 2);
        unittest::ExpectEqual(int, doc.GetCell<int>(0, 0), 1);
        unittest::ExpectEqual(int, doc.GetCell<int>(0, 1), 2);

        // Extend the document by both one column and one row.
        doc.SetColumn(1, std::vector<int>({ 3, 4, 5 }));
        unittest::ExpectEqual(size_t, doc.GetColumnCount(), 2);
        unittest::ExpectEqual(size_t, doc.GetRowCount(), 3);
        unittest::ExpectEqual(int, doc.GetCell<int>(1, 2), 5);

        // Updating an existing column must not grow the document.
        doc.SetColumn(0, std::vector<int>({ 6, 7, 8 }));
        unittest::ExpectEqual(size_t, doc.GetColumnCount(), 2);
      }
    }

    // Saving must not add empty columns.
    {
      rapidcsv::Document doc("", rapidcsv::LabelParams(0, 0),
                             rapidcsv::SeparatorParams(',', false, false));
      doc.SetColumn(0, std::vector<int>({ 1, 2 }));
      std::ostringstream output;
      doc.Save(output);
      unittest::ExpectEqual(std::string, output.str(), ",\n,1\n,2\n");
    }

    // Growing past a short header must preserve wider data rows.
    {
      std::istringstream input("id,A\nr1,1,2,3\n");
      rapidcsv::Document doc(input, rapidcsv::LabelParams(0, 0));
      doc.SetColumn(1, std::vector<int>({ 8 }));
      unittest::ExpectEqual(size_t, doc.GetColumnCount(), 2);
      unittest::ExpectEqual(int, doc.GetCell<int>(2, 0), 3);
      std::ostringstream output;
      doc.Save(output);
      unittest::ExpectEqual(std::string, output.str(), "id,A,\nr1,1,8,3\n");
    }
  }
  catch (const std::exception& ex)
  {
    std::cout << "exception: " << ex.what() << std::endl;
    rv = 1;
  }

  return rv;
}
