## class rapidcsv::LabelParams

Datastructure holding parameters controlling which row and column should be treated as labels.  

---

```c++
LabelParams (const int pColumnNameIdx = 0, const int pRowNameIdx = \-1)
```
Constructor. 

**Parameters**
- `pColumnNameIdx` specifies the zero-based row index of the column labels, setting it to -1 prevents column lookup by label name, and gives access to all rows as document data. Default: 0 
- `pRowNameIdx` specifies the zero-based column index of the row labels, setting it to -1 prevents row lookup by label name, and gives access to all columns as document data. Default: -1 

---

###### API documentation generated using [Doxygenmd](https://github.com/d99kris/doxygenmd)

