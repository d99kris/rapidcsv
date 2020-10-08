## class rapidcsv::ConverterParams

Datastructure holding parameters controlling how invalid numbers (including empty strings) should be handled.  

---

```c++
ConverterParams (const bool pHasDefaultConverter = false, const long double pDefaultFloat = std::numeric_limits<long double>::signaling_NaN(), const long long pDefaultInteger = 0)
```
Constructor. 

**Parameters**
- `pHasDefaultConverter` specifies if conversion of non-numerical strings shall be converted to a default numerical value, instead of causing an exception to be thrown (default). 
- `pDefaultFloat` floating-point default value to represent invalid numbers. 
- `pDefaultInteger` integer default value to represent invalid numbers. 

---

###### API documentation generated using [Doxygenmd](https://github.com/d99kris/doxygenmd)

