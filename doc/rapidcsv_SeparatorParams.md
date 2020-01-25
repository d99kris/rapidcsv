## class rapidcsv::SeparatorParams

Datastructure holding parameters controlling how the CSV data fields are separated.  

---

```c++
SeparatorParams (const char pSeparator = ',', const bool pTrim = false, const bool pHasCR = sPlatformHasCR, const bool pQuotedLinebreaks = false)
```
Constructor. 

**Parameters**
- `pSeparator` specifies the column separator (default ','). 
- `pTrim` specifies whether to trim leading and trailing spaces from cells read. 
- `pHasCR` specifies whether a new document (i.e. not an existing document read) should use CR/LF instead of only LF (default is to use standard behavior of underlying platforms - CR/LF for Win, and LF for others). 
- `pQuotedLinebreaks` specifies whether to allow line breaks in quoted text. 

---

###### API documentation generated using [Doxyman2md](https://github.com/d99kris/doxyman2md)

