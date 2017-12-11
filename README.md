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
A simple example reading a CSV file and getting 'Close' column as a vector of floats, and getting a
specific cell as well.

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

The [tests](https://github.com/d99kris/rapidcsv/tree/master/tests) directory contains many simple usage
examples as well.

Supported Platforms
===================
Rapidcsv is implemented using C++11 with the intention of being portable. It's been tested on:
- OS X El Capitan 10.11
- Ubuntu 16.04 LTS
- Windows 7 / Visual Studio 2015

Installation
============
Simply copy [src/rapidcsv.h](https://raw.githubusercontent.com/d99kris/rapidcsv/master/src/rapidcsv.h) to your project/include directory and include it. 

API Documentation
=================

Document Constructors
---------------------
The Document can be created empty, or based on reading the content of specified file path. It is also possible to control which column and row index should be treated as labels. One can access the complete CSV file by setting pColumnNameIdx and pRowNameIdx to -1 (disabled).

```cpp
    rapidcsv::Properties(const std::string& pPath = "", const int pColumnNameIdx = 0, const int pRowNameIdx = 0, const bool pHasCR = DEFAULT_HASCR);
    explicit rapidcsv::Document(const std::string& pPath = "");
    explicit rapidcsv::Document(const rapidcsv::Properties& pProperties);
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
    size_t GetColumnCount();
    size_t GetRowCount();
```

Get Data
--------
Data can be accessed by column, row or cell, using the following Get methods. These methods produce a copy of the data in specified datatype. The requested position in the spreadsheet is either specified using their label(s), or the zero-based position index.

```cpp
    template<typename T>
    std::vector<T> rapidcsv::Document::GetColumn(const size_t pColumnIdx);
    template<typename T>
    std::vector<T> rapidcsv::Document::GetColumn(const std::string& pColumnName);
    
    template<typename T>
    std::vector<T> rapidcsv::Document::GetRow(const size_t pRowIdx);
    template<typename T>
    std::vector<T> rapidcsv::Document::GetRow(const std::string& pRowName);
    
    template<typename T>
    T rapidcsv::Document::Document::GetCell(const size_t pColumnIdx, const size_t pRowIdx);

    template<typename T>
    T rapidcsv::Document::GetCell(const std::string& pColumnName, const std::string& pRowName);
```

Set Data
--------
Since the Get methods listed above creates a copy of the actual data in the Document class, one needs to use Set methods to modify the data.

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
    void rapidcsv::Document::rapidcsv::Document::RemoveColumn(const size_t pColumnIdx);
    void rapidcsv::Document::rapidcsv::Document::RemoveColumn(const std::string& pColumnName);
    void rapidcsv::Document::rapidcsv::Document::RemoveRow(const size_t pRowIdx);
    void rapidcsv::Document::rapidcsv::Document::RemoveRow(const std::string& pRowName);
    void rapidcsv::Document::SetColumnLabel(size_t pColumnIdx, const std::string& pColumnName);
    void rapidcsv::Document::SetRowLabel(size_t pRowIdx, const std::string& pRowName);
```

Custom Data Conversion
----------------------
The internal cell representation in the Document class is using std::string and when other types are requested, standard conversion routines are used. One may override conversion routines (or add new ones) by implementing ToVal() and ToStr(). Here is an example overriding int conversion, to instead provide two decimal fixed-point numbers. See [tests/test035.cpp](https://github.com/d99kris/rapidcsv/blob/master/tests/test035.cpp) for a complete program example.

```cpp
    namespace rapidcsv
    {
      template<>
      void Converter<int>::ToVal(int& pVal, const std::string& pStr)
      {
        pVal = roundf(100.0 * stof(pStr));
      }
    
      template<>
      void Converter<int>::ToStr(std::string& pStr, const int& pVal)
      {
        std::ostringstream out;
        out << std::fixed << std::setprecision(2) << static_cast<float>(pVal) / 100.0f;
        pStr = out.str();
      }
    }
```

Technical Details
=================
Rapidcsv uses cmake for its tests. Commands to execute the test suite:

    mkdir -p build && cd build && cmake .. && make && ctest --output-on-failure ; cd -

Alternatives
============
There are many CSV parsers for C++, for example:
- [C/C++ CSV Parser](https://sourceforge.net/projects/cccsvparser/)
- [Fast C++ CSV Parser](https://github.com/ben-strasser/fast-cpp-csv-parser)

License
=======
Rapidcsv is distributed under the BSD 3-Clause license. See [LICENSE](https://github.com/d99kris/rapidcsv/blob/master/LICENSE) file.

Keywords
========
c++, c++11, csv parser, comma separated values, single header library.

