/*
 * rapidcsv.h
 *
 * URL:      https://github.com/d99kris/rapidcsv
 * Version:  1.1
 *
 * Copyright (C) 2017 Kristofer Berggren
 * All rights reserved.
 * 
 * rapidcsv is distributed under the BSD 3-Clause license, see LICENSE for details.
 *
 */

#pragma once

#include <assert.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <typeinfo>

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#define DEFAULT_HASCR true
#else
#define DEFAULT_HASCR false
#endif

namespace rapidcsv
{
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
    void ToStr(std::string& pStr, const T& pVal)
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
        pStr = std::to_string(pVal);
      }
      else
      {
        throw no_converter();
      }
    }

    void ToVal(T& pVal, const std::string& pStr)
    {
      if (typeid(T) == typeid(int))
      {
        pVal = static_cast<T>(std::stoi(pStr));
      }
      else if (typeid(T) == typeid(long))
      {
        pVal = static_cast<T>(std::stol(pStr));
      }
      else if (typeid(T) == typeid(long long))
      {
        pVal = static_cast<T>(std::stoll(pStr));
      }
      else if (typeid(T) == typeid(unsigned))
      {
        pVal = static_cast<T>(std::stoul(pStr));
      }
      else if (typeid(T) == typeid(unsigned long))
      {
        pVal = static_cast<T>(std::stoul(pStr));
      }
      else if (typeid(T) == typeid(unsigned long long))
      {
        pVal = static_cast<T>(std::stoull(pStr));
      }
      else if (typeid(T) == typeid(float))
      {
        pVal = static_cast<T>(std::stof(pStr));
      }
      else if (typeid(T) == typeid(double))
      {
        pVal = static_cast<T>(std::stod(pStr));
      }
      else if (typeid(T) == typeid(long double))
      {
        pVal = static_cast<T>(std::stold(pStr));
      }
      else if (typeid(T) == typeid(char))
      {
        pVal = static_cast<T>(pStr[0]);
      }
      else
      {
        throw no_converter();
      }
    }
  };

  template<>
  inline void Converter<std::string>::ToStr(std::string& pStr, const std::string& pVal)
  {
    pStr = pVal;
  }

  template<>
  inline void Converter<std::string>::ToVal(std::string& pVal, const std::string& pStr)
  {
    pVal = pStr;
  }

  struct Properties
  {
    Properties(const std::string& pPath = "", const int pColumnNameIdx = 0, const int pRowNameIdx = 0, const bool pHasCR = DEFAULT_HASCR)
      : mPath(pPath)
      , mColumnNameIdx(pColumnNameIdx)
      , mRowNameIdx(pRowNameIdx)
      , mHasCR(pHasCR)
    {
    }
    std::string mPath;
    int mColumnNameIdx;
    int mRowNameIdx;
    bool mHasCR;
  };

  class Document
  {
  public:
    // Contructors --------------------------------------------------------
    explicit Document(const std::string& pPath = "")
    {
      mProperties.mPath = pPath;
      if (!mProperties.mPath.empty())
      {
        ReadCsv();
      }
    }

    explicit Document(const Properties& pProperties)
      : mProperties(pProperties)
    {
      if (!mProperties.mPath.empty())
      {
        ReadCsv();
      }
    }

    explicit Document(const Document& pDocument)
      : mProperties(pDocument.mProperties)
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
      mProperties.mPath = pPath;
      ReadCsv();
    }

    void Save(const std::string& pPath = "")
    {
      if (!pPath.empty())
      {
        mProperties.mPath = pPath;
      }
      WriteCsv();
    }

    // Column Methods -----------------------------------------------------
    template<typename T>
    std::vector<T> GetColumn(const size_t pColumnIdx)
    {
      const ssize_t columnIdx = pColumnIdx + (mProperties.mRowNameIdx + 1);
      std::vector<T> column;
      Converter<T> converter;
      for (auto itRow = mData.begin(); itRow != mData.end(); ++itRow)
      {
        if (std::distance(mData.begin(), itRow) > mProperties.mColumnNameIdx)
        {
          T val;
          converter.ToVal(val, itRow->at(columnIdx));
          column.push_back(val);
        }
      }
      return column;
    }

    template<typename T>
    std::vector<T> GetColumn(const std::string& pColumnName)
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
      const size_t columnIdx = pColumnIdx + (mProperties.mRowNameIdx + 1);
        
      while (pColumn.size() + (mProperties.mColumnNameIdx + 1) > GetRowCount())
      {
        std::vector<std::string> row;
        row.resize(GetColumnCount());
        mData.push_back(row);
      }

      if ((columnIdx + 1) > GetColumnCount())
      {
        for (auto itRow = mData.begin(); itRow != mData.end(); ++itRow)
        {
          itRow->resize(columnIdx + 1 + (mProperties.mRowNameIdx + 1));
        }
      }
      
      Converter<T> converter;
      for (auto itRow = pColumn.begin(); itRow != pColumn.end(); ++itRow)
      {
        std::string str;
        converter.ToStr(str, *itRow);
        mData.at(std::distance(pColumn.begin(), itRow) + (mProperties.mColumnNameIdx + 1)).at(columnIdx) = str;
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
      const ssize_t columnIdx = pColumnIdx + (mProperties.mRowNameIdx + 1);
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

    // Row Methods --------------------------------------------------------
    template<typename T>
    std::vector<T> GetRow(const size_t pRowIdx)
    {
      const ssize_t rowIdx = pRowIdx + (mProperties.mColumnNameIdx + 1);
      std::vector<T> row;
      Converter<T> converter;
      for (auto itCol = mData.at(rowIdx).begin(); itCol != mData.at(rowIdx).end(); ++itCol)
      {
        if (std::distance(mData.at(rowIdx).begin(), itCol) > mProperties.mRowNameIdx)
        {
          T val;
          converter.ToVal(val, *itCol);
          row.push_back(val);
        }
      }
      return row;
    }

    template<typename T>
    std::vector<T> GetRow(const std::string& pRowName)
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
      const size_t rowIdx = pRowIdx + (mProperties.mColumnNameIdx + 1);

      while ((rowIdx + 1) > GetRowCount())
      {
        std::vector<std::string> row;
        row.resize(GetColumnCount());
        mData.push_back(row);
      }

      if (pRow.size() > GetColumnCount())
      {
        for (auto itRow = mData.begin(); itRow != mData.end(); ++itRow)
        {
          itRow->resize(pRow.size() + (mProperties.mRowNameIdx + 1));
        }
      }

      Converter<T> converter;
      for (auto itCol = pRow.begin(); itCol != pRow.end(); ++itCol)
      {
        std::string str;
        converter.ToStr(str, *itCol);
        mData.at(rowIdx).at(std::distance(pRow.begin(), itCol) + (mProperties.mRowNameIdx + 1)) = str;
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
      const ssize_t rowIdx = pRowIdx + (mProperties.mColumnNameIdx + 1);
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

    // Cell Methods -------------------------------------------------------
    template<typename T>
    T GetCell(const size_t pColumnIdx, const size_t pRowIdx)
    {
      const ssize_t columnIdx = pColumnIdx + (mProperties.mRowNameIdx + 1);
      const ssize_t rowIdx = pRowIdx + (mProperties.mColumnNameIdx + 1);
        
      T val;
      Converter<T> converter;
      converter.ToVal(val, mData.at(rowIdx).at(columnIdx));
      return val;
    }

    template<typename T>
    T GetCell(const std::string& pColumnName, const std::string& pRowName)
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
      const size_t columnIdx = pColumnIdx + (mProperties.mRowNameIdx + 1);
      const size_t rowIdx = pRowIdx + (mProperties.mColumnNameIdx + 1);

      while ((rowIdx + 1) > GetRowCount())
      {
        std::vector<std::string> row;
        row.resize(GetColumnCount());
        mData.push_back(row);
      }

      if ((columnIdx + 1) > GetColumnCount())
      {
        for (auto itRow = mData.begin(); itRow != mData.end(); ++itRow)
        {
          itRow->resize(columnIdx + 1);
        }
      }

      std::string str;
      Converter<T> converter;
      converter.ToStr(str, pCell);
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
      const ssize_t columnIdx = pColumnIdx + (mProperties.mRowNameIdx + 1);
      mColumnNames[pColumnName] = columnIdx;
      if (mProperties.mColumnNameIdx >= 0)
      {
        mData.at(mProperties.mColumnNameIdx).at(columnIdx) = pColumnName;
      }
    }

    void SetRowLabel(size_t pRowIdx, const std::string& pRowName)
    {
      const ssize_t rowIdx = pRowIdx + (mProperties.mColumnNameIdx + 1);
      mRowNames[pRowName] = rowIdx;
      if (mProperties.mRowNameIdx >= 0)
      {
        mData.at(rowIdx).at(mProperties.mRowNameIdx) = pRowName;
      }
    }

  private:
    void ReadCsv()
    {
      std::ifstream file;
      file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
      file.open(mProperties.mPath, std::ios::binary | std::ios::ate);
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
          switch(buffer[i])
          {
            case '"':
              if (cell.empty() || cell[0] == '"')
              {
                quoted = !quoted;
              }
              cell = cell + buffer[i];
              break;

            case ',':
              if (!quoted)
              {
                row.push_back(cell);
                cell.clear();
              }
              else
              {
                cell = cell + buffer[i];
              }
              break;

            case '\r':
              ++cr;
              break;

            case '\n':
              ++lf;
              row.push_back(cell);
              cell.clear();
              mData.push_back(row);
              row.clear();
              quoted = false; // disallow line breaks in quoted string, by auto-unquote at linebreak
              break;

            default:
              cell = cell + buffer[i];
              break;
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
      mProperties.mHasCR = (cr > (lf / 2));

      // Set up column labels
      if ((mProperties.mColumnNameIdx >= 0) &&
          (mData.size() > 0))
      {
        int i = 0;
        for (auto& columnName : mData[mProperties.mColumnNameIdx])
        {
          mColumnNames[columnName] = i++;
        }
      }

      // Set up row labels
      if ((mProperties.mRowNameIdx >= 0) &&
          (static_cast<ssize_t>(mData.size()) >
           (mProperties.mColumnNameIdx + 1)))
      {
        int i = 0;
        for (auto& dataRow : mData)
        {
          mRowNames[dataRow[mProperties.mRowNameIdx]] = i++;
        }
      }
    }

    void WriteCsv()
    {
      std::ofstream file;
      file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
      file.open(mProperties.mPath, std::ios::binary | std::ios::trunc);
      for (auto itr = mData.begin(); itr != mData.end(); ++itr)
      {
        for (auto itc = itr->begin(); itc != itr->end(); ++itc)
        {
          file << *itc;
          if (std::distance(itc, itr->end()) > 1)
          {
            file << ",";
          }
        }
        file << (mProperties.mHasCR ? "\r\n" : "\n");
      }
    }

    ssize_t GetColumnIdx(const std::string& pColumnName)
    {
      if (mProperties.mColumnNameIdx >= 0)
      {
        if (mColumnNames.find(pColumnName) != mColumnNames.end())
        {
          return mColumnNames[pColumnName] - (mProperties.mRowNameIdx + 1);
        }
      }
      return -1;
    }

    ssize_t GetRowIdx(const std::string& pRowName)
    {
      if (mProperties.mRowNameIdx >= 0)
      {
        if (mRowNames.find(pRowName) != mRowNames.end())
        {
          return mRowNames[pRowName] - (mProperties.mColumnNameIdx + 1);
        }
      }
      return -1;
    }

    size_t GetRowCount()
    {
      return mData.size();
    }

    size_t GetColumnCount()
    {
      return (mData.size() > 0) ? mData.at(0).size() : 0;
    }

  private:
    Properties mProperties;
    std::vector<std::vector<std::string> > mData;
    std::map<std::string, size_t> mColumnNames;
    std::map<std::string, size_t> mRowNames;
  };
}

