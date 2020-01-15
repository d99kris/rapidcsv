#pragma once

#ifndef _MSC_VER
#include <unistd.h>
#else
#include <io.h>
#endif

#include <fstream>
#include <string>
#include <sstream>


#define ExpectEqual(t, a, b) ExpectEqualFun<t>(a, b, #a, #b, __FILE__, __LINE__)
#define ExpectTrue(a) ExpectTrueFun(a, #a, __FILE__, __LINE__)

#define ExpectException(expr, excp)                                                           \
  do                                                                                          \
  {                                                                                           \
    bool success = false;                                                                     \
    try                                                                                       \
    {                                                                                         \
      expr;                                                                                   \
    }                                                                                         \
    catch (const excp &)                                                                      \
    {                                                                                         \
      success = true;                                                                         \
    }                                                                                         \
    catch (const std::exception& ex)                                                          \
    {                                                                                         \
      std::stringstream ss;                                                                   \
      ss << unittest::detail::FileName(__FILE__) << ":" << std::to_string(__LINE__);          \
      ss << " ExpectException failed: unexpected exception '" << typeid(ex).name();           \
      ss << "' thrown." << std::endl;                                                         \
      throw std::runtime_error(ss.str());                                                     \
    }                                                                                         \
                                                                                              \
    if (!success)                                                                             \
    {                                                                                         \
      std::stringstream ss;                                                                   \
      ss << unittest::detail::FileName(__FILE__) << ":" << std::to_string(__LINE__);          \
      ss << " ExpectException failed: expected exception '" << #excp << "' not thrown.";      \
      ss << std::endl;                                                                        \
      throw std::runtime_error(ss.str());                                                     \
    }                                                                                         \
  }                                                                                           \
  while (0)


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
    char name[] = "rapidcsvtest.XXXXXX";
#ifndef _MSC_VER
    int fd = mkstemp(name);
    close(fd);
#else
    _mktemp_s(name, strlen(name) + 1);
#endif
    return std::string(name);
  }

  inline void WriteFile(const std::string& pPath, const std::string& pData)
  {
    std::ofstream outfile;
    outfile.open(pPath, std::ifstream::out | std::ifstream::binary);
    outfile << pData;
    outfile.close();
  }

  inline std::string ReadFile(const std::string& pPath)
  {
    std::ifstream infile;
    infile.open(pPath, std::ifstream::in | std::ifstream::binary);
    std::string data((std::istreambuf_iterator<char>(infile)), std::istreambuf_iterator<char>());
    infile.close();
    return data;
  }

  inline void DeleteFile(const std::string& pPath)
  {
    std::remove(pPath.c_str());
  }

  template<typename T>
  inline void ExpectEqualFun(T pTest, T pRef, const std::string& testName,
                             const std::string& refName, const std::string& filePath, int lineNo)
  {
    if (pTest != pRef)
    {
      std::stringstream ss;
      ss << detail::FileName(filePath) << ":" << std::to_string(lineNo);
      ss << " ExpectEqual failed: " << testName << " != " << refName << std::endl;
      ss << testName << " = '" << pTest << "'" << std::endl;
      ss << refName << " = '" << pRef << "'" << std::endl;

      throw std::runtime_error(ss.str());
    }
  }

  inline void ExpectTrueFun(bool pTest, const std::string& testName, const std::string& filePath,
                            int lineNo)
  {
    if (!pTest)
    {
      std::stringstream ss;
      ss << detail::FileName(filePath) << ":" << std::to_string(lineNo);
      ss << " ExpectTrue failed: " << testName << " == false" << std::endl;

      throw std::runtime_error(ss.str());
    }
  }
}
