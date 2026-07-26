// test109.cpp - write and read cells containing carriage return

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  std::string path = unittest::TempPath();

  try
  {
    rapidcsv::SeparatorParams seppar(',' /* pSeparator */, false /* pTrim */, false /* pHasCR */,
                                     true /* pQuotedLinebreaks */, true /* pAutoQuote */);

    // write cells containing carriage return and linefeed
    {
      rapidcsv::Document doc(std::string(), rapidcsv::LabelParams(-1, -1), seppar);
      doc.SetCell<std::string>(0, 0, "hello\rworld");
      doc.SetCell<std::string>(1, 0, "hello\nworld");
      doc.Save(path);
    }

    // both cells shall be quoted
    const std::string csvref = "\"hello\rworld\",\"hello\nworld\"\n";
    unittest::ExpectEqual(std::string, unittest::ReadFile(path), csvref);

    // read back, cell content shall be preserved
    {
      rapidcsv::Document doc(path, rapidcsv::LabelParams(-1, -1), seppar);

      unittest::ExpectEqual(size_t, doc.GetRowCount(), 1);
      unittest::ExpectEqual(size_t, doc.GetColumnCount(), 2);

      unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 0), "hello\rworld");
      unittest::ExpectEqual(std::string, doc.GetCell<std::string>(1, 0), "hello\nworld");
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
