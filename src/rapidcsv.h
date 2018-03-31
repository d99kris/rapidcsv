/*
 * rapidcsv.h
 *
 * URL:      https://github.com/d99kris/rapidcsv
 * Version:  2.0
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

  struct ConverterParams
  {
    explicit ConverterParams(const bool pHasDefaultConverter = false,
                             const long double pDefaultFloat = std::numeric_limits<long double>::signaling_NaN(),
                             const long long pDefaultInteger = 0)
      : mHasDefaultConverter(pHasDefaultConverter)
      , mDefaultFloat(pDefaultFloat)
      , mDefaultInteger(pDefaultInteger)
    {
    }

    bool mHasDefaultConverter;
    long double mDefaultFloat;
    long long mDefaultInteger;
  };
  
  class no_converter : public std::exception
  {
    virtual const char* what() const throw()
    {
      return "unsupported conversion datatype";
    }
  };

  template <typename T>
  class Converter
  {
  public:
    Converter(const ConverterParams& pConverterParams)
      : mConverterParams(pConverterParams)
    {
    }
    
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
          typeid(T) == typeid(long double))
      {
        pStr = std::to_string(pVal);
      }
      else if (typeid(T) == typeid(char))
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

  template<>
  inline void Converter<std::string>::ToStr(const std::string& pVal, std::string& pStr) const
  {
    pStr = pVal;
  }

  template<>
  inline void Converter<std::string>::ToVal(const std::string& pStr, std::string& pVal) const
  {
    pVal = pStr;
  }

  struct LabelParams
  {
    explicit LabelParams(const int pColumnNameIdx = 0, const int pRowNameIdx = 0)
      : mColumnNameIdx(pColumnNameIdx)
      , mRowNameIdx(pRowNameIdx)      
    {
    }

    int mColumnNameIdx;
    int mRowNameIdx;
  };

  struct SeparatorParams
  {
    explicit SeparatorParams(const char pSeparator = ',', const bool pHasCR = sPlatformHasCR)
      : mSeparator(pSeparator)
      , mHasCR(pHasCR)
    {
    }

    char mSeparator;
    bool mHasCR;
  };

  class Document
  {
  public:
    // Contructors --------------------------------------------------------
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

    // Destructors --------------------------------------------------------
    virtual ~Document()
    {
    }

    // Document Methods ---------------------------------------------------
    void Load(const std::string& pPath)
    {
      mPath = pPath;
      ReadCsv();
    }

    void Save(const std::string& pPath = std::string())
    {
      if (!pPath.empty())
      {
        mPath = pPath;
      }
      WriteCsv();
    }

    // Column Methods -----------------------------------------------------
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

    void RemoveColumn(const size_t pColumnIdx)
    {
      const ssize_t columnIdx = pColumnIdx + (mLabelParams.mRowNameIdx + 1);
      for (auto itRow = mData.begin(); itRow != mData.end(); ++itRow)
      {
        itRow->erase(itRow->begin() + columnIdx);
      }
    }

    void RemoveColumn(const std::string& pColumnName)
    {
      ssize_t columnIdx = GetColumnIdx(pColumnName);
      if (columnIdx < 0)
      {
        throw std::out_of_range("column not found: " + pColumnName);
      }

      RemoveColumn(columnIdx);
    }

    size_t GetColumnCount() const
    {
      return (mData.size() > 0) ? (mData.at(0).size() - (mLabelParams.mRowNameIdx + 1)) : 0;
    }

    // Row Methods --------------------------------------------------------
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

    void RemoveRow(const size_t pRowIdx)
    {
      const ssize_t rowIdx = pRowIdx + (mLabelParams.mColumnNameIdx + 1);
      mData.erase(mData.begin() + rowIdx);
    }

    void RemoveRow(const std::string& pRowName)
    {
      ssize_t rowIdx = GetRowIdx(pRowName);
      if (rowIdx < 0)
      {
        throw std::out_of_range("row not found: " + pRowName);
      }

      RemoveRow(rowIdx);
    }

    size_t GetRowCount() const
    {
      return mData.size() - (mLabelParams.mColumnNameIdx + 1);
    }

    // Cell Methods -------------------------------------------------------
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

    // Label Methods ------------------------------------------------------
    void SetColumnLabel(size_t pColumnIdx, const std::string& pColumnName)
    {
      const ssize_t columnIdx = pColumnIdx + (mLabelParams.mRowNameIdx + 1);
      mColumnNames[pColumnName] = columnIdx;
      if (mLabelParams.mColumnNameIdx >= 0)
      {
        mData.at(mLabelParams.mColumnNameIdx).at(columnIdx) = pColumnName;
      }
    }

    void SetRowLabel(size_t pRowIdx, const std::string& pRowName)
    {
      const ssize_t rowIdx = pRowIdx + (mLabelParams.mColumnNameIdx + 1);
      mRowNames[pRowName] = rowIdx;
      if (mLabelParams.mRowNameIdx >= 0)
      {
        mData.at(rowIdx).at(mLabelParams.mRowNameIdx) = pRowName;
      }
    }

  private:
    void ReadCsv()
    {
      std::ifstream file;
      file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      file.open(mPath, std::ios::binary | std::ios::ate);
      std::streamsize fileLength = file.tellg();
      file.seekg(0, std::ios::beg);
      const std::streamsize bufLength = 64 * 1024;
      std::vector<char> buffer(bufLength);
      std::vector<std::string> row;
      std::string cell;
      bool quoted = false;
      int cr = 0;
      int lf = 0;

      while (fileLength > 0)
      {
        long long readLength = std::min(fileLength, bufLength);
        file.read(buffer.data(), readLength);
        for (int i = 0; i < readLength; ++i)
        {
          if (buffer[i] == '"')
          {
            if (cell.empty() || cell[0] == '"')
            {
              quoted = !quoted;
            }
            cell = cell + buffer[i];
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
              cell = cell + buffer[i];
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
              cell = cell + buffer[i];
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
      std::ofstream file;
      file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
      file.open(mPath, std::ios::binary | std::ios::trunc);
      for (auto itr = mData.begin(); itr != mData.end(); ++itr)
      {
        for (auto itc = itr->begin(); itc != itr->end(); ++itc)
        {
          file << *itc;
          if (std::distance(itc, itr->end()) > 1)
          {
            file << mSeparatorParams.mSeparator;
          }
        }
        file << (mSeparatorParams.mHasCR ? "\r\n" : "\n");
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

