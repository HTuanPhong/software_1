# CODE OF CONDUCT
## Comment
section seperator
```c
////////////////////////////////////////////////////////////////////////////////
```
note
```c
// NOTE(usr): content
```
todo
```c
// TODO(usr): content
```
## Argument
- function should take pure data type as much as possible.
- only data structure that member value interlink should take struct.
## Misc
header guard
```
#ifndef BASE_NAME_H
#define BASE_NAME_H
#endif //BASE_NAME_H
```
best initialize structure method
- work for both stack and heap
- avoid potential struct copy 
```
MyStruct s;
initMyStruct(&s);
```