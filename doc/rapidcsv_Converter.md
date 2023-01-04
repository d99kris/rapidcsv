## class rapidcsv::Converter< T >

Class providing conversion to/from numerical datatypes and strings. Only intended for rapidcsv internal usage, but exposed externally to allow specialization for custom datatype conversions.  

---

```c++
template<typename T> Converter (const ConverterParams & pConverterParams)
```
Constructor. 

**Parameters**
- `pConverterParams` specifies how conversion of non-numerical values to numerical datatype shall be handled. 

---

```c++
template<typename T> void ToStr (const T & pVal, std::string & pStr)
```
Converts numerical value to string representation. 

**Parameters**
- `pVal` numerical value 
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
template<typename T> void ToVal (const std::string & pStr, T & pVal)
```
Converts string holding a numerical value to numerical datatype representation. 

**Parameters**
- `pVal` numerical value 
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

###### API documentation generated using [Doxygenmd](https://github.com/d99kris/doxygenmd)

