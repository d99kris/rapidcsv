## class rapidcsv::LabelParams

Datastructure holding parameters controlling which row and column should be treated as labels.  

---

```c++
LabelParams (const int pColumnNameIdx = 0, const int pRowNameIdx = 0)
```
Constructor. 

**Parameters**
- `pColumnNameIdx` specifies the zero-based row index of the column labels, setting it to -1 prevents column lookup by label name, and gives access to all rows as document data. 
- `pRowNameIdx` specifies the zero-based column index of the row labels, setting it to -1 prevents row lookup by label name, and gives access to all columns as document data. 

---

###### API documentation generated using [Doxyman2md](https://github.com/d99kris/doxyman2md)

