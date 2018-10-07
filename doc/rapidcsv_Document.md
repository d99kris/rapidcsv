## class rapidcsv::Document

Class representing a CSV document.  

---

```c++
Document (const std::string & pPath = std::string(), const LabelParams & pLabelParams = LabelParams(), const SeparatorParams & pSeparatorParams = SeparatorParams(), const ConverterParams & pConverterParams = ConverterParams())
```
Constructor. 

**Parameters**
- `pPath` specifies the path of an existing CSV-file to populate the Document data with. 
- `pLabelParams` specifies which row and column should be treated as labels. 
- `pSeparatorParams` specifies which field and row separators should be used. 
- `pConverterParams` specifies how invalid numbers (including empty strings) should be handled. 

---

```c++
Document (std::istream & pStream, const LabelParams & pLabelParams = LabelParams(), const SeparatorParams & pSeparatorParams = SeparatorParams(), const ConverterParams & pConverterParams = ConverterParams())
```
Constructor. 

**Parameters**
- `pStream` specifies an input stream to read CSV data from. 
- `pLabelParams` specifies which row and column should be treated as labels. 
- `pSeparatorParams` specifies which field and row separators should be used. 
- `pConverterParams` specifies how invalid numbers (including empty strings) should be handled. 

---

```c++
Document (const Document & pDocument)
```
Copy constructor. 

**Parameters**
- `pDocument` specifies the Document instance to copy. 

---

```c++
template<typename T > T GetCell (const size_t pColumnIdx, const size_t pRowIdx)
```
Get cell by index. 

**Parameters**
- `pRowIdx` zero-based row index. 
- `pColumnIdx` zero-based column index. 

**Returns:**
- cell data. 

---

```c++
template<typename T > T GetCell (const std::string & pColumnName, const std::string & pRowName)
```
Get cell by name. 

**Parameters**
- `pColumnName` column label name. 
- `pRowName` row label name. 

**Returns:**
- cell data. 

---

```c++
template<typename T > std::vector<T> GetColumn (const size_t pColumnIdx)
```
Get column by index. 

**Parameters**
- `pColumnIdx` zero-based column index. 

**Returns:**
- vector of column data. 

---

```c++
template<typename T > std::vector<T> GetColumn (const std::string & pColumnName)
```
Get column by name. 

**Parameters**
- `pColumnName` column label name. 

**Returns:**
- vector of column data. 

---

```c++
size_t GetColumnCount ()
```
Get number of data columns. 

**Returns:**
- column count. 

---

```c++
std::string GetColumnName (const ssize_t pColumnIdx)
```
Get column name. 

**Parameters**
- `pColumnIdx` zero-based column index. 

**Returns:**
- column name. 

---

```c++
std::vector<std::string> GetColumnNames ()
```
Get column names. 

**Returns:**
- vector of column names. 

---

```c++
template<typename T > std::vector<T> GetRow (const size_t pRowIdx)
```
Get row by index. 

**Parameters**
- `pRowIdx` zero-based row index. 

**Returns:**
- vector of row data. 

---

```c++
template<typename T > std::vector<T> GetRow (const std::string & pRowName)
```
Get row by name. 

**Parameters**
- `pRowName` row label name. 

**Returns:**
- vector of row data. 

---

```c++
size_t GetRowCount ()
```
Get number of data rows. 

**Returns:**
- row count. 

---

```c++
std::string GetRowName (const ssize_t pRowIdx)
```
Get row name. 

**Parameters**
- `pRowIdx` zero-based column index. 

**Returns:**
- row name. 

---

```c++
std::vector<std::string> GetRowNames ()
```
Get row names. 

**Returns:**
- vector of row names. 

---

```c++
void Load (const std::string & pPath)
```
Read Document data from file. 

**Parameters**
- `pPath` specifies the path of an existing CSV-file to populate the Document data with. 

---

```c++
void RemoveColumn (const size_t pColumnIdx)
```
Remove column by index. 

**Parameters**
- `pColumnIdx` zero-based column index. 

---

```c++
void RemoveColumn (const std::string & pColumnName)
```
Remove column by name. 

**Parameters**
- `pColumnName` column label name. 

---

```c++
void RemoveRow (const size_t pRowIdx)
```
Remove row by index. 

**Parameters**
- `pRowIdx` zero-based row index. 

---

```c++
void RemoveRow (const std::string & pRowName)
```
Remove row by name. 

**Parameters**
- `pRowName` row label name. 

---

```c++
void Save (const std::string & pPath = std::string())
```
Write Document data to file. 

**Parameters**
- `pPath` optionally specifies the path where the CSV-file will be created (if not specified, the original path provided when creating or loading the Document data will be used). 

---

```c++
void Save (std::ostream & pStream)
```
Write Document data to stream. 

**Parameters**
- `pStream` specifies an output stream to write the data to. 

---

```c++
template<typename T > void SetCell (const size_t pColumnIdx, const size_t pRowIdx, const T & pCell)
```
Set cell by index. 

**Parameters**
- `pRowIdx` zero-based row index. 
- `pColumnIdx` zero-based column index. 
- `pCell` cell data. 

---

```c++
template<typename T > void SetCell (const std::string & pColumnName, const std::string & pRowName, const T & pCell)
```
Set cell by name. 

**Parameters**
- `pColumnName` column label name. 
- `pRowName` row label name. 
- `pCell` cell data. 

---

```c++
template<typename T > void SetColumn (const size_t pColumnIdx, const std::vector< T > & pColumn)
```
Set column by index. 

**Parameters**
- `pColumnIdx` zero-based column index. 
- `pColumn` vector of column data. 

---

```c++
template<typename T > void SetColumn (const std::string & pColumnName, const std::vector< T > & pColumn)
```
Set column by name. 

**Parameters**
- `pColumnName` column label name. 
- `pColumn` vector of column data. 

---

```c++
void SetColumnName (size_t pColumnIdx, const std::string & pColumnName)
```
Set column name. 

**Parameters**
- `pColumnIdx` zero-based column index. 
- `pColumnName` column name. 

---

```c++
template<typename T > void SetRow (const size_t pRowIdx, const std::vector< T > & pRow)
```
Set row by index. 

**Parameters**
- `pRowIdx` zero-based row index. 
- `pRow` vector of row data. 

---

```c++
template<typename T > void SetRow (const std::string & pRowName, const std::vector< T > & pRow)
```
Set row by name. 

**Parameters**
- `pRowName` row label name. 
- `pRow` vector of row data. 

---

```c++
void SetRowName (size_t pRowIdx, const std::string & pRowName)
```
Set row name. 

**Parameters**
- `pRowIdx` zero-based row index. 
- `pRowName` row name. 

---

###### API documentation generated using [Doxyman2md](https://github.com/d99kris/doxyman2md)

