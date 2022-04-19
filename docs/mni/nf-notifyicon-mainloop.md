# MainLoop function

Process window messages until quit message is received.

## Syntax

```cpp
auto MainLoop() -> int;
```

## Return value

Type: **int**

Function returns last message ```wParam``` or ```-1``` if GetMessage() call fails.
