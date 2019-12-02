## class rapidcsv::Converter< T >

Class providing conversion to/from numerical datatypes and strings. Only intended for rapidcsv internal usage, but exposed externally to allow specialization for custom datatype conversions.  

---

```c++
template<typename T> Converter (bool pHasDefaultConverter, const T & pDefaultVal)
```
Constructor. 

**Parameters**
- `pHasDefaultConverter` specifies if conversion of non-numerical strings shall be converted to a default value, instead of causing an exception to be thrown (default). 
- `pDefaultVal` default value to represent invalid numbers. 

---

```c++
template<typename T> Converter (bool pHasDefaultConverter)
```
Constructor. 

**Parameters**
- `pHasDefaultConverter` specifies if conversion of non-numerical strings shall be converted to a default value, instead of causing an exception to be thrown (default). 

---

```c++
template<> Converter< std::string >::Converter (bool pHasDefaultConverter)
```
Specialized constructor handling string conversion. 

**Parameters**
- `pHasDefaultConverter` specifies if conversion of non-numerical strings shall be converted to a default value, instead of causing an exception to be thrown (default). 

---

```c++
template<> Converter< float >::Converter (bool pHasDefaultConverter)
```
Specialized constructor handling float conversion. 

**Parameters**
- `pHasDefaultConverter` specifies if conversion of non-numerical strings shall be converted to a default value, instead of causing an exception to be thrown (default). 

---

```c++
template<> Converter< double >::Converter (bool pHasDefaultConverter)
```
Specialized constructor handling double conversion. 

**Parameters**
- `pHasDefaultConverter` specifies if conversion of non-numerical strings shall be converted to a default value, instead of causing an exception to be thrown (default). 

---

```c++
template<> Converter< long double >::Converter (bool pHasDefaultConverter)
```
Specialized constructor handling long double conversion. 

**Parameters**
- `pHasDefaultConverter` specifies if conversion of non-numerical strings shall be converted to a default value, instead of causing an exception to be thrown (default). 

---

```c++
template<typename T> virtual void ToStr (const T & pVal, std::string & pStr)
```
Converts numerical value to string representation. 

**Parameters**
- `pVal` value 
- `pStr` output string 

---

```c++
template<> void Converter< std::string >::ToStr (const std::string & pVal, std::string & pStr)
```
Specialized implementation handling string to string conversion. 

**Parameters**
- `pVal` string 
- `pStr` string 

---

```c++
template<> void Converter< char >::ToStr (const char & pVal, std::string & pStr)
```
Specialized implementation handling char to string conversion. 

**Parameters**
- `pVal` char 
- `pStr` string 

---

```c++
template<> void Converter< double >::ToStr (const double & pVal, std::string & pStr)
```
Specialized implementation handling double to string conversion. 

**Parameters**
- `pVal` double 
- `pStr` string 

---

```c++
template<typename T> virtual void ToVal (const std::string & pStr, T & pVal)
```
Converts string holding a numerical value to numerical datatype representation. 

**Parameters**
- `pVal` value 
- `pStr` output string 

---

```c++
template<> void Converter< std::string >::ToVal (const std::string & pStr, std::string & pVal)
```
Specialized implementation handling string to string conversion. 

**Parameters**
- `pVal` string 
- `pStr` string 

---

```c++
template<> void Converter< int >::ToVal (const std::string & pStr, int & pVal)
```
Specialized implementation handling int to string conversion. 

**Parameters**
- `pVal` int 
- `pStr` string 

---

```c++
template<> void Converter< long >::ToVal (const std::string & pStr, long & pVal)
```
Specialized implementation handling long to string conversion. 

**Parameters**
- `pVal` long 
- `pStr` string 

---

```c++
template<> void Converter< long long >::ToVal (const std::string & pStr, long long & pVal)
```
Specialized implementation handling long long to string conversion. 

**Parameters**
- `pVal` long long 
- `pStr` string 

---

```c++
template<> void Converter< unsigned >::ToVal (const std::string & pStr, unsigned & pVal)
```
Specialized implementation handling unsigned to string conversion. 

**Parameters**
- `pVal` unsigned 
- `pStr` string 

---

```c++
template<> void Converter< unsigned long >::ToVal (const std::string & pStr, unsigned long & pVal)
```
Specialized implementation handling unsigned long to string conversion. 

**Parameters**
- `pVal` unsigned long 
- `pStr` string 

---

```c++
template<> void Converter< unsigned long long >::ToVal (const std::string & pStr, unsigned long long & pVal)
```
Specialized implementation handling unsigned long long to string conversion. 

**Parameters**
- `pVal` unsigned long long 
- `pStr` string 

---

```c++
template<> void Converter< float >::ToVal (const std::string & pStr, float & pVal)
```
Specialized implementation handling float to string conversion. 

**Parameters**
- `pVal` float 
- `pStr` string 

---

```c++
template<> void Converter< double >::ToVal (const std::string & pStr, double & pVal)
```
Specialized implementation handling double to string conversion. 

**Parameters**
- `pVal` double 
- `pStr` string 

---

```c++
template<> void Converter< long double >::ToVal (const std::string & pStr, long double & pVal)
```
Specialized implementation handling long double to string conversion. 

**Parameters**
- `pVal` long double 
- `pStr` string 

---

```c++
template<> void Converter< bool >::ToVal (const std::string & pStr, bool & pVal)
```
Specialized implementation handling bool to string conversion. 

**Parameters**
- `pVal` bool 
- `pStr` string 

---

###### API documentation generated using [Doxyman2md](https://github.com/d99kris/doxyman2md)

