# Init function

Perform initialization. This function will create invisible window, required for all the calls to
shell Notification Area.
You want to call this function after object creation.

## Syntax

```cpp
auto Init() -> HRESULT;
```

```cpp
auto Init(
    [in] const Desc& desc
) -> HRESULT;
```

## Parameters

`[in] desc`</br>
Type: **const [Desc](ns-notifyicon-desc.md)**

Description used to initialize NotifyIcon object.

## Return value

This function returns one of the following values.

Return code                        | Type    | Description
---------------------------------- | ------- | --------------------------------------------------
MNI_SUCCESS                        | Success | The function successfully created invisible window
MNI_WINDOW_ALREADY_CREATED         | Warning | Window was already created
MNI_INVALID_INSTANCE               | Error   | hInstance value is NULL
MNI_FAILED_TO_REGISTER_WNDCLASS    | Error   | Call to RegisterClassExW failed
MNI_FAILED_TO_CREATE_WINDOW        | Error   | Call to CreateWindow failed

## Remarks

If internal hInstance value is NULL, this function will first try to obtain instance handle by
calling GetModuleHandleW(nullptr) function. If that fails, MNI_INVALID_INSTANCE is returned.
