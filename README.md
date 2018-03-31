Rapidcsv
========

| **Linux + Mac** | **Windows** |
|-----------------|-------------|
| [![Build status](https://travis-ci.org/d99kris/rapidcsv.svg?branch=master)](https://travis-ci.org/d99kris/rapidcsv) | [![Build status](https://ci.appveyor.com/api/projects/status/yyc65as5ln6m6i8l/branch/master?svg=true)](https://ci.appveyor.com/project/d99kris/rapidcsv/branch/master) |

Rapidcsv is a simple C++ header-only library for CSV parsing. While the name admittedly was inspired
by the rapidjson project, the objectives are not the same. The goal of rapidcsv is to be an easy-to-use
CSV library enabling rapid development. For optimal performance (be it CPU or memory usage) a CSV
parser implemented for the specific use-case is likely to be more performant.

Example Usage
=============
Here is a simple example reading a CSV file and getting 'Close' column as a vector of floats, and an
example of getting a specific cell as well.

```cpp
    #include <iostream>
    #include <vector>
    #include <rapidcsv.h>

    int main()
    {
      rapidcsv::Document doc("../tests/msft.csv");

      std::vector<float> close = doc.GetColumn<float>("Close");
      std::cout << "Read " << close.size() << " values." << std::endl;

      long long volume = doc.GetCell<long long>("Volume", "2011-03-09");
      std::cout << "Volume " << volume << " on 2011-03-09." << std::endl;

      // ...
    }
```

The below section *More Examples* contains more examples(!), and additionally the
[tests](https://github.com/d99kris/rapidcsv/tree/master/tests) directory contains many simple
usage examples as well.

Supported Platforms
===================
Rapidcsv is implemented using C++11 with the intention of being portable. It's been tested on:
- OS X El Capitan 10.11
- Ubuntu 16.04 LTS
- Windows 7 / Visual Studio 2015

Installation
============
Simply copy [src/rapidcsv.h](https://raw.githubusercontent.com/d99kris/rapidcsv/master/src/rapidcsv.h) to your project/include directory and include it. 

More Examples
=============

Reading a File with Custom Separator
------------------------------------
For reading of files with custom separator (i.e. not comma), one need to specify the SeparatorParams 
argument. The following line reads file.csv using semi-colon as separator.

```cpp
    rapidcsv::Document doc("file.csv", rapidcsv::LabelParams(), rapidcsv::SeparatorParams(';'));
```

Reading a File without Headers
------------------------------
By default rapidcsv treats the first row as column headers, and the first column as row headers.
This allows accessing rows/columns/cells using their labels, for example
GetCell<float>("Close", "2011-03-09") to get the cell from column labelled "Close", at row
labelled "2011-03-09". Sometimes one may prefer to be able to access first row and column as data,
and only access cells by their row and column index. In order to do so one need use
LabelParams and set pColumnNameIdx and pRowNameIdx to -1 (disabled). Example:

```cpp
    rapidcsv::Document doc("file.csv", rapidcsv::LabelParams(-1, -1));
```

Reading a File with Invalid Numbers (e.g. Empty Cells) as Numeric Data
-----------------------------------------------------------------------
By default rapidcsv throws an exception if one tries to access non-numeric data as a numeric datatype, as
it basically propagates the underlying conversion routines' exceptions to the calling application.
The reason for this is to ensure data correctness. If one wants to be able to read data with invalid
numbers as numeric datatypes, one can use ConverterParams to configure the converter to default to a
numeric value. The value is configurable, but by default it's
std::numeric_limits<long double>::signaling_NaN() for float types, and 0 for integer types. Example:

```cpp
    rapidcsv::Document doc("file.csv", rapidcsv::LabelParams(), rapidcsv::SeparatorParams(), rapidcsv::ConverterParams(true));
```


API Documentation
=================

Document Constructors
---------------------
The Document can be created empty, or based on reading the content of specified file path.
Using the classes LabelParams, SeparatorParams and ConverterParams, one can control which column
and row index should be treated as labels, which field separator to use, etc.

```cpp
    explicit rapidcsv::LabelParams(const int pColumnNameIdx = 0, const int pRowNameIdx = 0);
    explicit rapidcsv::SeparatorParams(const char pSeparator = ',', const bool pHasCR = sPlatformHasCR);
    explicit rapidcsv::ConverterParams(const bool pHasDefaultConverter = false,
                                       const long double pDefaultFloat = std::numeric_limits<long double>::signaling_NaN(),
                                       const long long pDefaultInteger = 0);
    explicit rapidcsv::Document(const std::string& pPath = std::string(),                 
                                const LabelParams& pLabelParams = LabelParams(),          
                                const SeparatorParams& pSeparatorParams = SeparatorParams(),
                                const ConverterParams& pConverterParams = ConverterParams());
    explicit rapidcsv::Document(const rapidcsv::Document& pDocument);
```

Document Load / Save
--------------------
The following methods allow loading and saving document in CSV format to specified path.

```cpp
    void rapidcsv::Document::Load(const std::string& pPath);
    void rapidcsv::Document::Save(const std::string& pPath = "");
```

Get Data Size
-------------
The following methods provide getting number of columns and rows of the data.

```cpp
    size_t rapidcsv::Document::GetColumnCount() const;
    size_t rapidcsv::Document::GetRowCount() const;
```

Get Data
--------
Data can be accessed by column, row or cell, using the following Get methods. These methods produce a
copy of the data in specified datatype. The requested position in the spreadsheet is either specified
using their label(s), or the zero-based position index.

```cpp
    template<typename T>
    std::vector<T> rapidcsv::Document::GetColumn(const size_t pColumnIdx) const;
    template<typename T>
    std::vector<T> rapidcsv::Document::GetColumn(const std::string& pColumnName) const;
    
    template<typename T>
    std::vector<T> rapidcsv::Document::GetRow(const size_t pRowIdx) const;
    template<typename T>
    std::vector<T> rapidcsv::Document::GetRow(const std::string& pRowName) const;
    
    template<typename T>
    T rapidcsv::Document::Document::GetCell(const size_t pColumnIdx, const size_t pRowIdx) const;

    template<typename T>
    T rapidcsv::Document::GetCell(const std::string& pColumnName, const std::string& pRowName) const;
```

Set Data
--------
Since the Get methods listed above creates a copy of the actual data in the Document class, one needs
to use Set methods to modify the data.

```cpp
    template<typename T>
    void rapidcsv::Document::SetColumn(const size_t pColumnIdx, const std::vector<T>& pColumn);
    
    template<typename T>
    void rapidcsv::Document::SetColumn(const std::string& pColumnName, const std::vector<T>& pColumn);
    
    template<typename T>
    void rapidcsv::Document::SetRow(const size_t pRowIdx, const std::vector<T>& pRow);
    
    template<typename T>
    void rapidcsv::Document::SetRow(const std::string& pRowName, const std::vector<T>& pRow);
    
    template<typename T>
    void rapidcsv::Document::SetCell(const size_t pColumnIdx, const size_t pRowIdx, const T& pCell);
    
    template<typename T>
    void rapidcsv::Document::SetCell(const std::string& pColumnName, const std::string& pRowName, const T& pCell);
```

Modify Document Structure
-------------------------
These methods can be used for modifying document structure.

```cpp
    void rapidcsv::Document::RemoveColumn(const size_t pColumnIdx);
    void rapidcsv::Document::RemoveColumn(const std::string& pColumnName);
    void rapidcsv::Document::RemoveRow(const size_t pRowIdx);
    void rapidcsv::Document::RemoveRow(const std::string& pRowName);
    void rapidcsv::Document::SetColumnLabel(size_t pColumnIdx, const std::string& pColumnName);
    void rapidcsv::Document::SetRowLabel(size_t pRowIdx, const std::string& pRowName);
```

Custom Data Conversion
----------------------
The internal cell representation in the Document class is using std::string and when other types are
requested, standard conversion routines are used. One may override conversion routines (or add new ones)
by implementing ToVal() and ToStr(). Here is an example overriding int conversion, to instead provide
two decimal fixed-point numbers.
See [tests/test035.cpp](https://github.com/d99kris/rapidcsv/blob/master/tests/test035.cpp) for a complete
program example.

```cpp
    namespace rapidcsv
    {
      template<>
      void Converter<int>::ToVal(const std::string& pStr, int& pVal) const
      {
        pVal = roundf(100.0 * stof(pStr));
      }
    
      template<>
      void Converter<int>::ToStr(const int& pVal, std::string& pStr) const
      {
        std::ostringstream out;
        out << std::fixed << std::setprecision(2) << static_cast<float>(pVal) / 100.0f;
        pStr = out.str();
      }
    }
```

Technical Details
=================
Rapidcsv uses cmake for its tests. Commands to build and execute the test suite:

    mkdir -p build && cd build && cmake .. && make && ctest --output-on-failure ; cd -

Alternatives
============
There are many CSV parsers for C++, for example:
- [CSVparser](https://github.com/MyBoon/CSVparser)
- [Fast C++ CSV Parser](https://github.com/ben-strasser/fast-cpp-csv-parser)

License
=======
Rapidcsv is distributed under the BSD 3-Clause license. See
[LICENSE](https://github.com/d99kris/rapidcsv/blob/master/LICENSE) file.

Contributions
=============
Bugs, PRs, etc are welcome on the GitHub project page https://github.com/d99kris/rapidcsv

Keywords
========
c++, c++11, csv parser, comma separated values, single header library.

