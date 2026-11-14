// test114.cpp - preserve quoted whitespace when trimming is enabled

#include <rapidcsv.h>
#include "unittest.h"

int main()
{
  int rv = 0;

  try
  {
    for (char quote : { '"', '\'' })
    {
      for (char whitespace : { '\t', '\v', '\f' })
      {
        for (const std::string& value : { std::string(1, whitespace),
                                         std::string(1, whitespace) + "value",
                                         "value" + std::string(1, whitespace),
                                         "left" + std::string(1, whitespace) + "right" })
        {
          for (bool trim : { false, true })
          {
            const rapidcsv::SeparatorParams params(',', trim, false, false, true, quote);
            const std::string quoteStr(1, quote);
            std::istringstream input(quoteStr + value + quoteStr + ",plain\n");
            rapidcsv::Document doc(input, rapidcsv::LabelParams(-1, -1), params);
            unittest::ExpectEqual(std::string, doc.GetCell<std::string>(0, 0), value);

            std::ostringstream output;
            doc.Save(output);
            const bool needsQuote = trim && ((value.front() == whitespace) || (value.back() == whitespace));
            const std::string expected = (needsQuote ? quoteStr + value + quoteStr : value) + ",plain\n";
            unittest::ExpectEqual(std::string, output.str(), expected);

            std::istringstream saved(output.str());
            rapidcsv::Document reloaded(saved, rapidcsv::LabelParams(-1, -1), params);
            unittest::ExpectEqual(std::string, reloaded.GetCell<std::string>(0, 0), value);
            unittest::ExpectEqual(std::string, reloaded.GetCell<std::string>(1, 0), "plain");
            unittest::ExpectEqual(size_t, reloaded.GetRowCount(), 1);
            unittest::ExpectEqual(size_t, reloaded.GetColumnCount(), 2);
          }
        }
      }
    }

    // Empty and non-ASCII cells do not need whitespace protection.
    rapidcsv::Document boundaries("", rapidcsv::LabelParams(-1, -1),
                                  rapidcsv::SeparatorParams(',', true, false));
    boundaries.SetCell<std::string>(0, 0, "");
    boundaries.SetCell<std::string>(1, 0, "\xc3\xa9");
    std::ostringstream boundaryOutput;
    boundaries.Save(boundaryOutput);
    unittest::ExpectEqual(std::string, boundaryOutput.str(), ",\xc3\xa9\n");

    // AutoQuote disabled continues writing literal cell contents.
    rapidcsv::Document literal("", rapidcsv::LabelParams(-1, -1),
                               rapidcsv::SeparatorParams(',', true, false, false, false));
    literal.SetCell<std::string>(0, 0, "\tvalue\t");
    literal.SetCell<std::string>(1, 0, "");
    std::ostringstream output;
    literal.Save(output);
    unittest::ExpectEqual(std::string, output.str(), "\tvalue\t,\n");
  }
  catch (const std::exception& ex)
  {
    std::cout << "exception: " << ex.what() << std::endl;
    rv = 1;
  }

  return rv;
}
