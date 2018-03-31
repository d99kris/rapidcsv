## class rapidcsv::SeparatorParams

Datastructure holding parameters controlling how the CSV data fields are separated.  

---

```c++
SeparatorParams (const char pSeparator = ',', const bool pHasCR = sPlatformHasCR)
```
Constructor. 

**Parameters**
- `pSeparator` specifies the column separator (default ','). 
- `pHasCR` specifies whether a new document (i.e. not an existing document read) should use CR/LF instead of only LF (default is to use standard behavior of underlying platforms - CR/LF for Win, and LF for others). 

---

###### API documentation generated using [Doxyman2md](https://github.com/d99kris/doxyman2md)

