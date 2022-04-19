# SendCustomMessage function

Send custom message. This function is thread safe.

## Syntax

```cpp
auto SendCustomMessage(
    [in] UINT   msg,
    [in] WPARAM wParam,
    [in] LPARAM lParam
) -> BOOL;
```

## Parameters

`[in] msg`</br>
Type: **UINT**

Id of a message.</br>
Use **MNI_USER_MESSAGE_ID** for defining custom messages.

`[in] wParam`</br>
Type: **WPARAM**

User data passed with message.

`[in] lParam`</br>
Type: **LPARAM**

User data passed with message.

## Return value

Type: **BOOL**

## Remarks

If you call this function before initialization it will always return `FALSE`.
