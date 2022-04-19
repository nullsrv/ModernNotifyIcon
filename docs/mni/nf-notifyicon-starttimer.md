# StartTimer function

Start timer with specific interval.

## Syntax

```cpp
auto StartTimer(
    [in] UINT id,
    [in] UINT interval
) -> UINT;
```

## Parameters

`[in] id`</br>
Type: **UINT**

Id of a timer.</br>
Use **MNI_USER_TIMER_ID** for defining custom timers.

`[in] interval`</br>
Type: **UINT**

Interval at which timer will timeout. In milliseconds.

## Return value

Type: **BOOL**

## Remarks

If you call this function before initialization it will always return `FALSE`.
