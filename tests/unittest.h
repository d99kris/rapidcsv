#pragma once

#ifndef _MSC_VER
#include <unistd.h>
#else
#include <io.h>
#endif

#include <fstream>
#include <string>
#include <sstream>

namespace unittest
{
  namespace detail
  {
    inline std::string FileName(const std::string& pPath)
    {
      const std::size_t slash = pPath.rfind("/");
      std::string name = (slash != std::string::npos) ? pPath.substr(slash + 1) : pPath;
      return name;
    }
  }

  inline std::string TempPath()
  {
    int fd = -1;
    char name[] = "rapidcsvtest.XXXXXX";
#ifndef _MSC_VER
    fd = mkstemp(name);
    close(fd);
#else
    _mktemp(name);    
#endif
    return std::string(name);
  }
  
  inline void WriteFile(const std::string& pPath, const std::string& pData)
  {
    std::ofstream outfile(pPath);
    outfile << pData;
    outfile.close();
  }
  
  inline std::string ReadFile(const std::string& pPath)
  {
    std::ifstream infile(pPath);
    std::string data((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
    infile.close();
    return data;
  }

  inline void DeleteFile(const std::string& pPath)
  {
    std::remove(pPath.c_str());
  }

  #define ExpectEqual(t, a, b) ExpectEqualFun<t>(a, b, #a, #b, __FILE__, __LINE__); 

  template<typename T>
  inline void ExpectEqualFun(T pTest, T pRef, const std::string& testName, const std::string& refName, const std::string& filePath, int lineNo)
  {
    if (pTest != pRef)
    {
      std::stringstream ss;
      ss << detail::FileName(filePath) << ":" << std::to_string(lineNo) << " ExpectEqual failed: " << testName << " != " << refName << std::endl;
      ss << testName << " = '" << pTest << "'" << std::endl;
      ss << refName << " = '" << pRef << "'" << std::endl;

      throw std::runtime_error(ss.str());
    }
  }

  #define ExpectTrue(a) ExpectTrueFun(a, #a, __FILE__, __LINE__); 

  inline void ExpectTrueFun(bool pTest, const std::string& testName, const std::string& filePath, int lineNo)
  {
    if (!pTest)
    {
      std::stringstream ss;
      ss << detail::FileName(filePath) << ":" << std::to_string(lineNo) << " ExpectTrue failed: " << testName << " == false" << std::endl;

      throw std::runtime_error(ss.str());
    }
  }
}

