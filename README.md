# objectifier
Objective programming in C!

## How to install
Just copy `objectifier` directory to your source directory and include `objectifier.h` and add `objectifier.c` to the files you compile.

## Documentation
```c
#define OCR_SIMPLY_NAMES
```
Enables simplified names for functions e.g. `OCR_class` — `class`. `OCR_free`, `OCR_free_all` and internal names were not simplified.

`OCR_free_all()` - Frees all the initiated classes.

`OCR_free(class)` - Frees only class that you gave to it.

`OCR_method(type, name, ...)` - Creates method inside a class.

`OCR_memberize(class, class_name, method_name)` - Connects method from class with it representant function.

`OCR_call(class, name, ..)` - Calls method from class.

`OCR_init(type, ...)`, `OCR_init_dyn(type, ...)` - Creates instance of class.

`OCR_return(variable)` - Executes collector and returns variable like standard return.

## Used libraries
[uthash](https://github.com/troydhanson/uthash) - Amazing hashmap library!