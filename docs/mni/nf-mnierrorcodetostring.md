# MniErrorCodeToString function

Get string of error code.

## Syntax

```cpp
constexpr auto MniErrorCodeToString(
    const HRESULT code
) -> std::wstring_view;

```

## Parameters

`[in] code`</br>
Type: **HRESULT**

MNI_ERROR_CODE error code.

## Return value

String representation of MNI_ERROR_CODE.
