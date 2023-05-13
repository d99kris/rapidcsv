## class rapidcsv::Document

Class representing a CSV document.  

---

```c++
Document (const std::string & pPath = std::string(), const LabelParams & pLabelParams = LabelParams(), const SeparatorParams & pSeparatorParams = SeparatorParams(), const ConverterParams & pConverterParams = ConverterParams(), const LineReaderParams & pLineReaderParams = LineReaderParams())
```
Constructor. 

**Parameters**
- `pPath` specifies the path of an existing CSV-file to populate the Document data with. 
- `pLabelParams` specifies which row and column should be treated as labels. 
- `pSeparatorParams` specifies which field and row separators should be used. 
- `pConverterParams` specifies how invalid numbers (including empty strings) should be handled. 
- `pLineReaderParams` specifies how special line formats should be treated. 

---

```c++
Document (std::istream & pStream, const LabelParams & pLabelParams = LabelParams(), const SeparatorParams & pSeparatorParams = SeparatorParams(), const ConverterParams & pConverterParams = ConverterParams(), const LineReaderParams & pLineReaderParams = LineReaderParams())
```
Constructor. 

**Parameters**
- `pStream` specifies a binary input stream to read CSV data from. 
- `pLabelParams` specifies which row and column should be treated as labels. 
- `pSeparatorParams` specifies which field and row separators should be used. 
- `pConverterParams` specifies how invalid numbers (including empty strings) should be handled. 
- `pLineReaderParams` specifies how special line formats should be treated. 

---

```c++
void Clear ()
```
Clears loaded Document data. 

---

```c++
template<typename T > T GetCell (const size_t pColumnIdx, const size_t pRowIdx)
```
Get cell by index. 

**Parameters**
- `pColumnIdx` zero-based column index. 
- `pRowIdx` zero-based row index. 

**Returns:**
- cell data. 

---

```c++
template<typename T > T GetCell (const size_t pColumnIdx, const size_t pRowIdx, ConvFunc< T > pToVal)
```
Get cell by index. 

**Parameters**
- `pColumnIdx` zero-based column index. 
- `pRowIdx` zero-based row index. 
- `pToVal` conversion function. 

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
template<typename T > T GetCell (const std::string & pColumnName, const std::string & pRowName, ConvFunc< T > pToVal)
```
Get cell by name. 

**Parameters**
- `pColumnName` column label name. 
- `pRowName` row label name. 
- `pToVal` conversion function. 

**Returns:**
- cell data. 

---

```c++
template<typename T > T GetCell (const std::string & pColumnName, const size_t pRowIdx)
```
Get cell by column name and row index. 

**Parameters**
- `pColumnName` column label name. 
- `pRowIdx` zero-based row index. 

**Returns:**
- cell data. 

---

```c++
template<typename T > T GetCell (const std::string & pColumnName, const size_t pRowIdx, ConvFunc< T > pToVal)
```
Get cell by column name and row index. 

**Parameters**
- `pColumnName` column label name. 
- `pRowIdx` zero-based row index. 
- `pToVal` conversion function. 

**Returns:**
- cell data. 

---

```c++
template<typename T > T GetCell (const size_t pColumnIdx, const std::string & pRowName)
```
Get cell by column index and row name. 

**Parameters**
- `pColumnIdx` zero-based column index. 
- `pRowName` row label name. 

**Returns:**
- cell data. 

---

```c++
template<typename T > T GetCell (const size_t pColumnIdx, const std::string & pRowName, ConvFunc< T > pToVal)
```
Get cell by column index and row name. 

**Parameters**
- `pColumnIdx` zero-based column index. 
- `pRowName` row label name. 
- `pToVal` conversion function. 

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
template<typename T > std::vector<T> GetColumn (const size_t pColumnIdx, ConvFunc< T > pToVal)
```
Get column by index. 

**Parameters**
- `pColumnIdx` zero-based column index. 
- `pToVal` conversion function. 

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
template<typename T > std::vector<T> GetColumn (const std::string & pColumnName, ConvFunc< T > pToVal)
```
Get column by name. 

**Parameters**
- `pColumnName` column label name. 
- `pToVal` conversion function. 

**Returns:**
- vector of column data. 

---

```c++
size_t GetColumnCount ()
```
Get number of data columns (excluding label columns). 

**Returns:**
- column count. 

---

```c++
int GetColumnIdx (const std::string & pColumnName)
```
Get column index by name. 

**Parameters**
- `pColumnName` column label name. 

**Returns:**
- zero-based column index. 

---

```c++
std::string GetColumnName (const size_t pColumnIdx)
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
template<typename T > std::vector<T> GetRow (const size_t pRowIdx, ConvFunc< T > pToVal)
```
Get row by index. 

**Parameters**
- `pRowIdx` zero-based row index. 
- `pToVal` conversion function. 

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
template<typename T > std::vector<T> GetRow (const std::string & pRowName, ConvFunc< T > pToVal)
```
Get row by name. 

**Parameters**
- `pRowName` row label name. 
- `pToVal` conversion function. 

**Returns:**
- vector of row data. 

---

```c++
size_t GetRowCount ()
```
Get number of data rows (excluding label rows). 

**Returns:**
- row count. 

---

```c++
int GetRowIdx (const std::string & pRowName)
```
Get row index by name. 

**Parameters**
- `pRowName` row label name. 

**Returns:**
- zero-based row index. 

---

```c++
std::string GetRowName (const size_t pRowIdx)
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
template<typename T > void InsertColumn (const size_t pColumnIdx, const std::vector< T > & pColumn = std::vector<T>(), const std::string & pColumnName = std::string())
```
Insert column at specified index. 

**Parameters**
- `pColumnIdx` zero-based column index. 
- `pColumn` vector of column data (optional argument). 
- `pColumnName` column label name (optional argument). 

---

```c++
template<typename T > void InsertRow (const size_t pRowIdx, const std::vector< T > & pRow = std::vector<T>(), const std::string & pRowName = std::string())
```
Insert row at specified index. 

**Parameters**
- `pRowIdx` zero-based row index. 
- `pRow` vector of row data (optional argument). 
- `pRowName` row label name (optional argument). 

---

```c++
void Load (const std::string & pPath, const LabelParams & pLabelParams = LabelParams(), const SeparatorParams & pSeparatorParams = SeparatorParams(), const ConverterParams & pConverterParams = ConverterParams(), const LineReaderParams & pLineReaderParams = LineReaderParams())
```
Read Document data from file. 

**Parameters**
- `pPath` specifies the path of an existing CSV-file to populate the Document data with. 
- `pLabelParams` specifies which row and column should be treated as labels. 
- `pSeparatorParams` specifies which field and row separators should be used. 
- `pConverterParams` specifies how invalid numbers (including empty strings) should be handled. 
- `pLineReaderParams` specifies how special line formats should be treated. 

---

```c++
void Load (std::istream & pStream, const LabelParams & pLabelParams = LabelParams(), const SeparatorParams & pSeparatorParams = SeparatorParams(), const ConverterParams & pConverterParams = ConverterParams(), const LineReaderParams & pLineReaderParams = LineReaderParams())
```
Read Document data from stream. 

**Parameters**
- `pStream` specifies a binary input stream to read CSV data from. 
- `pLabelParams` specifies which row and column should be treated as labels. 
- `pSeparatorParams` specifies which field and row separators should be used. 
- `pConverterParams` specifies how invalid numbers (including empty strings) should be handled. 
- `pLineReaderParams` specifies how special line formats should be treated. 

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
- `pStream` specifies a binary output stream to write the data to. 

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
template<typename T > void SetCell (const size_t pColumnIdx, const std::string & pRowName, const T & pCell)
```
Set cell by column index and row name. 

**Parameters**
- `pColumnIdx` zero-based column index. 
- `pRowName` row label name. 
- `pCell` cell data. 

---

```c++
template<typename T > void SetCell (const std::string & pColumnName, const size_t pRowIdx, const T & pCell)
```
Set cell by column name and row index. 

**Parameters**
- `pColumnName` column label name. 
- `pRowIdx` zero-based row index. 
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

###### API documentation generated using [Doxygenmd](https://github.com/d99kris/doxygenmd)

