/*
 * rapidcsv.h
 *
 * URL:      https://github.com/d99kris/rapidcsv
 * Version:  8.50
 *
 * Copyright (C) 2017-2021 Kristofer Berggren
 * All rights reserved.
 *
 * rapidcsv is distributed under the BSD 3-Clause license, see LICENSE for details.
 *
 */

#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#ifdef HAS_CODECVT
#include <codecvt>
#endif
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <typeinfo>
#include <vector>

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

namespace rapidcsv
{
#if defined(_MSC_VER)
  static const bool sPlatformHasCR = true;
#else
  static const bool sPlatformHasCR = false;
#endif

  /**
   * @brief     Datastructure holding parameters controlling how invalid numbers (including
   *            empty strings) should be handled.
   */
  struct ConverterParams
  {
    /**
     * @brief   Constructor
     * @param   pHasDefaultConverter  specifies if conversion of non-numerical strings shall be
     *                                converted to a default numerical value, instead of causing
     *                                an exception to be thrown (default).
     * @param   pDefaultFloat         floating-point default value to represent invalid numbers.
     * @param   pDefaultInteger       integer default value to represent invalid numbers.
     */
    explicit ConverterParams(const bool pHasDefaultConverter = false,
                             const long double pDefaultFloat = std::numeric_limits<long double>::signaling_NaN(),
                             const long long pDefaultInteger = 0)
      : mHasDefaultConverter(pHasDefaultConverter)
      , mDefaultFloat(pDefaultFloat)
      , mDefaultInteger(pDefaultInteger)
    {
    }

    /**
     * @brief   specifies if conversion of non-numerical strings shall be converted to a default
     *          numerical value, instead of causing an exception to be thrown (default).
     */
    bool mHasDefaultConverter;

    /**
     * @brief   floating-point default value to represent invalid numbers.
     */
    long double mDefaultFloat;

    /**
     * @brief   integer default value to represent invalid numbers.
     */
    long long mDefaultInteger;
  };

  /**
   * @brief     Exception thrown when attempting to access Document data in a datatype which
   *            is not supported by the Converter class.
   */
  class no_converter : public std::exception
  {
    /**
     * @brief   Provides details about the exception
     * @returns an explanatory string
     */
    virtual const char* what() const throw()
    {
      return "unsupported conversion datatype";
    }
  };

  /**
   * @brief     Class providing conversion to/from numerical datatypes and strings. Only
   *            intended for rapidcsv internal usage, but exposed externally to allow
   *            specialization for custom datatype conversions.
   */
  template<typename T>
  class Converter
  {
  public:
    /**
     * @brief   Constructor
     * @param   pConverterParams      specifies how conversion of non-numerical values to
     *                                numerical datatype shall be handled.
     */
    Converter(const ConverterParams& pConverterParams)
      : mConverterParams(pConverterParams)
    {
    }

    /**
     * @brief   Converts numerical value to string representation.
     * @param   pVal                  numerical value
     * @param   pStr                  output string
     */
    void ToStr(const T& pVal, std::string& pStr) const
    {
      if (typeid(T) == typeid(int) ||
          typeid(T) == typeid(long) ||
          typeid(T) == typeid(long long) ||
          typeid(T) == typeid(unsigned) ||
          typeid(T) == typeid(unsigned long) ||
          typeid(T) == typeid(unsigned long long) ||
          typeid(T) == typeid(float) ||
          typeid(T) == typeid(double) ||
          typeid(T) == typeid(long double) ||
          typeid(T) == typeid(char))
      {
        std::ostringstream out;
        out << pVal;
        pStr = out.str();
      }
      else
      {
        throw no_converter();
      }
    }

    /**
     * @brief   Converts string holding a numerical value to numerical datatype representation.
     * @param   pVal                  numerical value
     * @param   pStr                  output string
     */
    void ToVal(const std::string& pStr, T& pVal) const
    {
      try
      {
        if (typeid(T) == typeid(int))
        {
          pVal = static_cast<T>(std::stoi(pStr));
          return;
        }
        else if (typeid(T) == typeid(long))
        {
          pVal = static_cast<T>(std::stol(pStr));
          return;
        }
        else if (typeid(T) == typeid(long long))
        {
          pVal = static_cast<T>(std::stoll(pStr));
          return;
        }
        else if (typeid(T) == typeid(unsigned))
        {
          pVal = static_cast<T>(std::stoul(pStr));
          return;
        }
        else if (typeid(T) == typeid(unsigned long))
        {
          pVal = static_cast<T>(std::stoul(pStr));
          return;
        }
        else if (typeid(T) == typeid(unsigned long long))
        {
          pVal = static_cast<T>(std::stoull(pStr));
          return;
        }
      }
      catch (...)
      {
        if (!mConverterParams.mHasDefaultConverter)
        {
          throw;
        }
        else
        {
          pVal = static_cast<T>(mConverterParams.mDefaultInteger);
          return;
        }
      }

      try
      {
        if (typeid(T) == typeid(float))
        {
          pVal = static_cast<T>(std::stof(pStr));
          return;
        }
        else if (typeid(T) == typeid(double))
        {
          pVal = static_cast<T>(std::stod(pStr));
          return;
        }
        else if (typeid(T) == typeid(long double))
        {
          pVal = static_cast<T>(std::stold(pStr));
          return;
        }
      }
      catch (...)
      {
        if (!mConverterParams.mHasDefaultConverter)
        {
          throw;
        }
        else
        {
          pVal = static_cast<T>(mConverterParams.mDefaultFloat);
          return;
        }
      }

      if (typeid(T) == typeid(char))
      {
        pVal = static_cast<T>(pStr[0]);
        return;
      }
      else
      {
        throw no_converter();
      }
    }

  private:
    const ConverterParams& mConverterParams;
  };

  /**
   * @brief     Specialized implementation handling string to string conversion.
   * @param     pVal                  string
   * @param     pStr                  string
   */
  template<>
  inline void Converter<std::string>::ToStr(const std::string& pVal, std::string& pStr) const
  {
    pStr = pVal;
  }

  /**
   * @brief     Specialized implementation handling string to string conversion.
   * @param     pVal                  string
   * @param     pStr                  string
   */
  template<>
  inline void Converter<std::string>::ToVal(const std::string& pStr, std::string& pVal) const
  {
    pVal = pStr;
  }

  template<typename T>
  using ConvFunc = std::function<void (const std::string & pStr, T & pVal)>;

  /**
   * @brief     Datastructure holding parameters controlling which row and column should be
   *            treated as labels.
   */
  struct LabelParams
  {
    /**
     * @brief   Constructor
     * @param   pColumnNameIdx        specifies the zero-based row index of the column labels, setting
     *                                it to -1 prevents column lookup by label name, and gives access
     *                                to all rows as document data. Default: 0
     * @param   pRowNameIdx           specifies the zero-based column index of the row labels, setting
     *                                it to -1 prevents row lookup by label name, and gives access
     *                                to all columns as document data. Default: -1
     */
    explicit LabelParams(const int pColumnNameIdx = 0, const int pRowNameIdx = -1)
      : mColumnNameIdx(pColumnNameIdx)
      , mRowNameIdx(pRowNameIdx)
    {
    }

    /**
     * @brief   specifies the zero-based row index of the column labels.
     */
    int mColumnNameIdx;

    /**
     * @brief   specifies the zero-based column index of the row labels.
     */
    int mRowNameIdx;
  };

  /**
   * @brief     Datastructure holding parameters controlling how the CSV data fields are separated.
   */
  struct SeparatorParams
  {
    /**
     * @brief   Constructor
     * @param   pSeparator            specifies the column separator (default ',').
     * @param   pTrim                 specifies whether to trim leading and trailing spaces from
     *                                cells read (default false).
     * @param   pHasCR                specifies whether a new document (i.e. not an existing document read)
     *                                should use CR/LF instead of only LF (default is to use standard
     *                                behavior of underlying platforms - CR/LF for Win, and LF for others).
     * @param   pQuotedLinebreaks     specifies whether to allow line breaks in quoted text (default false)
     * @param   pAutoQuote            specifies whether to automatically dequote data during read, and add
     *                                quotes during write (default true).
     */
    explicit SeparatorParams(const char pSeparator = ',', const bool pTrim = false,
                             const bool pHasCR = sPlatformHasCR, const bool pQuotedLinebreaks = false,
                             const bool pAutoQuote = true)
      : mSeparator(pSeparator)
      , mTrim(pTrim)
      , mHasCR(pHasCR)
      , mQuotedLinebreaks(pQuotedLinebreaks)
      , mAutoQuote(pAutoQuote)
    {
    }

    /**
     * @brief   specifies the column separator.
     */
    char mSeparator;

    /**
     * @brief   specifies whether to trim leading and trailing spaces from cells read.
     */
    bool mTrim;

    /**
     * @brief   specifies whether new documents should use CR/LF instead of LF.
     */
    bool mHasCR;

    /**
     * @brief   specifies whether to allow line breaks in quoted text.
     */
    bool mQuotedLinebreaks;

    /**
     * @brief   specifies whether to automatically dequote cell data.
     */
    bool mAutoQuote;
  };

  /**
   * @brief     Class representing a CSV document.
   */
  class Document
  {
  public:
    /**
     * @brief   Constructor
     * @param   pPath                 specifies the path of an existing CSV-file to populate the Document
     *                                data with.
     * @param   pLabelParams          specifies which row and column should be treated as labels.
     * @param   pSeparatorParams      specifies which field and row separators should be used.
     * @param   pConverterParams      specifies how invalid numbers (including empty strings) should be
     *                                handled.
     */
    explicit Document(const std::string& pPath = std::string(),
                      const LabelParams& pLabelParams = LabelParams(),
                      const SeparatorParams& pSeparatorParams = SeparatorParams(),
                      const ConverterParams& pConverterParams = ConverterParams())
      : mPath(pPath)
      , mLabelParams(pLabelParams)
      , mSeparatorParams(pSeparatorParams)
      , mConverterParams(pConverterParams)
    {
      if (!mPath.empty())
      {
        ReadCsv();
      }
    }

    /**
     * @brief   Constructor
     * @param   pStream               specifies an input stream to read CSV data from.
     * @param   pLabelParams          specifies which row and column should be treated as labels.
     * @param   pSeparatorParams      specifies which field and row separators should be used.
     * @param   pConverterParams      specifies how invalid numbers (including empty strings) should be
     *                                handled.
     */
    explicit Document(std::istream& pStream,
                      const LabelParams& pLabelParams = LabelParams(),
                      const SeparatorParams& pSeparatorParams = SeparatorParams(),
                      const ConverterParams& pConverterParams = ConverterParams())
      : mPath()
      , mLabelParams(pLabelParams)
      , mSeparatorParams(pSeparatorParams)
      , mConverterParams(pConverterParams)
    {
      ReadCsv(pStream);
    }

    /**
     * @brief   Read Document data from file.
     * @param   pPath                 specifies the path of an existing CSV-file to populate the Document
     *                                data with.
     * @param   pLabelParams          specifies which row and column should be treated as labels.
     * @param   pSeparatorParams      specifies which field and row separators should be used.
     * @param   pConverterParams      specifies how invalid numbers (including empty strings) should be
     *                                handled.
     */
    void Load(const std::string& pPath,
              const LabelParams& pLabelParams = LabelParams(),
              const SeparatorParams& pSeparatorParams = SeparatorParams(),
              const ConverterParams& pConverterParams = ConverterParams())
    {
      mPath = pPath;
      mLabelParams = pLabelParams;
      mSeparatorParams = pSeparatorParams;
      mConverterParams = pConverterParams;
      ReadCsv();
    }

    /**
     * @brief   Read Document data from stream.
     * @param   pStream               specifies an input stream to read CSV data from.
     * @param   pLabelParams          specifies which row and column should be treated as labels.
     * @param   pSeparatorParams      specifies which field and row separators should be used.
     * @param   pConverterParams      specifies how invalid numbers (including empty strings) should be
     *                                handled.
     */
    void Load(std::istream& pStream,
              const LabelParams& pLabelParams = LabelParams(),
              const SeparatorParams& pSeparatorParams = SeparatorParams(),
              const ConverterParams& pConverterParams = ConverterParams())
    {
      mPath = "";
      mLabelParams = pLabelParams;
      mSeparatorParams = pSeparatorParams;
      mConverterParams = pConverterParams;
      ReadCsv(pStream);
    }

    /**
     * @brief   Write Document data to file.
     * @param   pPath                 optionally specifies the path where the CSV-file will be created
     *                                (if not specified, the original path provided when creating or
     *                                loading the Document data will be used).
     */
    void Save(const std::string& pPath = std::string())
    {
      if (!pPath.empty())
      {
        mPath = pPath;
      }
      WriteCsv();
    }

    /**
     * @brief   Write Document data to stream.
     * @param   pStream               specifies an output stream to write the data to.
     */
    void Save(std::ostream& pStream)
    {
      WriteCsv(pStream);
    }

    /**
     * @brief   Clears loaded Document data.
     *
     */
    void Clear()
    {
      mData.clear();
      mColumnNames.clear();
      mRowNames.clear();
#ifdef HAS_CODECVT
      mIsUtf16 = false;
      mIsLE = false;
#endif
    }

    /**
     * @brief   Get column index by name.
     * @param   pColumnName           column label name.
     * @returns zero-based column index.
     */
    ssize_t GetColumnIdx(const std::string& pColumnName) const
    {
      if (mLabelParams.mColumnNameIdx >= 0)
      {
        if (mColumnNames.find(pColumnName) != mColumnNames.end())
        {
          return mColumnNames.at(pColumnName) - (mLabelParams.mRowNameIdx + 1);
        }
      }
      return -1;
    }

    /**
     * @brief   Get column by index.
     * @param   pColumnIdx            zero-based column index.
     * @returns vector of column data.
     */
    template<typename T>
    std::vector<T> GetColumn(const size_t pColumnIdx) const
    {
      const ssize_t columnIdx = pColumnIdx + (mLabelParams.mRowNameIdx + 1);
      std::vector<T> column;
      Converter<T> converter(mConverterParams);
      for (auto itRow = mData.begin(); itRow != mData.end(); ++itRow)
      {
        if (std::distance(mData.begin(), itRow) > mLabelParams.mColumnNameIdx)
        {
          T val;
          converter.ToVal(itRow->at(columnIdx), val);
          column.push_back(val);
        }
      }
      return column;
    }

    /**
     * @brief   Get column by index.
     * @param   pColumnIdx            zero-based column index.
     * @param   pToVal                conversion function.
     * @returns vector of column data.
     */
    template<typename T>
    std::vector<T> GetColumn(const size_t pColumnIdx, ConvFunc<T> pToVal) const
    {
      const ssize_t columnIdx = pColumnIdx + (mLabelParams.mRowNameIdx + 1);
      std::vector<T> column;
      for (auto itRow = mData.begin(); itRow != mData.end(); ++itRow)
      {
        if (std::distance(mData.begin(), itRow) > mLabelParams.mColumnNameIdx)
        {
          T val;
          pToVal(itRow->at(columnIdx), val);
          column.push_back(val);
        }
      }
      return column;
    }

    /**
     * @brief   Get column by name.
     * @param   pColumnName           column label name.
     * @returns vector of column data.
     */
    template<typename T>
    std::vector<T> GetColumn(const std::string& pColumnName) const
    {
      const ssize_t columnIdx = GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }
      return GetColumn<T>(columnIdx);
    }

    /**
     * @brief   Get column by name.
     * @param   pColumnName           column label name.
     * @param   pToVal                conversion function.
     * @returns vector of column data.
     */
    template<typename T>
    std::vector<T> GetColumn(const std::string& pColumnName, ConvFunc<T> pToVal) const
    {
      const ssize_t columnIdx = GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }
      return GetColumn<T>(columnIdx, pToVal);
    }

    /**
     * @brief   Set column by index.
     * @param   pColumnIdx            zero-based column index.
     * @param   pColumn               vector of column data.
     */
    template<typename T>
    void SetColumn(const size_t pColumnIdx, const std::vector<T>& pColumn)
    {
      const size_t columnIdx = pColumnIdx + (mLabelParams.mRowNameIdx + 1);

      while (pColumn.size() + (mLabelParams.mColumnNameIdx + 1) > GetDataRowCount())
      {
        std::vector<std::string> row;
        row.resize(GetDataColumnCount());
        mData.push_back(row);
      }

      if ((columnIdx + 1) > GetDataColumnCount())
      {
        for (auto itRow = mData.begin(); itRow != mData.end(); ++itRow)
        {
          itRow->resize(columnIdx + 1 + (mLabelParams.mRowNameIdx + 1));
        }
      }

      Converter<T> converter(mConverterParams);
      for (auto itRow = pColumn.begin(); itRow != pColumn.end(); ++itRow)
      {
        std::string str;
        converter.ToStr(*itRow, str);
        mData.at(std::distance(pColumn.begin(), itRow) + (mLabelParams.mColumnNameIdx + 1)).at(columnIdx) = str;
      }
    }

    /**
     * @brief   Set column by name.
     * @param   pColumnName           column label name.
     * @param   pColumn               vector of column data.
     */
    template<typename T>
    void SetColumn(const std::string& pColumnName, const std::vector<T>& pColumn)
    {
      const ssize_t columnIdx = GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }
      SetColumn<T>(columnIdx, pColumn);
    }

    /**
     * @brief   Remove column by index.
     * @param   pColumnIdx            zero-based column index.
     */
    void RemoveColumn(const size_t pColumnIdx)
    {
      const ssize_t columnIdx = pColumnIdx + (mLabelParams.mRowNameIdx + 1);
      for (auto itRow = mData.begin(); itRow != mData.end(); ++itRow)
      {
        itRow->erase(itRow->begin() + columnIdx);
      }
    }

    /**
     * @brief   Remove column by name.
     * @param   pColumnName           column label name.
     */
    void RemoveColumn(const std::string& pColumnName)
    {
      ssize_t columnIdx = GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }

      RemoveColumn(columnIdx);
    }

    /**
     * @brief   Insert column at specified index.
     * @param   pColumnIdx            zero-based column index.
     * @param   pColumn               vector of column data (optional argument).
     * @param   pColumnName           column label name (optional argument).
     */
    template<typename T>
    void InsertColumn(const size_t pColumnIdx, const std::vector<T>& pColumn = std::vector<T>(),
                      const std::string& pColumnName = std::string())
    {
      const size_t columnIdx = pColumnIdx + (mLabelParams.mRowNameIdx + 1);

      std::vector<std::string> column;
      if (pColumn.empty())
      {
        column.resize(GetDataRowCount());
      }
      else
      {
        column.resize(pColumn.size() + (mLabelParams.mColumnNameIdx + 1));
        Converter<T> converter(mConverterParams);
        for (auto itRow = pColumn.begin(); itRow != pColumn.end(); ++itRow)
        {
          std::string str;
          converter.ToStr(*itRow, str);
          const size_t rowIdx = std::distance(pColumn.begin(), itRow) + (mLabelParams.mColumnNameIdx + 1);
          column.at(rowIdx) = str;
        }
      }

      while (column.size() > GetDataRowCount())
      {
        std::vector<std::string> row;
        const size_t columnCount = std::max(static_cast<size_t>(mLabelParams.mColumnNameIdx + 1),
                                            GetDataColumnCount());
        row.resize(columnCount);
        mData.push_back(row);
      }

      for (auto itRow = mData.begin(); itRow != mData.end(); ++itRow)
      {
        const size_t rowIdx = std::distance(mData.begin(), itRow);
        itRow->insert(itRow->begin() + columnIdx, column.at(rowIdx));
      }

      if (!pColumnName.empty())
      {
        SetColumnName(pColumnIdx, pColumnName);
      }
    }

    /**
     * @brief   Get number of data columns (excluding label columns).
     * @returns column count.
     */
    size_t GetColumnCount() const
    {
      const ssize_t count = static_cast<ssize_t>((mData.size() > 0) ? mData.at(0).size() : 0) -
        (mLabelParams.mRowNameIdx + 1);
      return (count >= 0) ? count : 0;
    }

    /**
     * @brief   Get row index by name.
     * @param   pRowName              row label name.
     * @returns zero-based row index.
     */
    ssize_t GetRowIdx(const std::string& pRowName) const
    {
      if (mLabelParams.mRowNameIdx >= 0)
      {
        if (mRowNames.find(pRowName) != mRowNames.end())
        {
          return mRowNames.at(pRowName) - (mLabelParams.mColumnNameIdx + 1);
        }
      }
      return -1;
    }

    /**
     * @brief   Get row by index.
     * @param   pRowIdx               zero-based row index.
     * @returns vector of row data.
     */
    template<typename T>
    std::vector<T> GetRow(const size_t pRowIdx) const
    {
      const ssize_t rowIdx = pRowIdx + (mLabelParams.mColumnNameIdx + 1);
      std::vector<T> row;
      Converter<T> converter(mConverterParams);
      for (auto itCol = mData.at(rowIdx).begin(); itCol != mData.at(rowIdx).end(); ++itCol)
      {
        if (std::distance(mData.at(rowIdx).begin(), itCol) > mLabelParams.mRowNameIdx)
        {
          T val;
          converter.ToVal(*itCol, val);
          row.push_back(val);
        }
      }
      return row;
    }

    /**
     * @brief   Get row by index.
     * @param   pRowIdx               zero-based row index.
     * @param   pToVal                conversion function.
     * @returns vector of row data.
     */
    template<typename T>
    std::vector<T> GetRow(const size_t pRowIdx, ConvFunc<T> pToVal) const
    {
      const ssize_t rowIdx = pRowIdx + (mLabelParams.mColumnNameIdx + 1);
      std::vector<T> row;
      Converter<T> converter(mConverterParams);
      for (auto itCol = mData.at(rowIdx).begin(); itCol != mData.at(rowIdx).end(); ++itCol)
      {
        if (std::distance(mData.at(rowIdx).begin(), itCol) > mLabelParams.mRowNameIdx)
        {
          T val;
          pToVal(*itCol, val);
          row.push_back(val);
        }
      }
      return row;
    }

    /**
     * @brief   Get row by name.
     * @param   pRowName              row label name.
     * @returns vector of row data.
     */
    template<typename T>
    std::vector<T> GetRow(const std::string& pRowName) const
    {
      ssize_t rowIdx = GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }
      return GetRow<T>(rowIdx);
    }

    /**
     * @brief   Get row by name.
     * @param   pRowName              row label name.
     * @param   pToVal                conversion function.
     * @returns vector of row data.
     */
    template<typename T>
    std::vector<T> GetRow(const std::string& pRowName, ConvFunc<T> pToVal) const
    {
      ssize_t rowIdx = GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }
      return GetRow<T>(rowIdx, pToVal);
    }

    /**
     * @brief   Set row by index.
     * @param   pRowIdx               zero-based row index.
     * @param   pRow                  vector of row data.
     */
    template<typename T>
    void SetRow(const size_t pRowIdx, const std::vector<T>& pRow)
    {
      const size_t rowIdx = pRowIdx + (mLabelParams.mColumnNameIdx + 1);

      while ((rowIdx + 1) > GetDataRowCount())
      {
        std::vector<std::string> row;
        row.resize(GetDataColumnCount());
        mData.push_back(row);
      }

      if (pRow.size() > GetDataColumnCount())
      {
        for (auto itRow = mData.begin(); itRow != mData.end(); ++itRow)
        {
          itRow->resize(pRow.size() + (mLabelParams.mRowNameIdx + 1));
        }
      }

      Converter<T> converter(mConverterParams);
      for (auto itCol = pRow.begin(); itCol != pRow.end(); ++itCol)
      {
        std::string str;
        converter.ToStr(*itCol, str);
        mData.at(rowIdx).at(std::distance(pRow.begin(), itCol) + (mLabelParams.mRowNameIdx + 1)) = str;
      }
    }

    /**
     * @brief   Set row by name.
     * @param   pRowName              row label name.
     * @param   pRow                  vector of row data.
     */
    template<typename T>
    void SetRow(const std::string& pRowName, const std::vector<T>& pRow)
    {
      ssize_t rowIdx = GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }
      return SetRow<T>(rowIdx, pRow);
    }

    /**
     * @brief   Remove row by index.
     * @param   pRowIdx               zero-based row index.
     */
    void RemoveRow(const size_t pRowIdx)
    {
      const ssize_t rowIdx = pRowIdx + (mLabelParams.mColumnNameIdx + 1);
      mData.erase(mData.begin() + rowIdx);
    }

    /**
     * @brief   Remove row by name.
     * @param   pRowName              row label name.
     */
    void RemoveRow(const std::string& pRowName)
    {
      ssize_t rowIdx = GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }

      RemoveRow(rowIdx);
    }

    /**
     * @brief   Insert row at specified index.
     * @param   pRowIdx               zero-based row index.
     * @param   pRow                  vector of row data (optional argument).
     * @param   pRowName              row label name (optional argument).
     */
    template<typename T>
    void InsertRow(const size_t pRowIdx, const std::vector<T>& pRow = std::vector<T>(),
                   const std::string& pRowName = std::string())
    {
      const size_t rowIdx = pRowIdx + (mLabelParams.mColumnNameIdx + 1);

      std::vector<std::string> row;
      if (pRow.empty())
      {
        row.resize(GetDataColumnCount());
      }
      else
      {
        row.resize(pRow.size() + (mLabelParams.mRowNameIdx + 1));
        Converter<T> converter(mConverterParams);
        for (auto itCol = pRow.begin(); itCol != pRow.end(); ++itCol)
        {
          std::string str;
          converter.ToStr(*itCol, str);
          row.at(std::distance(pRow.begin(), itCol) + (mLabelParams.mRowNameIdx + 1)) = str;
        }
      }

      while (rowIdx > GetDataRowCount())
      {
        std::vector<std::string> tempRow;
        tempRow.resize(GetDataColumnCount());
        mData.push_back(tempRow);
      }

      mData.insert(mData.begin() + rowIdx, row);

      if (!pRowName.empty())
      {
        SetRowName(pRowIdx, pRowName);
      }
    }

    /**
     * @brief   Get number of data rows (excluding label rows).
     * @returns row count.
     */
    size_t GetRowCount() const
    {
      const ssize_t count = static_cast<ssize_t>(mData.size()) - (mLabelParams.mColumnNameIdx + 1);
      return (count >= 0) ? count : 0;
    }

    /**
     * @brief   Get cell by index.
     * @param   pColumnIdx            zero-based column index.
     * @param   pRowIdx               zero-based row index.
     * @returns cell data.
     */
    template<typename T>
    T GetCell(const size_t pColumnIdx, const size_t pRowIdx) const
    {
      const ssize_t columnIdx = pColumnIdx + (mLabelParams.mRowNameIdx + 1);
      const ssize_t rowIdx = pRowIdx + (mLabelParams.mColumnNameIdx + 1);

      T val;
      Converter<T> converter(mConverterParams);
      converter.ToVal(mData.at(rowIdx).at(columnIdx), val);
      return val;
    }

    /**
     * @brief   Get cell by index.
     * @param   pColumnIdx            zero-based column index.
     * @param   pRowIdx               zero-based row index.
     * @param   pToVal                conversion function.
     * @returns cell data.
     */
    template<typename T>
    T GetCell(const size_t pColumnIdx, const size_t pRowIdx, ConvFunc<T> pToVal) const
    {
      const ssize_t columnIdx = pColumnIdx + (mLabelParams.mRowNameIdx + 1);
      const ssize_t rowIdx = pRowIdx + (mLabelParams.mColumnNameIdx + 1);

      T val;
      pToVal(mData.at(rowIdx).at(columnIdx), val);
      return val;
    }

    /**
     * @brief   Get cell by name.
     * @param   pColumnName           column label name.
     * @param   pRowName              row label name.
     * @returns cell data.
     */
    template<typename T>
    T GetCell(const std::string& pColumnName, const std::string& pRowName) const
    {
      const ssize_t columnIdx = GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }

      const ssize_t rowIdx = GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }

      return GetCell<T>(columnIdx, rowIdx);
    }

    /**
     * @brief   Get cell by name.
     * @param   pColumnName           column label name.
     * @param   pRowName              row label name.
     * @param   pToVal                conversion function.
     * @returns cell data.
     */
    template<typename T>
    T GetCell(const std::string& pColumnName, const std::string& pRowName, ConvFunc<T> pToVal) const
    {
      const ssize_t columnIdx = GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }

      const ssize_t rowIdx = GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }

      return GetCell<T>(columnIdx, rowIdx, pToVal);
    }

    /**
     * @brief   Get cell by column name and row index.
     * @param   pColumnName           column label name.
     * @param   pRowIdx               zero-based row index.
     * @returns cell data.
     */
    template<typename T>
    T GetCell(const std::string& pColumnName, const size_t pRowIdx) const
    {
      const ssize_t columnIdx = GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }

      return GetCell<T>(columnIdx, pRowIdx);
    }

    /**
     * @brief   Get cell by column name and row index.
     * @param   pColumnName           column label name.
     * @param   pRowIdx               zero-based row index.
     * @param   pToVal                conversion function.
     * @returns cell data.
     */
    template<typename T>
    T GetCell(const std::string& pColumnName, const size_t pRowIdx, ConvFunc<T> pToVal) const
    {
      const ssize_t columnIdx = GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }

      return GetCell<T>(columnIdx, pRowIdx, pToVal);
    }

    /**
     * @brief   Get cell by column index and row name.
     * @param   pColumnIdx            zero-based column index.
     * @param   pRowName              row label name.
     * @returns cell data.
     */
    template<typename T>
    T GetCell(const size_t pColumnIdx, const std::string& pRowName) const
    {
      const ssize_t rowIdx = GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }

      return GetCell<T>(pColumnIdx, rowIdx);
    }

    /**
     * @brief   Get cell by column index and row name.
     * @param   pColumnIdx            zero-based column index.
     * @param   pRowName              row label name.
     * @param   pToVal                conversion function.
     * @returns cell data.
     */
    template<typename T>
    T GetCell(const size_t pColumnIdx, const std::string& pRowName, ConvFunc<T> pToVal) const
    {
      const ssize_t rowIdx = GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }

      return GetCell<T>(pColumnIdx, rowIdx, pToVal);
    }

    /**
     * @brief   Set cell by index.
     * @param   pRowIdx               zero-based row index.
     * @param   pColumnIdx            zero-based column index.
     * @param   pCell                 cell data.
     */
    template<typename T>
    void SetCell(const size_t pColumnIdx, const size_t pRowIdx, const T& pCell)
    {
      const size_t columnIdx = pColumnIdx + (mLabelParams.mRowNameIdx + 1);
      const size_t rowIdx = pRowIdx + (mLabelParams.mColumnNameIdx + 1);

      while ((rowIdx + 1) > GetDataRowCount())
      {
        std::vector<std::string> row;
        row.resize(GetDataColumnCount());
        mData.push_back(row);
      }

      if ((columnIdx + 1) > GetDataColumnCount())
      {
        for (auto itRow = mData.begin(); itRow != mData.end(); ++itRow)
        {
          itRow->resize(columnIdx + 1);
        }
      }

      std::string str;
      Converter<T> converter(mConverterParams);
      converter.ToStr(pCell, str);
      mData.at(rowIdx).at(columnIdx) = str;
    }

    /**
     * @brief   Set cell by name.
     * @param   pColumnName           column label name.
     * @param   pRowName              row label name.
     * @param   pCell                 cell data.
     */
    template<typename T>
    void SetCell(const std::string& pColumnName, const std::string& pRowName, const T& pCell)
    {
      const ssize_t columnIdx = GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }

      const ssize_t rowIdx = GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }

      SetCell<T>(columnIdx, rowIdx, pCell);
    }

    /**
     * @brief   Get column name
     * @param   pColumnIdx            zero-based column index.
     * @returns column name.
     */
    std::string GetColumnName(const ssize_t pColumnIdx)
    {
      const ssize_t columnIdx = pColumnIdx + (mLabelParams.mRowNameIdx + 1);
      if (mLabelParams.mColumnNameIdx < 0)
      {
        throw std::out_of_range("column name row index < 0: " + std::to_string(mLabelParams.mColumnNameIdx));
      }

      return mData.at(mLabelParams.mColumnNameIdx).at(columnIdx);
    }

    /**
     * @brief   Set column name
     * @param   pColumnIdx            zero-based column index.
     * @param   pColumnName           column name.
     */
    void SetColumnName(size_t pColumnIdx, const std::string& pColumnName)
    {
      const ssize_t columnIdx = pColumnIdx + (mLabelParams.mRowNameIdx + 1);
      mColumnNames[pColumnName] = columnIdx;
      if (mLabelParams.mColumnNameIdx < 0)
      {
        throw std::out_of_range("column name row index < 0: " + std::to_string(mLabelParams.mColumnNameIdx));
      }

      // increase table size if necessary:
      const int rowIdx = mLabelParams.mColumnNameIdx;
      if (rowIdx >= static_cast<int>(mData.size()))
      {
        mData.resize(rowIdx + 1);
      }
      auto& row = mData[rowIdx];
      if (columnIdx >= static_cast<int>(row.size()))
      {
        row.resize(columnIdx + 1);
      }

      mData.at(mLabelParams.mColumnNameIdx).at(columnIdx) = pColumnName;
    }

    /**
     * @brief   Get column names
     * @returns vector of column names.
     */
    std::vector<std::string> GetColumnNames()
    {
      if (mLabelParams.mColumnNameIdx >= 0)
      {
        return std::vector<std::string>(mData.at(mLabelParams.mColumnNameIdx).begin() +
                                        (mLabelParams.mRowNameIdx + 1),
                                        mData.at(mLabelParams.mColumnNameIdx).end());
      }

      return std::vector<std::string>();
    }

    /**
     * @brief   Get row name
     * @param   pRowIdx               zero-based column index.
     * @returns row name.
     */
    std::string GetRowName(const ssize_t pRowIdx)
    {
      const ssize_t rowIdx = pRowIdx + (mLabelParams.mColumnNameIdx + 1);
      if (mLabelParams.mRowNameIdx < 0)
      {
        throw std::out_of_range("row name column index < 0: " + std::to_string(mLabelParams.mRowNameIdx));
      }

      return mData.at(rowIdx).at(mLabelParams.mRowNameIdx);
    }

    /**
     * @brief   Set row name
     * @param   pRowIdx               zero-based row index.
     * @param   pRowName              row name.
     */
    void SetRowName(size_t pRowIdx, const std::string& pRowName)
    {
      const ssize_t rowIdx = pRowIdx + (mLabelParams.mColumnNameIdx + 1);
      mRowNames[pRowName] = rowIdx;
      if (mLabelParams.mRowNameIdx < 0)
      {
        throw std::out_of_range("row name column index < 0: " + std::to_string(mLabelParams.mRowNameIdx));
      }

      // increase table size if necessary:
      if (rowIdx >= static_cast<int>(mData.size()))
      {
        mData.resize(rowIdx + 1);
      }
      auto& row = mData[rowIdx];
      if (mLabelParams.mRowNameIdx >= static_cast<int>(row.size()))
      {
        row.resize(mLabelParams.mRowNameIdx + 1);
      }

      mData.at(rowIdx).at(mLabelParams.mRowNameIdx) = pRowName;
    }

    /**
     * @brief   Get row names
     * @returns vector of row names.
     */
    std::vector<std::string> GetRowNames()
    {
      std::vector<std::string> rownames;
      if (mLabelParams.mRowNameIdx >= 0)
      {
        for (auto itRow = mData.begin(); itRow != mData.end(); ++itRow)
        {
          if (std::distance(mData.begin(), itRow) > mLabelParams.mColumnNameIdx)
          {
            rownames.push_back(itRow->at(mLabelParams.mRowNameIdx));
          }
        }
      }
      return rownames;
    }

  private:
    void ReadCsv()
    {
      std::ifstream stream;
      stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      stream.open(mPath, std::ios::binary);
      ReadCsv(stream);
    }

    void ReadCsv(std::istream& pStream)
    {
      Clear();
      pStream.seekg(0, std::ios::end);
      std::streamsize length = pStream.tellg();
      pStream.seekg(0, std::ios::beg);

#ifdef HAS_CODECVT
      std::vector<char> bom2b(2, '\0');
      if (length >= 2)
      {
        pStream.read(bom2b.data(), 2);
        pStream.seekg(0, std::ios::beg);
      }

      static const std::vector<char> bomU16le = { '\xff', '\xfe' };
      static const std::vector<char> bomU16be = { '\xfe', '\xff' };
      if ((bom2b == bomU16le) || (bom2b == bomU16be))
      {
        mIsUtf16 = true;
        mIsLE = (bom2b == bomU16le);

        std::wifstream wstream;
        wstream.exceptions(std::wifstream::failbit | std::wifstream::badbit);
        wstream.open(mPath, std::ios::binary);
        if (mIsLE)
        {
          wstream.imbue(std::locale(wstream.getloc(),
                                    new std::codecvt_utf16<wchar_t, 0x10ffff,
                                                           static_cast<std::codecvt_mode>(std::consume_header |
                                                                                          std::little_endian)>));
        }
        else
        {
          wstream.imbue(std::locale(wstream.getloc(),
                                    new std::codecvt_utf16<wchar_t, 0x10ffff,
                                                           std::consume_header>));
        }
        std::wstringstream wss;
        wss << wstream.rdbuf();
        std::string utf8 = ToString(wss.str());
        std::stringstream ss(utf8);
        ParseCsv(ss, utf8.size());
      }
      else
#endif
      {
        // check for UTF-8 Byte order mark and skip it when found
        if (length >= 3)
        {
          std::vector<char> bom3b(3, '\0');
          pStream.read(bom3b.data(), 3);
          static const std::vector<char> bomU8 = { '\xef', '\xbb', '\xbf' };
          if (bom3b != bomU8)
          {
            // file does not start with a UTF-8 Byte order mark
            pStream.seekg(0, std::ios::beg);
          }
          else
          {
            // file did start with a UTF-8 Byte order mark, simply skip it
            length -= 3;
          }
        }

        ParseCsv(pStream, length);
      }
    }

    void ParseCsv(std::istream& pStream, std::streamsize p_FileLength)
    {
      const std::streamsize bufLength = 64 * 1024;
      std::vector<char> buffer(bufLength);
      std::vector<std::string> row;
      std::string cell;
      bool quoted = false;
      int cr = 0;
      int lf = 0;

      while (p_FileLength > 0)
      {
        std::streamsize readLength = std::min<std::streamsize>(p_FileLength, bufLength);
        pStream.read(buffer.data(), readLength);
        for (int i = 0; i < readLength; ++i)
        {
          if (buffer[i] == '"')
          {
            if (cell.empty() || cell[0] == '"')
            {
              quoted = !quoted;
            }
            cell += buffer[i];
          }
          else if (buffer[i] == mSeparatorParams.mSeparator)
          {
            if (!quoted)
            {
              row.push_back(Unquote(Trim(cell)));
              cell.clear();
            }
            else
            {
              cell += buffer[i];
            }
          }
          else if (buffer[i] == '\r')
          {
            if (mSeparatorParams.mQuotedLinebreaks && quoted)
            {
              cell += buffer[i];
            }
            else
            {
              ++cr;
            }
          }
          else if (buffer[i] == '\n')
          {
            if (mSeparatorParams.mQuotedLinebreaks && quoted)
            {
              cell += buffer[i];
            }
            else
            {
              ++lf;
              row.push_back(Unquote(Trim(cell)));
              cell.clear();
              mData.push_back(row);
              row.clear();
              quoted = false;
            }
          }
          else
          {
            cell += buffer[i];
          }
        }
        p_FileLength -= readLength;
      }

      // Handle last line without linebreak
      if (!cell.empty() || !row.empty())
      {
        row.push_back(Unquote(Trim(cell)));
        cell.clear();
        mData.push_back(row);
        row.clear();
      }

      // Assume CR/LF if at least half the linebreaks have CR
      mSeparatorParams.mHasCR = (cr > (lf / 2));

      // Set up column labels
      if ((mLabelParams.mColumnNameIdx >= 0) &&
          (static_cast<ssize_t>(mData.size()) > mLabelParams.mColumnNameIdx))
      {
        int i = 0;
        for (auto& columnName : mData[mLabelParams.mColumnNameIdx])
        {
          mColumnNames[columnName] = i++;
        }
      }

      // Set up row labels
      if ((mLabelParams.mRowNameIdx >= 0) &&
          (static_cast<ssize_t>(mData.size()) >
           (mLabelParams.mColumnNameIdx + 1)))
      {
        int i = 0;
        for (auto& dataRow : mData)
        {
          if (static_cast<ssize_t>(dataRow.size()) > mLabelParams.mRowNameIdx)
          {
            mRowNames[dataRow[mLabelParams.mRowNameIdx]] = i++;
          }
        }
      }
    }

    void WriteCsv() const
    {
#ifdef HAS_CODECVT
      if (mIsUtf16)
      {
        std::stringstream ss;
        WriteCsv(ss);
        std::string utf8 = ss.str();
        std::wstring wstr = ToWString(utf8);

        std::wofstream wstream;
        wstream.exceptions(std::wofstream::failbit | std::wofstream::badbit);
        wstream.open(mPath, std::ios::binary | std::ios::trunc);

        if (mIsLE)
        {
          wstream.imbue(std::locale(wstream.getloc(),
                                    new std::codecvt_utf16<wchar_t, 0x10ffff,
                                                           static_cast<std::codecvt_mode>(std::little_endian)>));
        }
        else
        {
          wstream.imbue(std::locale(wstream.getloc(),
                                    new std::codecvt_utf16<wchar_t, 0x10ffff>));
        }

        wstream << static_cast<wchar_t>(0xfeff);
        wstream << wstr;
      }
      else
#endif
      {
        std::ofstream stream;
        stream.exceptions(std::ofstream::failbit | std::ofstream::badbit);
        stream.open(mPath, std::ios::binary | std::ios::trunc);
        WriteCsv(stream);
      }
    }

    void WriteCsv(std::ostream& pStream) const
    {
      for (auto itr = mData.begin(); itr != mData.end(); ++itr)
      {
        for (auto itc = itr->begin(); itc != itr->end(); ++itc)
        {
          if (mSeparatorParams.mAutoQuote &&
              ((itc->find(mSeparatorParams.mSeparator) != std::string::npos) ||
               (itc->find(' ') != std::string::npos)))
          {
            // escape quotes in string
            std::string str = *itc;
            ReplaceString(str, "\"", "\"\"");

            pStream << "\"" << str << "\"";
          }
          else
          {
            pStream << *itc;
          }

          if (std::distance(itc, itr->end()) > 1)
          {
            pStream << mSeparatorParams.mSeparator;
          }
        }
        pStream << (mSeparatorParams.mHasCR ? "\r\n" : "\n");
      }
    }

    size_t GetDataRowCount() const
    {
      return mData.size();
    }

    size_t GetDataColumnCount() const
    {
      return (mData.size() > 0) ? mData.at(0).size() : 0;
    }

    std::string Trim(const std::string& pStr)
    {
      if (mSeparatorParams.mTrim)
      {
        std::string str = pStr;

        // ltrim
        str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](int ch) { return !isspace(ch); }));

        // rtrim
        str.erase(std::find_if(str.rbegin(), str.rend(), [](int ch) { return !isspace(ch); }).base(), str.end());

        return str;
      }
      else
      {
        return pStr;
      }
    }

    std::string Unquote(const std::string& pStr)
    {
      if (mSeparatorParams.mAutoQuote && (pStr.size() >= 2) && (pStr.front() == '"') && (pStr.back() == '"'))
      {
        // remove start/end quotes
        std::string str = pStr.substr(1, pStr.size() - 2);

        // unescape quotes in string
        ReplaceString(str, "\"\"", "\"");

        return str;
      }
      else
      {
        return pStr;
      }
    }

#ifdef HAS_CODECVT
#if defined(_MSC_VER)
#pragma warning (disable: 4996)
#endif
    static std::string ToString(const std::wstring& pWStr)
    {
      size_t len = std::wcstombs(nullptr, pWStr.c_str(), 0) + 1;
      char* cstr = new char[len];
      std::wcstombs(cstr, pWStr.c_str(), len);
      std::string str(cstr);
      delete[] cstr;
      return str;
    }

    static std::wstring ToWString(const std::string& pStr)
    {
      size_t len = 1 + mbstowcs(nullptr, pStr.c_str(), 0);
      wchar_t* wcstr = new wchar_t[len];
      std::mbstowcs(wcstr, pStr.c_str(), len);
      std::wstring wstr(wcstr);
      delete[] wcstr;
      return wstr;
    }
#if defined(_MSC_VER)
#pragma warning (default: 4996)
#endif
#endif

    static void ReplaceString(std::string& pStr, const std::string& pSearch, const std::string& pReplace)
    {
      size_t pos = 0;

      while ((pos = pStr.find(pSearch, pos)) != std::string::npos)
      {
        pStr.replace(pos, pSearch.size(), pReplace);
        pos += pReplace.size();
      }
    }

  private:
    std::string mPath;
    LabelParams mLabelParams;
    SeparatorParams mSeparatorParams;
    ConverterParams mConverterParams;
    std::vector<std::vector<std::string>> mData;
    std::map<std::string, size_t> mColumnNames;
    std::map<std::string, size_t> mRowNames;
#ifdef HAS_CODECVT
    bool mIsUtf16 = false;
    bool mIsLE = false;
#endif
  };
}
