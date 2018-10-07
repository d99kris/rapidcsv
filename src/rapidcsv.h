/*
 * rapidcsv.h
 *
 * URL:      https://github.com/d99kris/rapidcsv
 * Version:  3.1
 *
 * Copyright (C) 2017-2018 Kristofer Berggren
 * All rights reserved.
 *
 * rapidcsv is distributed under the BSD 3-Clause license, see LICENSE for details.
 *
 */

#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
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
     *                                to all rows as document data.
     * @param   pRowNameIdx           specifies the zero-based column index of the row labels, setting
     *                                it to -1 prevents row lookup by label name, and gives access
     *                                to all columns as document data.
     */
    explicit LabelParams(const int pColumnNameIdx = 0, const int pRowNameIdx = 0)
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
     * @param   pHasCR                specifies whether a new document (i.e. not an existing document read)
     *                                should use CR/LF instead of only LF (default is to use standard
     *                                behavior of underlying platforms - CR/LF for Win, and LF for others).
     */
    explicit SeparatorParams(const char pSeparator = ',', const bool pHasCR = sPlatformHasCR)
      : mSeparator(pSeparator)
      , mHasCR(pHasCR)
    {
    }

    /**
     * @brief   specifies the column separator.
     */
    char mSeparator;

    /**
     * @brief   specifies whether new documents should use CR/LF instead of LF.
     */
    bool mHasCR;
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
     * @param   pStream               specified an input stream to read CSV data from.
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
     * @brief   Copy constructor
     * @param   pDocument             specifies the Document instance to copy.
     */
    explicit Document(const Document& pDocument)
      : mPath(pDocument.mPath)
      , mLabelParams(pDocument.mLabelParams)
      , mSeparatorParams(pDocument.mSeparatorParams)
      , mConverterParams(pDocument.mConverterParams)
      , mData(pDocument.mData)
      , mColumnNames(pDocument.mColumnNames)
      , mRowNames(pDocument.mRowNames)
    {
    }

    /**
     * @brief   Read Document data from file.
     * @param   pPath                 specifies the path of an existing CSV-file to populate the Document
     *                                data with.
     */
    void Load(const std::string& pPath)
    {
      mPath = pPath;
      ReadCsv();
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
     * @param   pStream               specified an output stream to write the data to.
     */
    void Save(std::ostream& pStream)
    {
      WriteCsv(pStream);
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
     * @brief   Get number of data columns.
     * @returns column count.
     */
    size_t GetColumnCount() const
    {
      return (mData.size() > 0) ? (mData.at(0).size() - (mLabelParams.mRowNameIdx + 1)) : 0;
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
     * @brief   Get number of data rows.
     * @returns row count.
     */
    size_t GetRowCount() const
    {
      return mData.size() - (mLabelParams.mColumnNameIdx + 1);
    }

    /**
     * @brief   Get cell by index.
     * @param   pRowIdx               zero-based row index.
     * @param   pColumnIdx            zero-based column index.
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
      stream.open(mPath, std::ios::binary | std::ios::ate);
      ReadCsv(stream);
    }

    void ReadCsv(std::istream& pStream)
    {
      pStream.seekg(0, std::ios::end);
      std::streamsize fileLength = pStream.tellg();
      pStream.seekg(0, std::ios::beg);
      const std::streamsize bufLength = 64 * 1024;
      std::vector<char> buffer(bufLength);
      std::vector<std::string> row;
      std::string cell;
      bool quoted = false;
      int cr = 0;
      int lf = 0;

      while (fileLength > 0)
      {
        std::streamsize readLength = std::min(fileLength, bufLength);
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
              row.push_back(cell);
              cell.clear();
            }
            else
            {
              cell += buffer[i];
            }
          }
          else if (buffer[i] == '\r')
          {
            ++cr;
          }
          else if (buffer[i] == '\n')
          {
            ++lf;
            row.push_back(cell);
            cell.clear();
            mData.push_back(row);
            row.clear();
            quoted = false; // disallow line breaks in quoted string, by auto-unquote at linebreak
          }
          else
          {
            cell += buffer[i];
          }
        }
        fileLength -= readLength;
      }

      // Handle last line without linebreak
      if (!cell.empty() || !row.empty())
      {
        row.push_back(cell);
        cell.clear();
        mData.push_back(row);
        row.clear();
      }

      // Assume CR/LF if at least half the linebreaks have CR
      mSeparatorParams.mHasCR = (cr > (lf / 2));

      // Set up column labels
      if ((mLabelParams.mColumnNameIdx >= 0) &&
          (mData.size() > 0))
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
          mRowNames[dataRow[mLabelParams.mRowNameIdx]] = i++;
        }
      }
    }

    void WriteCsv() const
    {
      std::ofstream stream;
      stream.exceptions(std::ofstream::failbit | std::ofstream::badbit);
      stream.open(mPath, std::ios::binary | std::ios::trunc);
      WriteCsv(stream);
    }

    void WriteCsv(std::ostream& pStream) const
    {
      for (auto itr = mData.begin(); itr != mData.end(); ++itr)
      {
        for (auto itc = itr->begin(); itc != itr->end(); ++itc)
        {
          if ((std::string::npos == itc->find(mSeparatorParams.mSeparator)) ||
              ((itc->length() >= 2) && ((*itc)[0] == '\"') && ((*itc)[itc->length() - 1] == '\"')))
          {
            pStream << *itc;
          }
          else
          {
            pStream << '"' << *itc << '"';
          }

          if (std::distance(itc, itr->end()) > 1)
          {
            pStream << mSeparatorParams.mSeparator;
          }
        }
        pStream << (mSeparatorParams.mHasCR ? "\r\n" : "\n");
      }
    }

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

    size_t GetDataRowCount() const
    {
      return mData.size();
    }

    size_t GetDataColumnCount() const
    {
      return (mData.size() > 0) ? mData.at(0).size() : 0;
    }

  private:
    std::string mPath;
    LabelParams mLabelParams;
    SeparatorParams mSeparatorParams;
    ConverterParams mConverterParams;
    std::vector<std::vector<std::string> > mData;
    std::map<std::string, size_t> mColumnNames;
    std::map<std::string, size_t> mRowNames;
  };
}
