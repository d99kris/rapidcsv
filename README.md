Rapidcsv
========

| **Linux + Mac** | **Windows** |
|-----------------|-------------|
| [![Build status](https://travis-ci.org/d99kris/rapidcsv.svg?branch=master)](https://travis-ci.org/d99kris/rapidcsv) | [![Build status](https://ci.appveyor.com/api/projects/status/yyc65as5ln6m6i8l/branch/master?svg=true)](https://ci.appveyor.com/project/d99kris/rapidcsv/branch/master) |

Rapidcsv is a C++ header-only library for CSV parsing. While the name
admittedly was inspired by the rapidjson project, the objectives are not the
same. The goal of rapidcsv is to be an easy-to-use CSV library enabling rapid
development. For optimal performance (be it CPU or memory usage) a CSV parser
implemented for the specific use-case is likely to be more performant.

Example Usage
=============
Here is a simple example reading a CSV file and getting 'Close' column as a
vector of floats, and an example of getting a specific cell as well.

[colrowhdr.csv](examples/colrowhdr.csv) content:
```
    Date,Open,High,Low,Close,Volume,Adj Close
    2017-02-24,64.529999,64.800003,64.139999,64.620003,21705200,64.620003
    2017-02-23,64.419998,64.730003,64.190002,64.620003,20235200,64.620003
    2017-02-22,64.330002,64.389999,64.050003,64.360001,19259700,64.360001
    2017-02-21,64.610001,64.949997,64.449997,64.489998,19384900,64.489998
    2017-02-17,64.470001,64.690002,64.300003,64.620003,21234600,64.620003
```

[ex001.cpp](examples/ex001.cpp) content:
```cpp
    #include <iostream>
    #include <vector>
    #include "rapidcsv.h"

    int main()
    {
      rapidcsv::Document doc("examples/colrowhdr.csv");

      std::vector<float> close = doc.GetColumn<float>("Close");
      std::cout << "Read " << close.size() << " values." << std::endl;

      long long volume = doc.GetCell<long long>("Volume", "2017-02-22");
      std::cout << "Volume " << volume << " on 2017-02-22." << std::endl;
    }
```

Refer to section [More Examples](#more-examples) below for more examples.
The [tests](tests/) directory also contains many simple usage examples.

Supported Platforms
===================
Rapidcsv is implemented using C++11 with the intention of being portable. It's
been tested on:
- macOS Mojave 10.14
- Ubuntu 18.04 LTS
- Windows 7 / Visual Studio 2015

Installation
============
Simply copy
[src/rapidcsv.h](https://raw.githubusercontent.com/d99kris/rapidcsv/master/src/rapidcsv.h)
to your project/include directory and include it. 

More Examples
=============

Several of the following examples are also provided in the `examples/`
directory and can be executed directly under Linux and macOS thanks to a
shebang-hack. Example running ex001.cpp:

```
    ./examples/ex001.cpp
```


Reading a File without Headers
------------------------------
By default rapidcsv treats the first row as column headers, and the first
column as row headers. This allows accessing rows/columns/cells using their
labels, for example `GetCell<float>("Close", "2017-02-22")` to get the cell
from column labelled "Close", at row labelled "2017-02-22". Sometimes one may
prefer to be able to access first row and/or column as data, and only access
cells by their row and column index. In order to do so one need use
LabelParams and set pColumnNameIdx and/or pRowNameIdx to -1 (disabled).

### Column Headers Only
[colhdr.csv](examples/colhdr.csv) content:
```
    Open,High,Low,Close,Volume,Adj Close
    64.529999,64.800003,64.139999,64.620003,21705200,64.620003
    64.419998,64.730003,64.190002,64.620003,20235200,64.620003
    64.330002,64.389999,64.050003,64.360001,19259700,64.360001
    64.610001,64.949997,64.449997,64.489998,19384900,64.489998
    64.470001,64.690002,64.300003,64.620003,21234600,64.620003
```

[ex002.cpp](examples/ex002.cpp) content:
```cpp
    #include <iostream>
    #include <vector>
    #include "rapidcsv.h"

    int main()
    {
      rapidcsv::Document doc("examples/colhdr.csv", rapidcsv::LabelParams(0, -1));

      std::vector<float> col = doc.GetColumn<float>("Close");
      std::cout << "Read " << col.size() << " values." << std::endl;
    }
```

### Row Headers Only
[rowhdr.csv](examples/rowhdr.csv) content:
```
    2017-02-24,64.529999,64.800003,64.139999,64.620003,21705200,64.620003
    2017-02-23,64.419998,64.730003,64.190002,64.620003,20235200,64.620003
    2017-02-22,64.330002,64.389999,64.050003,64.360001,19259700,64.360001
    2017-02-21,64.610001,64.949997,64.449997,64.489998,19384900,64.489998
    2017-02-17,64.470001,64.690002,64.300003,64.620003,21234600,64.620003
```

[ex003.cpp](examples/ex003.cpp) content:
```cpp
    #include <iostream>
    #include <vector>
    #include "rapidcsv.h"

    int main()
    {
      rapidcsv::Document doc("examples/rowhdr.csv", rapidcsv::LabelParams(-1, 0));

      std::vector<std::string> row = doc.GetRow<std::string>("2017-02-22");
      std::cout << "Read " << row.size() << " values." << std::endl;
    }
```

### No Headers
[nohdr.csv](examples/nohdr.csv) content:
```
    64.529999,64.800003,64.139999,64.620003,21705200,64.620003
    64.419998,64.730003,64.190002,64.620003,20235200,64.620003
    64.330002,64.389999,64.050003,64.360001,19259700,64.360001
    64.610001,64.949997,64.449997,64.489998,19384900,64.489998
    64.470001,64.690002,64.300003,64.620003,21234600,64.620003
```

[ex004.cpp](examples/ex004.cpp) content:
```cpp
    #include <iostream>
    #include <vector>
    #include "rapidcsv.h"

    int main()
    {
      rapidcsv::Document doc("examples/nohdr.csv", rapidcsv::LabelParams(-1, -1));

      std::vector<float> close = doc.GetColumn<float>(5);
      std::cout << "Read " << close.size() << " values." << std::endl;

      long long volume = doc.GetCell<long long>(4, 2);
      std::cout << "Volume " << volume << " on 2017-02-22." << std::endl;
    }
```

Reading a File with Custom Separator
------------------------------------
For reading of files with custom separator (i.e. not comma), one need to
specify the SeparatorParams argument. The following example reads a file using
semi-colon as separator.

[semi.csv](examples/semi.csv) content:
```
    Date;Open;High;Low;Close;Volume;Adj Close
    2017-02-24;64.529999;64.800003;64.139999;64.620003;21705200;64.620003
    2017-02-23;64.419998;64.730003;64.190002;64.620003;20235200;64.620003
    2017-02-22;64.330002;64.389999;64.050003;64.360001;19259700;64.360001
    2017-02-21;64.610001;64.949997;64.449997;64.489998;19384900;64.489998
    2017-02-17;64.470001;64.690002;64.300003;64.620003;21234600;64.620003
```

[ex005.cpp](examples/ex005.cpp) content:
```cpp
    #include <iostream>
    #include <vector>
    #include "rapidcsv.h"

    int main()
    {
      rapidcsv::Document doc("examples/semi.csv", rapidcsv::LabelParams(),
                             rapidcsv::SeparatorParams(';'));

      std::vector<float> close = doc.GetColumn<float>("Close");
      std::cout << "Read " << close.size() << " values." << std::endl;

      long long volume = doc.GetCell<long long>("Volume", "2017-02-22");
      std::cout << "Volume " << volume << " on 2017-02-22." << std::endl;
    }
```

Supported Get/Set Datatypes
---------------------------
The internal cell representation in the Document class is using std::string
and when other types are requested, standard conversion routines are used.
All standard conversions are relatively straight-forward, with the
exception of `char` for which rapidcsv interprets the cell's (first) byte
as a character. The following example illustrates the supported datatypes.

[colrowhdr.csv](examples/colrowhdr.csv) content:
```
    Date,Open,High,Low,Close,Volume,Adj Close
    2017-02-24,64.529999,64.800003,64.139999,64.620003,21705200,64.620003
    2017-02-23,64.419998,64.730003,64.190002,64.620003,20235200,64.620003
    2017-02-22,64.330002,64.389999,64.050003,64.360001,19259700,64.360001
    2017-02-21,64.610001,64.949997,64.449997,64.489998,19384900,64.489998
    2017-02-17,64.470001,64.690002,64.300003,64.620003,21234600,64.620003
```

[ex006.cpp](examples/ex006.cpp) content:
```cpp
    #include <iostream>
    #include <vector>
    #include "rapidcsv.h"

    int main()
    {
      rapidcsv::Document doc("examples/colrowhdr.csv");

      std::cout << doc.GetCell<std::string>("Volume", "2017-02-22") << std::endl;
      std::cout << doc.GetCell<int>("Volume", "2017-02-22") << std::endl;
      std::cout << doc.GetCell<long>("Volume", "2017-02-22") << std::endl;
      std::cout << doc.GetCell<long long>("Volume", "2017-02-22") << std::endl;
      std::cout << doc.GetCell<unsigned>("Volume", "2017-02-22") << std::endl;
      std::cout << doc.GetCell<unsigned long>("Volume", "2017-02-22") << std::endl;
      std::cout << doc.GetCell<unsigned long long>("Volume", "2017-02-22") << std::endl;
      std::cout << doc.GetCell<float>("Volume", "2017-02-22") << std::endl;
      std::cout << doc.GetCell<double>("Volume", "2017-02-22") << std::endl;
      std::cout << doc.GetCell<long double>("Volume", "2017-02-22") << std::endl;
      std::cout << doc.GetCell<char>("Volume", "2017-02-22") << std::endl;
    }

```

Custom Data Conversion
----------------------
One may override conversion routines (or add new ones) by implementing ToVal()
and ToStr(). Here is an example overriding int conversion, to instead provide
two decimal fixed-point numbers. See
[tests/test035.cpp](https://github.com/d99kris/rapidcsv/blob/master/tests/test035.cpp)
for a complete program example.

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

Reading CSV Data from a Stream or String
----------------------------------------
In addition to specifying a filename, rapidcsv supports constructing a Document
from a stream and, indirectly through stringstream, from a string. Here is a
simple example reading CSV data from a string:

[ex007.cpp](examples/ex007.cpp) content:
```cpp
    #include <iostream>
    #include <vector>
    #include "rapidcsv.h"

    int main()
    {
      const std::string& csv =
        "Date,Open,High,Low,Close,Volume,Adj Close\n"
        "2017-02-24,64.529999,64.800003,64.139999,64.620003,21705200,64.620003\n"
        "2017-02-23,64.419998,64.730003,64.190002,64.620003,20235200,64.620003\n"
        "2017-02-22,64.330002,64.389999,64.050003,64.360001,19259700,64.360001\n"
        "2017-02-21,64.610001,64.949997,64.449997,64.489998,19384900,64.489998\n"
        "2017-02-17,64.470001,64.690002,64.300003,64.620003,21234600,64.620003\n"
        ;

      std::stringstream sstream(csv);
      rapidcsv::Document doc(sstream);

      std::vector<float> close = doc.GetColumn<float>("Close");
      std::cout << "Read " << close.size() << " values." << std::endl;

      long long volume = doc.GetCell<long long>("Volume", "2017-02-22");
      std::cout << "Volume " << volume << " on 2017-02-22." << std::endl;
    }
```

Reading a File with Invalid Numbers (e.g. Empty Cells) as Numeric Data
-----------------------------------------------------------------------
By default rapidcsv throws an exception if one tries to access non-numeric
data as a numeric datatype, as it basically propagates the underlying
conversion routines' exceptions to the calling application.

The reason for this is to ensure data correctness. If one wants to be able
to read data with invalid numbers as numeric datatypes, one can use
ConverterParams to configure the converter to default to a numeric value.
The value is configurable and by default it's
std::numeric_limits<long double>::signaling_NaN() for float types, and 0 for
integer types. Example:

```cpp
    rapidcsv::Document doc("file.csv", rapidcsv::LabelParams(),
                           rapidcsv::SeparatorParams(),
                           rapidcsv::ConverterParams(true));
```

Check if a Column Exists
------------------------
Rapidcsv provides the methods GetColumnNames() and GetRowNames() to retrieve
the column and row names. To check whether a particular column name exists
one can for example do:

```cpp
    rapidcsv::Document doc("file.csv");
    std::vector<std::string> columnNames = doc.GetColumnNames();
    bool column_A_exists =
      (std::find(columnNames.begin(), columnNames.end(), "A") != columnNames.end());
```

UTF-16 and UTF-8
----------------
Rapidcsv's preferred encoding for non-ASCII text is UTF-8. UTF-16 LE and
UTF-16 BE can be read and written by rapidcsv on systems where codecvt header
is present. Define HAS_CODECVT before including rapidcsv.h in order to enable
the functionality. Rapidcsv unit tests automatically detects the presence of
codecvt and sets HAS_CODECVT as needed, see [CMakeLists.txt](CMakeLists.txt)
for reference. When enabled, the UTF-16 encoding of any loaded file is
automatically detected.

API Documentation
=================
The following classes makes up the Rapidcsv interface:
 - [class rapidcsv::Document](doc/rapidcsv_Document.md)
 - [class rapidcsv::SeparatorParams](doc/rapidcsv_SeparatorParams.md)
 - [class rapidcsv::LabelParams](doc/rapidcsv_LabelParams.md)
 - [class rapidcsv::ConverterParams](doc/rapidcsv_ConverterParams.md)
 - [class rapidcsv::no_converter](doc/rapidcsv_no_converter.md)
 - [class rapidcsv::Converter< T >](doc/rapidcsv_Converter.md)

Technical Details
=================
Rapidcsv uses cmake for its tests. Commands to build and execute the test suite:

    mkdir -p build && cd build && cmake .. && make && ctest -C unit --output-on-failure && ctest -C perf --verbose ; cd -

Rapidcsv uses [doxyman2md](https://github.com/d99kris/doxyman2md) to generate
its API documentation:

    doxyman2md src doc

Rapidcsv uses Uncrustify to ensure consistent code formatting:

    uncrustify -c uncrustify.cfg --no-backup src/rapidcsv.h

Alternatives
============
There are many CSV parsers for C++, for example:
- [CSV Parser](https://github.com/AriaFallah/csv-parser)
- [CSVparser](https://github.com/MyBoon/CSVparser)
- [Fast C++ CSV Parser](https://github.com/ben-strasser/fast-cpp-csv-parser)
- [Vince's CSV Parser](https://github.com/vincentlaucsb/csv-parser)

License
=======
Rapidcsv is distributed under the BSD 3-Clause license. See
[LICENSE](https://github.com/d99kris/rapidcsv/blob/master/LICENSE) file.

Contributions
=============
Bugs, PRs, etc are welcome on the GitHub project page
https://github.com/d99kris/rapidcsv

Keywords
========
c++, c++11, csv parser, comma separated values, single header library.

