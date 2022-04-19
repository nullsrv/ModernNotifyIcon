# StopTimer function

Stop timer.

## Syntax

```cpp
auto StartTimer(
    [in] UINT id
) -> UINT;
```

## Parameters

`[in] id`</br>
Type: **UINT**

Id of a timer.

## Return value

Type: **BOOL**

## Remarks

If you call this function before initialization it will always return `FALSE`.
